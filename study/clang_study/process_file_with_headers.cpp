
#include<iostream>

using namespace std;

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"

#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/Utils.h"

using namespace llvm;
using namespace clang::SrcMgr;

int main() {
  using clang::CompilerInstance;
  using clang::TargetOptions;
  using clang::TargetInfo;
  using clang::DiagnosticOptions;
  using clang::TextDiagnosticPrinter;
  using clang::FileEntry;
  using clang::FileID;
  using clang::Token;
  using clang::HeaderSearch;
  using clang::HeaderSearchOptions;

  CompilerInstance ci;
  ci.createDiagnostics();

  llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
  pto->Triple = llvm::sys::getDefaultTargetTriple();
  TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(),
                                                 pto.getPtr());
  ci.setTarget(pti);
  ci.createFileManager();
  ci.createSourceManager(ci.getFileManager());
  ci.createPreprocessor(clang::TU_Complete);
  ci.getPreprocessorOpts().UsePredefines = true;

  /* Adding headers */
  llvm::IntrusiveRefCntPtr<clang::HeaderSearchOptions> hso(new clang::HeaderSearchOptions());
  HeaderSearch headersearch(
                            hso,
                            ci.getSourceManager(),
                            ci.getDiagnostics(),
                            ci.getLangOpts(),
                            pti);
  hso->AddPath("/usr/include",
               clang::frontend::Angled,
               false,
               false);
  hso->AddPath("/usr/lib/gcc/x86_64-linux-gnu/4.8/include",
               clang::frontend::Angled,
               false,
               false);
  hso->AddPath("/usr/include/x86_64-linux-gnu/",
               clang::frontend::Angled,
               false,
               false);

  clang::InitializePreprocessor(
                              ci.getPreprocessor(),
                              ci.getPreprocessorOpts(),
                              *hso,
                              ci.getFrontendOpts());

  const FileEntry *pFile = ci.getFileManager().getFile("./sample_program.cpp");
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

  return 0;
}
