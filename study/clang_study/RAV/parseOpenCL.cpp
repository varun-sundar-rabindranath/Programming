#include <stdio.h>
#include<iostream>
#include <cmath>
using namespace std;

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/HeaderSearchOptions.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Parse/Parser.h"

#include "clang/Basic/SourceManagerInternals.h"

#include "clang/Basic/FileSystemOptions.h"

#include "clang/AST/Decl.h"

#include "clang/AST/Stmt.h"
#include "clang/AST/StmtIterator.h"

#include <map>
#include <vector>
#include <utility>
#include <string>
#include <queue>

using namespace llvm;
using namespace clang::SrcMgr;
using namespace clang;

enum addressSpecifier {
  NONE,
  __global,
  __local,
  __constant,
  __private
};

struct memoryBlock {
  int row;
  int column;
  int numRows;
  int numCols;

  memoryBlock() {
    row = -1;
    column = -1;
    numRows = -1;
    numCols = -1;
  }
};

struct memoryProperties {
  bool memoryReadDone;
  bool memoryWriteDone;

  memoryProperties(){
    memoryReadDone = false;
    memoryWriteDone = false;
  }
};

struct bufferDetails {
  string bufferName;
  addressSpecifier addrSpec;
  struct memoryProperties *memProp;
  struct memoryBlock *memBlock;

  bufferDetails() {
    bufferName = "";
    addrSpec = NONE;
    memProp = new memoryProperties;
    memBlock = new memoryBlock;
  }
};

string currentFunctionName;
bool isALocalAddressSpaceVariable = false;
bool isAnAssignment = false;

/* Queue used to do a BFS on trees with BinaryOperator as root */
queue<Stmt *> AST_BFSQueue;

/* All buffers are stored here */
vector<struct bufferDetails *> buffers;

/* All local variables in the OpenCL code and their dimLengths */
map< string, vector<int> > LocalAddressSpaceVariable;

/* Variables in LHS of an assignment operator */
vector< vector<string> > LHSVariables;

/* Variables in RHS of an assignment operator */
vector< vector<string> > RHSVariables;

/* Local functions used in this file (Coulbe be moved to a separate header afterwards)*/

/* Given an array declaration string, This function gets each dimension length
 * in the array; For example, for a declaration "int a[10][20];", the vector
 * dimLengths is expected to have the values 20,10 when returning. The order
 * "20, 10" is correct since we iterate the declaration from the reverse;
 * Handle this correctly in the calling code;
 * Returns true on Success; Returns false on some failure;
 */
bool getDimLengths(string arrayDeclaration, vector<int> &dimLengths) {

  size_t strLength = arrayDeclaration.length();

  int matchingBracketsVariable = 0;
  /*
   * Iterating from the reverse, calculating the values inside square brackets;
   * Why iterate from reverse ? because the value calulation inside the brackets,
   * will be easier if we do so.
   * If we dont find matching brackets, return false;
   * If everything OK return true;
   */
  for(int i = strLength-1; i >= 0; i--) {
    if(arrayDeclaration[i] == '[') {
      matchingBracketsVariable--;
    }
    else if(arrayDeclaration[i] == ']') {
      matchingBracketsVariable++;
    }

    /* If we dont have matching brackets; return false */
    if(!(matchingBracketsVariable == 0 || matchingBracketsVariable == 1)) {
      return false;
    }

    /* If we have encountered a closing bracket; Calculate the value in the bracket */
    /* FIXME : This code allows declaration like "int a[1 0][2 0];"; However this
     * will be caught by the OpenCL compiler so we need not worry about it;
     * Fix it if you have time
     */
    if(matchingBracketsVariable == 1) {
      int value = 0;
      int powerOfTen = 0;
      int j = 0;
      for( j=i-1; (   (int(arrayDeclaration[j])) >= 48
                      && (int(arrayDeclaration[j])) <= 57)
                      || (arrayDeclaration[j] == ' '); j--) {
        if(arrayDeclaration[j] == ' ') {
          continue;
        }

        value += (arrayDeclaration[j]-48) * pow(10, powerOfTen);
        powerOfTen++;
      }
      /* 'i' continues iteration from where 'j' left off */
      i = j+1;

      dimLengths.push_back(value);
    }
  }
  return true;
}

/* Detects variables in the given expression and fills the vector with the
 * detected variable names; If the expression has no variables the vector is
 * is returned unmodified;
 * Returns true on Success;
 * Returns false on some error;
 */
