
#include<iostream>
using namespace std;

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/HeaderSearchOptions.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Parse/Parser.h"

#include "clang/Basic/SourceManagerInternals.h"

using namespace llvm;
using namespace clang::SrcMgr;

/* Our astconsumer */
class MyASTConsumer : public clang::ASTConsumer {
public :
  MyASTConsumer() : clang::ASTConsumer() {}
  virtual ~MyASTConsumer() {}

  virtual bool HandleTopLevelDecl(clang::DeclGroupRef d) {
    cout<<" Handling top level decl "<<endl;
    clang::DeclGroupRef::iterator it;
    for(it = d.begin(); it != d.end(); it++) {
      clang::VarDecl *vd = llvm::dyn_cast<clang::VarDecl>(*it);
      if(!vd) { continue; }

      cout<<vd->getDeclName().getAsString();
      cout<<endl;
    }
    return true;
  }
};


int main() {
  using clang::CompilerInstance;
  using clang::TargetOptions;
  using clang::TargetInfo;
  using clang::DiagnosticOptions;
  using clang::TextDiagnosticPrinter;
  using clang::FileEntry;
  using clang::FileID;
  using clang::Token;


  /* Create compiler instance */
  CompilerInstance ci;
  DiagnosticOptions diagnosticoptions;
  ci.createDiagnostics();

  /*------------------Preprocessor creation--------------------*/
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
  /*------------------Preprocessor creation--------------------*/

  /*------------------Add header paths-------------------------*/
  llvm::IntrusiveRefCntPtr<clang::HeaderSearchOptions> hso = \
                                      new clang::HeaderSearchOptions();

  clang::HeaderSearch headersearch(
                                    hso,
                                    ci.getSourceManager(),
                                    ci.getDiagnostics(),
                                    ci.getLangOpts(),
                                    pti);

  hso->AddPath("/usr/include",
               clang::frontend::Angled,
               false,
               false);

  hso->AddPath("/usr/include/x86_64-linux-gnu",
               clang::frontend::Angled,
               false,
               false);

  hso->AddPath("/usr/lib/gcc/x86_64-linux-gnu/4.8/include",
               clang::frontend::Angled,
               false,
               false);

  clang::InitializePreprocessor(
                                ci.getPreprocessor(),
                                ci.getPreprocessorOpts(),
                                *hso,
                                ci.getFrontendOpts());

  /*------------------Add header paths-------------------------*/

  /* Creating AST-consumer, AST-context */
  clang::ASTConsumer *astconsumer = new MyASTConsumer();
  ci.setASTConsumer(astconsumer);
  ci.createASTContext();
  /* Creating AST-consumer, AST-context */

  /* Preparing the file to be parsed */
  const FileEntry *pFile = ci.getFileManager().getFile("./sample_program.c");
  FileID fid = ci.getSourceManager().createFileID(pFile, clang::SourceLocation(),\
                                                  (clang::SrcMgr::CharacteristicKind)C_User);

  ci.getSourceManager().setMainFileID(fid);

  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());
  clang::ParseAST(ci.getPreprocessor(), astconsumer, ci.getASTContext());
  ci.getDiagnosticClient().EndSourceFile();

  return 0;
}
