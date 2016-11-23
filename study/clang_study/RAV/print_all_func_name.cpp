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

class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
public :
  MyVisitor(ASTContext *Context) : context(Context) {}

  bool VisitStmt(Stmt *S) {

    if(isa<CallExpr>(*S)) {
      CallExpr *callex = dyn_cast<CallExpr>(S);
      FunctionDecl *callee = callex->getDirectCallee();
      DeclarationNameInfo name_info = callee->getNameInfo();
      fprintf(stdout, " func name %s \n", ((name_info.getName()).getAsString()).c_str());

    }
    return true;
  }

#if 0
  bool VisitDecl(Decl *D) {

    if(isa<FunctionDecl>(*D)) {
      FunctionDecl *fd = dyn_cast<FunctionDecl>(D);

      DeclarationNameInfo name_info = fd->getNameInfo();
      string curr_func_name = ((name_info.getName()).getAsString());
      func_name = curr_func_name;

      if(curr_func_name == topLevel_fName) {
        start_print = true;
      } else {
        start_print = false;
      }
      
    }
    return true;
  }
#endif

private :
  ASTContext *context;
};

class MyConsumer : public ASTConsumer {
public :
  void HandleTranslationUnit(ASTContext &Context) {
    MyVisitor visitor(&Context);
    visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

  virtual bool HandleTopLevelDecl(clang::DeclGroupRef d) {
    return true;
  }
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
                              clang::LangStandard::lang_opencl11);
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

  return 0;
}
