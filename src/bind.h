//Dimitri Gerin 2019

#ifndef __BIND_H__
#define __BIND_H__

#include "rtlize.h"
#include "stmt.h"
#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/ADT/StringRef.h"
#include <clang/Frontend/FrontendPluginRegistry.h>

namespace vlstmt {

class bind : public stmt {
public:
  using bindPtrType = std::shared_ptr<bind>;

  // solve bind function
  static bind *solveBind(const clang::CXXMemberCallExpr *Call);
  bind(clang::MemberExpr const *op, clang::MemberExpr const *calee,
       clang::CXXMethodDecl const *method);

  void dump() override;

protected:
  clang::CXXMethodDecl const *method_ = nullptr;
};

} // namespace vlstmt

#endif
