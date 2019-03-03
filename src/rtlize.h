// Dimitri Gerin 2019

#ifndef PRINT_FUNCTIONS_ACTION_H
#define PRINT_FUNCTIONS_ACTION_H

#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

namespace rtlize {

class rtlizeAction : public clang::PluginASTAction {
public:
  rtlizeAction() {}

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef) override {
    ci.getDiagnostics().setClient(new clang::IgnoringDiagConsumer());
    return llvm::make_unique<clang::ASTConsumer>();
  }

  bool ParseArgs(const clang::CompilerInstance &ci,
                 const std::vector<std::string> &args) override {
    return true;
  }

protected:
  void EndSourceFileAction() override;

  clang::PluginASTAction::ActionType getActionType() override {
    return ReplaceAction;
  }
};

} // namespace rtlize

#endif
