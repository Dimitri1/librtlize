//Dimitri Gerin 2019

#include "bind.h"

namespace vlstmt {

void bind::dump() {
  llvm::errs() << "===========+>BIND\n";
  calee_->dump();
  op_->dump();
  method_->dump();
}

// ctor
bind::bind(clang::MemberExpr const *op, clang::MemberExpr const *calee,
           clang::CXXMethodDecl const *method)
    : stmt(op, calee), method_(method) {}

// solve bind functio
bind *bind::solveBind(const clang::CXXMemberCallExpr *Call) {

  if (!Call)
    return nullptr;

  clang::MemberExpr const *op = nullptr;
  clang::MemberExpr const *calee = nullptr;
  clang::CXXMethodDecl const *method = nullptr;

  // Method Processing
  clang::CXXMethodDecl *methodDecl = Call->getMethodDecl();

  if (!methodDecl)
    return nullptr;

  // get method
  if (clang::isa<clang::FunctionDecl>(methodDecl)) {

    method = methodDecl;
    auto nd = clang::dyn_cast<clang::FunctionDecl>(methodDecl);

    auto nameinfo = nd->getNameInfo();

    if (nameinfo.getAsString() == "bind") {
      llvm::errs() << "CXXMemberCallExpr::" << nameinfo.getAsString() << "\n";
    } else
      return nullptr;
  }

  // Operand Processing (Arg0)
  auto lapo = Call->getNumArgs();
  if (!lapo)
    return nullptr;

  auto Arg0 = Call->getArg(0);
  if (Arg0) {
    // if Arg0 an Implicit Cast
    // process his subExpr
    auto op0ci = clang::dyn_cast<clang::ImplicitCastExpr>(Arg0);
    if (op0ci) {
      auto subexpr = op0ci->getSubExpr();
      clang::MemberExpr const *mExp =
          clang::dyn_cast<clang::MemberExpr>(subexpr);
      if (mExp) {
        auto nameInfo = mExp->getMemberNameInfo();
        llvm::errs() << "Arg 0  as subexp :" << nameInfo.getAsString() << "\n";
        op = mExp;
        // subexpr->dump();
      }
    }
  }

  // calee Processing (object that holds Method)
  clang::Expr *curExpr = Call->getImplicitObjectArgument();

  // case implicit Case
  if (auto implCastExp = clang::dyn_cast<clang::ImplicitCastExpr>(curExpr)) {
    auto subexpr = implCastExp->getSubExpr();
    auto asMexp = clang::dyn_cast<clang::MemberExpr>(subexpr);
    if (!asMexp)
      return nullptr;
    auto nameInfo = asMexp->getMemberNameInfo();
    calee = asMexp;
    llvm::errs() << "Calee:" << nameInfo.getAsString() << "\n";
  }

  // case direct MemberExpr
  auto asMexp = clang::dyn_cast<clang::MemberExpr>(curExpr);
  if (!asMexp)
    return nullptr;
  auto nameInfo = asMexp->getMemberNameInfo();
  calee = asMexp;
  llvm::errs() << "Calee:" << nameInfo.getAsString() << "\n";

  if (calee && op)
    return new bind(calee, op, method);

  return nullptr;
}
} // namespace vlstmt