bool detectVariablesAndFillVector(Expr *E, vector<string> &variableNames) {

  Stmt *S = dyn_cast<Stmt>(E);
  AST_BFSQueue.push(S);
  S->dumpColor();

  /* Do BFS */
  while(AST_BFSQueue.size() != 0) {
    Stmt *Statement = AST_BFSQueue.front();

    if(isa<DeclRefExpr>(Statement)) {
      DeclRefExpr *der = dyn_cast<DeclRefExpr>(Statement);
      DeclarationNameInfo nameInfo = der->getNameInfo();
      variableNames.push_back(nameInfo.getAsString());
      fprintf(stderr, "Inside DeclRefExpr : varname %s \n", nameInfo.getAsString().c_str());
    }

    clang::Stmt::child_iterator ci;
    for( ci = Statement->child_begin(); ci != Statement->child_end(); ci++) {
      AST_BFSQueue.push(*ci);
    }

    AST_BFSQueue.pop();
  }
  return true;
}

class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
public :
  MyVisitor(ASTContext *Context) : context(Context) {}

  bool VisitStmt(Stmt *S) {

    if(isa<BinaryOperator>(S)) {
      BinaryOperator *binOp = dyn_cast<BinaryOperator>(S);

      if(binOp->isAssignmentOp()) {

        vector<string> LHSVariableNames = vector<string>();
        vector<string> RHSVariableNames = vector<string>();

        /* LHS Expression */
        Expr *LHSExpr = binOp->getLHS();
        detectVariablesAndFillVector(LHSExpr, LHSVariableNames);

        /* RHS Expression */
        Expr *RHSExpr = binOp->getRHS();
        detectVariablesAndFillVector(RHSExpr, RHSVariableNames);

        /* Push the got variable name vector<string> in to the global variable */
        LHSVariables.push_back(LHSVariableNames);
        RHSVariables.push_back(RHSVariableNames);

        /*
         * Further processing of LHS and RHS
         * If it is a load from global to local
         * calculate the mapping of indices
         */

        /* Check if LHS is a __local */
        bool isLocal = false;
        for(size_t i=0; i<LHSVariableNames.size(); i++) {

        }




      }
    }
    return true;
  }

#if 0
  bool VisitType(Type *T) {
    if(isALocalAddressSpaceVariable) {
      if(T->isConstantArrayType()) {
        fprintf(stderr,"__local Array type is constant \n");
      }
    }
    isALocalAddressSpaceVariable = false;
    return true;
  }

  bool VisitType(TypeLoc TL) {
    fprintf(stderr, "\n========= Visting TypeLoc ==============\n");
    fprintf(stderr, "Full data size : %u \n", TL.getFullDataSize());
    return true;
  }
  bool VisitQualifiedTypeLoc(QualifiedTypeLoc TL) {
    fprintf(stderr, "\n======== Visiting QualifiedTypeLoc ============\n");
    fprintf(stderr, "Local data size : %u \n", TL.getLocalDataSize());
    return true;
  }
  bool VisitUnqualTypeLoc(UnqualTypeLoc TL) {
    /* What to print ? */
    fprintf(stderr, "\n============= Visiting UnqualTypeLoc ===============\n");
    return true;
  }
