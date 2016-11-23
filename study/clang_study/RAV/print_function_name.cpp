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

bool start_print = false;

string topLevel_fName;
string func_name;

queue <Decl *> functions_to_examine;

vector<vector<string> > file_function_map;

class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
public :
  MyVisitor(ASTContext *Context){
    context = Context;
  }

  bool VisitStmt(Stmt *S) {
    /* Enqueue in vector, the function list as in call-graph */
    if(isa<CallExpr>(*S)) {
      CallExpr *callex = dyn_cast<CallExpr>(S);
      FunctionDecl *callee = callex->getDirectCallee();
      DeclarationNameInfo name_info = callee->getNameInfo();

      string current_function_name = name_info.getAsString();

      Decl *func_decl = dyn_cast<Decl>(callee);
      functions_to_examine.push(func_decl);

      if(func_decl->isFunctionOrFunctionTemplate()) {
        FunctionDecl *fd = func_decl->getAsFunction();
        
        DeclarationNameInfo name_info = fd->getNameInfo();
        string function_name = (fd->getNameInfo()).getAsString();
        
        SourceLocation sl = name_info.getLoc();

        string location = (sl.printToString(context->getSourceManager()));
        size_t colon_pos = location.find_first_of(':');
        string file_name = location.substr(0, colon_pos);

        bool function_mapped = false;
        for(int i = 0; i < file_function_map.size(); i++) {

          if(file_function_map[i][0] == file_name) {
            /* We already have some function in this header file */

            for(int j = 1; j < file_function_map[i].size(); j++) {
              /* Determine if we have already enlisted the current function */
              if(file_function_map[i][j] == current_function_name) {
                function_mapped = true;
              }
            }

            /* We have not already enlisted; so enlist now */
            if(!function_mapped) {
            file_function_map[i].push_back(current_function_name);
            function_mapped = true;
            }
            break;
          }
        }

        /* This is a new file; create entry for file and enlist the current function */
        if(!function_mapped){
          file_function_map.push_back(vector<string>());
          file_function_map[file_function_map.size() - 1].push_back(file_name);
          file_function_map[file_function_map.size() - 1].push_back(current_function_name);
          function_mapped = true;
        }

      }
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
#if 0
  void HandleTranslationUnit(ASTContext &Context) {
    MyVisitor visitor(&Context);
    visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
#endif

  virtual bool HandleTopLevelDecl(clang::DeclGroupRef d) {
    clang::DeclGroupRef::iterator it;
    for(it = d.begin(); it != d.end(); it++) {
      clang::FunctionDecl *fd = llvm::dyn_cast<clang::FunctionDecl>(*it);
      if(!fd) { continue; }

      string curr_func_name = (fd->getNameInfo()).getAsString();
      if(curr_func_name == topLevel_fName) {
        functions_to_examine.push(*it);
        break;
      }
    }
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

  assert(argc == 3);

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

  /*------------------Add header paths-------------------------*/
  llvm::IntrusiveRefCntPtr<clang::HeaderSearchOptions> hso = \
                                      new clang::HeaderSearchOptions();

  clang::HeaderSearch headersearch(
                                    hso,
                                    ci.getSourceManager(),
                                    ci.getDiagnostics(),
                                    ci.getLangOpts(),
                                    pti);

  hso->AddPath("/home/varun/blenderDev/blender/intern/cycles/kernel/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/intern/cycles/kernel/closure/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/intern/cycles/kernel/svm/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/intern/cycles/kernel/shaders/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/intern/cycles/kernel/geom/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/build/bin/2.72/scripts/addons/cycles/kernel/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/build/bin/2.72/scripts/addons/cycles/kernel/closure/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/build/bin/2.72/scripts/addons/cycles/kernel/svm/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/build/bin/2.72/scripts/addons/cycles/kernel/shaders/",
               clang::frontend::Quoted,
               false,
               false);

  hso->AddPath("/home/varun/blenderDev/blender/build/bin/2.72/scripts/addons/cycles/kernel/geom/",
               clang::frontend::Quoted,
               false,
               false);

  clang::InitializePreprocessor(
                                ci.getPreprocessor(),
                                ci.getPreprocessorOpts(),
                                *hso,
                                ci.getFrontendOpts());

  /*------------------Add header paths-------------------------*/

  /*=+============== Creating AST-consumer, AST-context ==============+=*/
  clang::ASTConsumer *astconsumer = new MyConsumer();
  ci.setASTConsumer(astconsumer);
  ci.createASTContext();
  /*=+============== Creating AST-consumer, AST-context ==============+=*/

  /*=+============== Setting TopLevel function ====================+=*/
  topLevel_fName = string(argv[2]);
  /*=+============== Setting TopLevel function ====================+=*/

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

  /* Travere all functions_to_examine */
  MyVisitor list_visitor(&ci.getASTContext());
  while(!functions_to_examine.empty()) {
    Decl *func_decl = functions_to_examine.front();
    list_visitor.TraverseDecl(func_decl);
    functions_to_examine.pop();
  }

  /* Print file-function mapping */
  for(int i = 0; i < file_function_map.size(); i++) {
    fprintf(stderr, "%s :\n", (file_function_map[i][0]).c_str());
    for(int j = 1; j < file_function_map[i].size(); j++) {
      fprintf(stderr, "\t %s \n", (file_function_map[i][j]).c_str());
    }
  }

#if 0
  /* Get location of all listed functions */
  MyVisitor locate_visitor(&ci.getASTContext(), &ci.getSourceManager(), 0); /* Use this to locate listed functions */
  
  Decl *func_decl = topLevelDecl;
  locate_visitor.TraverseDecl(func_decl);

  vector<string>::iterator it;
  for(it = func_names.begin(); it != func_names.end(); it++) {
    assert(*it == "");
  }
#endif

  return 0;
}
