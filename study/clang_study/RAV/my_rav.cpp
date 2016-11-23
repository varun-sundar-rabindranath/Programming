#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include <iostream>

using namespace std;
using namespace clang;

class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
public :
  MyVisitor(ASTContext *Context) : context(Context) {}

  bool VisitDecl(Decl *D) {
    D->dumpColor();
    return false;
  }

private :
  ASTContext *context;
};

class MyConsumer : public ASTConsumer {
public :
  MyConsumer(ASTContext *context) : visitor_object(context) {}
  virtual void HandleTranslationUnit(ASTContext &Context) {
    visitor_object.TraverseDecl(Context.getTranslationUnitDecl());
  }
private :
  MyVisitor visitor_object;
};

class MyAction : public ASTFrontendAction {
public :
  virtual ASTConsumer *CreateASTConsumer(
			CompilerInstance &compiler,
			llvm::StringRef InFile) {
   using clang::CompilerInvocation;
   using clang::LangOptions;
   CompilerInvocation *Invocation = new CompilerInvocation;
   compiler.setInvocation(Invocation);
   LangOptions langOpts;
   langOpts.RTTI = 1;
   langOpts.Bool = 1;
   langOpts.OpenCL= 1;
   Invocation->setLangDefaults(langOpts,
                               clang::IK_OpenCL,
                               clang::LangStandard::lang_opencl);
    return new MyConsumer(&compiler.getASTContext());

  }
};

int main(int argc, char *argv[1]) {
  tooling::runToolOnCode(new MyAction, argv[1]);
  return 0;
}