#endif
  bool VisitDecl(Decl *D) {

    if(isa<VarDecl>(*D)) {
      VarDecl *vd = dyn_cast<VarDecl>(D);

      StorageClass storage_class;
      storage_class = vd->getStorageClass();

      /* FIXME : Change isFunctionOrMethodVarDecl to isLocalVarDecl */
      if(vd->isFunctionOrMethodVarDecl()) {
        if(isa<ValueDecl>(*D)) {
          ValueDecl *val_decl = dyn_cast<ValueDecl>(D);
          fprintf(stderr, "Function name : %s | Variable name : %s | Type : %s \n", 
                                  currentFunctionName.c_str(),
                                  string(vd->getName()).c_str(),
                                  (val_decl->getType().getAsString()).c_str());
          if(storage_class == SC_OpenCLWorkGroupLocal) {

            string localType = val_decl->getType().getAsString();
            vector<int> reverseDimLengths, dimLengths;
            if(!getDimLengths(localType, reverseDimLengths)) {
              fprintf(stderr, "Failed on getDimLengths \n");
              return false;
            }

            /* Put dimension lengths in the right order */
            for(int i = (int)(reverseDimLengths.size()-1); i >= 0; i--) {
              dimLengths.push_back(reverseDimLengths[i]);
            }

            /* Map the variable and the dimLengths got */
            LocalAddressSpaceVariable[vd->getName()] = dimLengths;
          }
        }
      }
    }
    /* If it is a function; print its name */
    if(D->isFunctionOrFunctionTemplate())
    {
      FunctionDecl *function_decl = D->getAsFunction();
      /* If the kernel function has body, Interate its parmVarDecl to list 
       * global variables.
       */
      if(function_decl->hasBody()) {
        for(clang::FunctionDecl::param_iterator it = function_decl->param_begin();
                                                it != function_decl->param_end();
                                                it++) {
          ParmVarDecl *pvd = *it;
          string paramName = pvd->getName();
          string typeName = pvd->getOriginalType().getAsString();
          fprintf(stderr, "Param name : %s | Param type %s \n",
                                                      paramName.c_str(),
                                                      typeName.c_str());
          /* Check if it is a "__global" variable */
          string globalKeyWord1 = " __global ";
          string globalKeyWord2 = "__global ";

          size_t searchResult_KeyWord1 = typeName.find(globalKeyWord1);
          size_t searchResult_KeyWord2 = typeName.find(globalKeyWord2);

          /* " __global " can be anywhere in the text */
          /* "__global " should be in the first position; Because
           * "int * int__global ;" also has "__global "
           */
          if(   searchResult_KeyWord1 != string::npos
             || searchResult_KeyWord2 == 0) {
            struct bufferDetails *buffer = new bufferDetails();
            buffer->bufferName = paramName;
            buffer->addrSpec = __global;
            buffers.push_back(buffer);
          }

          /* Check if it is a __constant varaible */
          string constantKeyWord1 = " __constant ";
          string constantKeyWord2 = "__constant ";

          searchResult_KeyWord1 = typeName.find(constantKeyWord1);
          searchResult_KeyWord2 = typeName.find(constantKeyWord2);

          /* " __constant " can be anywhere in the text */
          /* "__constant " should be in the first position; Because
           * "int * int__constant ;" also has "__constant "
           */
          if(   searchResult_KeyWord1 != string::npos
             || searchResult_KeyWord2 == 0) {
            struct bufferDetails *buffer = new bufferDetails();
            buffer->bufferName = paramName;
            buffer->addrSpec = __constant;
            buffers.push_back(buffer);
          }

          /* Check if it is a __local varaible */
          /* We dont support __local variables in function declaration now */
          string localKeyWord1 = " __local ";
          string localKeyWord2 = "__local ";

          searchResult_KeyWord1 = typeName.find(localKeyWord1);
          searchResult_KeyWord2 = typeName.find(localKeyWord2);
          /* " __local " can be anywhere in the text */
          /* "__local " should be in the first position; Because
           * "int * int__local ;" also has "__local "
           */
          if(   searchResult_KeyWord1 != string::npos
             || searchResult_KeyWord2 == 0) {
            assert(0 && "We dont support this now \n");
          }


        }

      }
      DeclarationNameInfo nameinfo = function_decl->getNameInfo();
      currentFunctionName = nameinfo.getAsString();
    }
    return true;
  }

private :
  ASTContext *context;
};

class MyConsumer : public ASTConsumer {
public :
  void HandleTranslationUnit(ASTContext &Context) {
    MyVisitor visitor(&Context);
    visitor.TraverseDecl(Context.getTranslationUnitDecl());

  }

#if 0
  bool HandleTopLevelDecl(DeclGroupRef D) {
    fprintf(stderr, "\n================HandleTopLevelDecl====================\n");
    DeclGroupRef::iterator it;
    fprintf(stderr, " Single decl ? %d \n", D.isSingleDecl());
    for(it = D.begin(); it != D.end(); it++) {
      fprintf(stderr, "\n ==============Decl===================\n");
      Decl *d = *it;
      d->dumpColor();
      if(isa<VarDecl>(*it)) {
        fprintf(stderr, "This dump has is a result of a vardecl\n");
      }
      fprintf(stderr, "\n ==============Decl===================\n");
    }
    fprintf(stderr, "\n================HandleTopLevelDecl====================\n");
    return false;
  }
#endif
};


