//Dimitri Gerin 2019

#ifndef __STMT__
#define __STMT__
#include "clang/AST/AST.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/ADT/StringRef.h"
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <memory>
#include <vector>

namespace vlstmt {

class stmt {
public:
  using stmtPtrType = std::shared_ptr<stmt>;

  stmt(clang::MemberExpr const *op, clang::MemberExpr const *calee) {
    op_ = op;
    calee_ = calee;
  }

  virtual void dump() {}

protected:
  clang::MemberExpr const *op_ = nullptr;
  clang::MemberExpr const *calee_ = nullptr;
};

class comoundStmt : public stmt {};

class ifStmt : public stmt {
  comoundStmt cond;
  comoundStmt body;
};

} // end namespace vlstmt
#endif
