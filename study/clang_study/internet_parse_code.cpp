/*** CItutorial4.cpp *****************************************************
* This code is licensed under the New BSD license.
* See LICENSE.txt for details.
*
* The CI tutorials remake the original tutorials but using the
* CompilerInstance object which has as one of its purpose to create commonly
* used Clang types.
*****************************************************************************/
#include <iostream>

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"

/******************************************************************************
*
*****************************************************************************/
using namespace clang::SrcMgr;
int main()
{
    using clang::CompilerInstance;
    using clang::TargetOptions;
    using clang::TargetInfo;
    using clang::FileEntry;
    using clang::Token;
    using clang::ASTContext;
    using clang::ASTConsumer;
    using clang::Parser;
    using clang::DiagnosticOptions;
    using clang::TextDiagnosticPrinter;
    using clang::FileID;

    CompilerInstance ci;
    DiagnosticOptions diagnosticOptions;
    ci.createDiagnostics();

    llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto.getPtr());
    ci.setTarget(pti);

    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());
    ci.createPreprocessor(clang::TU_Complete);
    ci.getPreprocessorOpts().UsePredefines = false;
    ASTConsumer *astConsumer = new ASTConsumer();
    ci.setASTConsumer(astConsumer);

    ci.createASTContext();
    ci.createSema(clang::TU_Complete, NULL);

    const FileEntry *pFile = ci.getFileManager().getFile("/home/varun/study/clang_study/sample_program.c");
    FileID fid = ci.getSourceManager().createFileID(pFile, clang::SourceLocation(),\
                                                  (clang::SrcMgr::CharacteristicKind)C_User);

    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());
    ci.getSourceManager().setMainFileID(fid);
    clang::ParseAST(ci.getSema());
    ci.getASTContext().Idents.PrintStats();

    return 0;
}
