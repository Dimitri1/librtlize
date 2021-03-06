// Dimitri Gerin 2019

#ifndef __ARCHITECTURAL__
#define __ARCHITECTURAL__
#include "bind.h"
#include "rtlize.h"
#include "clang/AST/AST.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/ADT/StringRef.h"
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <memory>
#include <vector>

#define PRINT_DBG(COL, a) llvm::errs() << COL << a << COLOR_END;

static const std::string COLOR_RED("\033[31m");
static const std::string COLOR_GREEN("\033[32m");
static const std::string COLOR_YELLOW("\033[33m");
static const std::string COLOR_BLUE("\033[36m");
static const std::string COLOR_MAGENTA("\033[35m");
static const std::string COLOR_END("\033[0m");

namespace vlarch {

class scsignal {
public:
  using scsignalPtrType = std::shared_ptr<scsignal>;

  scsignal(clang::FieldDecl *field) { this_ = field; }

  std::string getNameInfo() { return this_->getNameAsString(); }

  clang::FieldDecl *getComponent() { return this_; }

protected:
  clang::FieldDecl *this_ = nullptr;
};

class scin {
public:
  using scinPtrType = std::shared_ptr<scin>;

  scin(clang::FieldDecl *field) { this_ = field; }

  std::string getNameInfo() { return this_->getNameAsString(); }

  clang::FieldDecl *getComponent() { return this_; }

protected:
  clang::FieldDecl *this_ = nullptr;
};

class scout {
public:
  using scoutPtrType = std::shared_ptr<scout>;

  scout(clang::FieldDecl *field) { this_ = field; }

  std::string getNameInfo() { return this_->getNameAsString(); }

  clang::FieldDecl *getComponent() { return this_; }

protected:
  clang::FieldDecl *this_ = nullptr;
};

class scmethodBase {
public:
  using scmethodBasePtrType = std::shared_ptr<scmethodBase>;
  using base = scmethodBase;

  virtual void dump() {
    for (auto &i : stmtList_) {
      i->dump();
    }
  }

  std::string getNameInfo() { return name_; }
  void setNameInfo(std::string name) { name_ = name; }

  void setClangComponent(clang::CXXMethodDecl *c) { this_ = c; }

  scmethodBase(clang::CXXMethodDecl *cxxNode) { this_ = cxxNode; }

  virtual void solveStmt();

  std::vector<vlstmt::stmt::stmtPtrType> getStmtList() { return stmtList_; }

protected:
  std::string name_ = "";
  clang::CXXMethodDecl *this_ = nullptr;
  std::vector<vlstmt::stmt::stmtPtrType> stmtList_;
};

class scmethod : public scmethodBase {
public:
  using scmethodPtrType = scmethodBase::scmethodBasePtrType;

public:
  scmethod(clang::CXXMethodDecl *cxxNode = nullptr) : scmethodBase(cxxNode) {}
};

class scthread : public scmethodBase {
public:
  using scthreadPtrType = scmethodBase::scmethodBasePtrType;

public:
  scthread(clang::CXXMethodDecl *cxxNode = nullptr) : scmethodBase(cxxNode) {}
};

class scctor : public scmethodBase {
public:
  // contains specific methods
  using scctorPtrType = std::shared_ptr<scctor>;

  void solveStmt(std::vector<scmethod::scmethodPtrType> &methodL,
                 std::vector<scthread::scthreadPtrType> &threadL);

  void dump() override { base::dump(); }

public:
  scctor(clang::CXXMethodDecl *cxxNode) : scmethodBase(cxxNode) {}
};

class scm;

class scmodule {
public:
  using scmPtrType = std::shared_ptr<scm>;
  using scmodulePtrType = std::shared_ptr<scmodule>;
  using scctorPtrType = scctor::scctorPtrType;
  using scmethodPtrType = scmethod::scmethodPtrType;
  using scthreadPtrType = scthread::scthreadPtrType;

  scmodule(clang::CXXRecordDecl *cxxNode) { this_ = cxxNode; }

  std::string rtlize();

  bool solveCtor();

  void addCtor(scctorPtrType ctor) { ctor_ = ctor; }

  scctorPtrType getCtor() { return ctor_; }

  void addScModuleChild(scmPtrType scm) { scmoduleList_.push_back(scm); }

  void addScSignalChild(clang::FieldDecl *field) {
    scsignalList_.push_back(std::make_shared<scsignal>(field));
  }

  void addScInChild(clang::FieldDecl *field) {
    scinList_.push_back(std::make_shared<scin>(field));
  }
  void addScOutChild(clang::FieldDecl *field) {
    scoutList_.push_back(std::make_shared<scout>(field));
  }

  bool solveMethod();

  bool solveMember(std::vector<scmodulePtrType> &scmList);

  // find if childs are scmodule
  bool solveChildAsModule(std::vector<scmodulePtrType> &scmList);

  // find if childs are scmodule
  bool solveChildAsScsignal();

  // find if childs are scmodule
  bool solveChildAsScin();

  // find if childs are scmodule
  bool solveChildAsScout();

  std::string getNameInfo() { return this_->getNameAsString(); }

  void dump() {
    PRINT_DBG(COLOR_GREEN, *this_ << "\n");

    PRINT_DBG(COLOR_MAGENTA, "ctor()");
    ctor_->dump();

    /* for (auto &i : scmoduleList_) { */
    /*   PRINT_DBG(COLOR_MAGENTA, "|_"); */
    /*   //      PRINT_DBG(COLOR_MAGENTA, i->getNameInfo() << "\n"); */
    /* } */

    for (auto &i : scsignalList_) {
      PRINT_DBG(COLOR_RED, "|_");
      PRINT_DBG(COLOR_RED, "signal " << i->getNameInfo() << "\n");
    }
    for (auto &i : scinList_) {
      PRINT_DBG(COLOR_RED, "|_");
      PRINT_DBG(COLOR_RED, "scin " << i->getNameInfo() << "\n");
    }
    for (auto &i : scoutList_) {
      PRINT_DBG(COLOR_RED, "|_");
      PRINT_DBG(COLOR_RED, "scout " << i->getNameInfo() << "\n");
    }
  }

  auto getThis() { return this_; }

  std::vector<scin::scinPtrType> getScinList() { return scinList_; }
  std::vector<scout::scoutPtrType> getScoutList() { return scoutList_; }

  std::vector<scmethodPtrType> getScMethodList() { return scmethodList_; }
  std::vector<scthreadPtrType> getScThreadList() { return scthreadList_; }

protected:
  std::vector<scmethodPtrType> scmethodList_;
  std::vector<scthreadPtrType> scthreadList_;

  scctorPtrType ctor_;
  std::vector<scmPtrType> scmoduleList_;
  std::vector<scsignal::scsignalPtrType> scsignalList_;
  std::vector<scin::scinPtrType> scinList_;
  std::vector<scout::scoutPtrType> scoutList_;

  clang::CXXRecordDecl *this_ = nullptr;
};

class scm {
public:
  scm(clang::FieldDecl *field, scmodule::scmodulePtrType scm) {
    this_ = field;
    scmDecl_ = scm;
  }

  std::string getNameInfo() { return this_->getNameAsString(); }

  clang::FieldDecl *getThis() { return this_; }

  scmodule::scmodulePtrType getDecl() { return scmDecl_; }

protected:
  clang::FieldDecl *this_ = nullptr;
  scmodule::scmodulePtrType scmDecl_;
};

} // end namespace vlarch

#endif