int main(int argc, char *argv[]) {
  using clang::CompilerInstance;
  using clang::TargetOptions;
  using clang::TargetInfo;
  using clang::DiagnosticOptions;
  using clang::TextDiagnosticPrinter;
  using clang::FileEntry;
  using clang::FileID;
  using clang::Token;
  using clang::CompilerInvocation;
  using clang::ASTUnit;
  using clang::FunctionDecl;


  /*=+======================= Create compiler instance and set correct language options for compiler Invocation ====================+=*/
  CompilerInstance ci;

  CompilerInvocation &Invocation = ci.getInvocation();
  LangOptions *InvocationLangOpts = Invocation.getLangOpts();
  LangOptions langOpts;
  langOpts.OpenCL= 1;
  Invocation.setLangDefaults(langOpts,
                              clang::IK_OpenCL,
                              clang::LangStandard::lang_opencl);
  (*InvocationLangOpts) = langOpts;
  /*=+======================= Create compiler instance and set correct language options for compiler Invocation ====================+=*/

  /*=+========= Create diagnostics =============+=*/
  DiagnosticOptions diagnosticoptions;
  ci.createDiagnostics();
  /*=+========= Create diagnostics =============+=*/

  /*=+================ Preprocessor creation ================+=*/
  /* Getting target information */
  llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
  pto->Triple = llvm::sys::getDefaultTargetTriple();
  TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(),
                                                 pto.getPtr());
  /* Creating necessadry objects to create a Preprocessor */
  ci.setTarget(pti);
  ci.createFileManager();
  ci.createSourceManager(ci.getFileManager());

  /* Getting preprocessor */
  ci.createPreprocessor(clang::TU_Complete);
  ci.getPreprocessorOpts().UsePredefines = false;
  /*=+================ Preprocessor creation ================+=*/

  /*=+============== Creating AST-consumer, AST-context ==============+=*/
  clang::ASTConsumer *astconsumer = new MyConsumer();
  ci.setASTConsumer(astconsumer);
  ci.createASTContext();
  /*=+============== Creating AST-consumer, AST-context ==============+=*/

  /*=+============== Setting main file id ====================+=*/
  const FileEntry *pFile = ci.getFileManager().getFile(argv[1]);
  FileID fid = ci.getSourceManager().createFileID(pFile, clang::SourceLocation(),\
                                                  (clang::SrcMgr::CharacteristicKind)C_ExternCSystem);

  ci.getSourceManager().setMainFileID(fid);
  /*=+============== Setting main file id ====================+=*/

  /*=+============== Parsing input file ====================+=*/
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());
  clang::ParseAST(ci.getPreprocessor(), astconsumer, ci.getASTContext());
  ci.getDiagnosticClient().EndSourceFile();
  /*=+============== Parsing input file ====================+=*/

  /*=+============== Printing the map =====================+=*/
  map<string, vector<int> >::iterator it;
  for( it = LocalAddressSpaceVariable.begin();
       it != LocalAddressSpaceVariable.end();
       it++) {
    vector<int> currDimLengths = it->second;
    cout<< " Variable name : "<< it->first<< " : ";
    for( size_t i = 0; i < currDimLengths.size(); i++) {
      cout<<currDimLengths[i]<<"  ";
    }
    cout<<endl;
  }
  /*=+============== Printing the map =====================+=*/

  /*=+============== Printing buffers =====================+=*/
  for(size_t i = 0; i < buffers.size(); i++) {
    struct bufferDetails * buffer = buffers[i];
    cout<< " Param name : "<<buffer->bufferName<< " AddressSpec : "<<buffer->addrSpec<<endl;
  }
  /*=+============== Printing buffers =====================+=*/

  /*=+======== Printing variables on either sides of assignment ============+=*/
  assert(LHSVariables.size() == RHSVariables.size());
  for( int i = 0; i < LHSVariables.size(); i++) {
    vector<string> LHSVariableNames = LHSVariables[i];
    vector<string> RHSVariableNames = RHSVariables[i];

    /* Printing LHS Variables */
    for(int j = 0; j < LHSVariableNames.size(); j++) {
      cout<<LHSVariableNames[j]<<"  ";
    }
    cout<<" = ";
    for(int j = 0; j < RHSVariableNames.size(); j++) {
      cout<<RHSVariableNames[j]<<"  ";
    }
    cout<<endl;

  }

  /*=+======== Printing variables on either sides of assignment ============+=*/
  return 0;
}
