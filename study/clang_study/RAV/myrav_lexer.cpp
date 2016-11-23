
#include <stdio.h>
#include<iostream>
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

using namespace llvm;
using namespace clang::SrcMgr;
using namespace clang;

class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
public :
  MyVisitor(ASTContext *Context) : context(Context) {}

  bool VisitDecl(Decl *D) {
    D->dumpColor();
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


  /* Create compiler instance */
  CompilerInstance ci;
  DiagnosticOptions diagnosticoptions;
  ci.createDiagnostics();

  CompilerInvocation *Invocation = new CompilerInvocation;
  LangOptions langOpts;
  langOpts.RTTI = 1;
  langOpts.Bool = 1;
  langOpts.OpenCL= 1;
  langOpts.OpenCLVersion = 1;
  langOpts.FastRelaxedMath = 1;
  langOpts.FakeAddressSpaceMap = 1;
  Invocation->setLangDefaults(langOpts,
                              clang::IK_OpenCL,
                              clang::LangStandard::lang_opencl);
  ci.setInvocation(Invocation);

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

  /* Creating AST-consumer, AST-context */
  clang::ASTConsumer *astconsumer = new MyConsumer();
  ci.setASTConsumer(astconsumer);
  ci.createASTContext();
  /* Creating AST-consumer, AST-context */

#if 0
  /* Loading an ast file */
  IntrusiveRefCntPtr<DiagnosticsEngine> dengine = &(ci.getDiagnostics());
  ASTUnit *ast_unit = ASTUnit::LoadFromASTFile("/home/varun/study/clang_study/analyse_ast_dump/dummy_ocl.ast", \
                                      dengine,
                                      FileSystemOptions(),
                                      false,
                                      None,
                                      false,
                                      true);
  assert(ast_unit != NULL);
  /* Loading an ast file */

  /* Preparing the file to be parsed */
  const FileEntry *pFile = ast_unit->getPCHFile();
  assert(pFile != NULL);
#endif

  /* Adding headers */
  llvm::IntrusiveRefCntPtr<clang::HeaderSearchOptions> hso(new clang::HeaderSearchOptions());
  HeaderSearch headersearch(
                            hso,
                            ci.getSourceManager(),
                            ci.getDiagnostics(),
                            ci.getLangOpts(),
                            pti);
  hso->AddPath("/home/varun/llvm_and_clang/llvm/tools/clang/include/clang/Basic",
               clang::frontend::Angled,
               false,
               false);

  clang::InitializePreprocessor(
                              ci.getPreprocessor(),
                              ci.getPreprocessorOpts(),
                              *hso,
                              ci.getFrontendOpts());
  /* Adding headers */

  const FileEntry *pFile = ci.getFileManager().getFile(argv[1]);
  FileID fid = ci.getSourceManager().createFileID(pFile, clang::SourceLocation(),\
                                                  (clang::SrcMgr::CharacteristicKind)C_User);

  ci.getSourceManager().setMainFileID(fid);



  ci.getPreprocessor().EnterMainSourceFile();
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());

  Token tok;
  do {
        ci.getPreprocessor().Lex(tok);
        if(ci.getDiagnostics().hasErrorOccurred()) {
          break;
        }
        ci.getPreprocessor().DumpToken(tok);
        cout<<endl;
  }while( tok.isNot(clang::tok::eof));

#if 0
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());
  clang::ParseAST(ci.getPreprocessor(), astconsumer, ci.getASTContext());
  ci.getDiagnosticClient().EndSourceFile();
#endif
  return 0;
}
