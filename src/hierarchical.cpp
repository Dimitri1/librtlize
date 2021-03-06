
// Dimitri Gerin 2019

#include "hierarchical.h"
#include "bind.h"
#include "vhdl.h"

namespace vlarch {

std::string scmodule::rtlize() {

  // create target
  vhdl::architectural::entity::basePtrType entity =
      std::make_shared<vhdl::architectural::entity>();

  // name
  entity.get()->make_name(getNameInfo());

  // itf
  entity.get()->set_itf(vhdl::architectural::make_itf(scinList_, scoutList_));

  // component member
  entity.get()->getArchitecture().get()->make_componentList(scmoduleList_);

  // builtin member
  entity.get()->getArchitecture().get()->make_builtinList(scsignalList_);

  entity.get()->getArchitecture().get()->make_componentMap(
      getCtor().get()->getStmtList());

  // ctor
  // for (auto &i : getCtor().get()->getStmtList()) {
  //   i->dump();
  // }

  // processes
  std::string strEntity = entity->dump();

  return strEntity;
}

void scmethodBase::solveStmt() {}

void scctor::solveStmt(std::vector<scmodule::scmethodPtrType> &methodL,
                       std::vector<scmodule::scthreadPtrType> &threadL) {

  // TODO hardenize/log
  if (!(this_->hasBody()))
    return;

  auto body = this_->getBody();

  if (!body)
    return;

  // solve bind
  for (auto &stmtIt : body->children()) {
    auto curExpr = clang::dyn_cast<clang::CXXMemberCallExpr>(stmtIt);
    if (curExpr) {
      auto bind = vlstmt::bind::solveBind(curExpr);
      if (bind) {
        stmtList_.push_back(std::shared_ptr<vlstmt::bind>(bind));
      }
    }
  }

  // solve SCMETHOD/SCTHREAD
  std::string methodName = "";
  llvm::errs() << "Call \n";
  for (auto &stmtIt : body->children()) {
    
		
		auto sensBeg = clang::dyn_cast<clang::CXXOperatorCallExpr>(stmtIt);
		if (sensBeg)
		{
	    sensBeg->dump();
		
		}
		
		
		auto curExpr = clang::dyn_cast<clang::CompoundStmt>(stmtIt);
    llvm::errs() << "IT = \n";



    if (curExpr) {
      auto firstStmt = curExpr->child_begin();
      while (1) {
        firstStmt = firstStmt->child_begin();
        auto cexpr = clang::dyn_cast<clang::CXXMemberCallExpr>(*firstStmt);
        if (cexpr) {
          auto cexprFirst = cexpr->child_begin();
          auto itMexp = clang::dyn_cast<clang::MemberExpr>(*cexprFirst);
          if (itMexp) {
            auto name = itMexp->getMemberNameInfo().getAsString();

            if (name == "create_thread_process") {
              auto childSecond = firstStmt->child_begin();
              childSecond++;
              if (*childSecond) {
                auto strFind = childSecond->child_begin();
                auto strLit = clang::dyn_cast<clang::StringLiteral>(*strFind);
                if (strLit) {
                  llvm::errs() << "found = \n";

                  methodName = strLit->getString();
                  methodL.push_back(std::make_shared<scmethod>());
                  methodL.back()->setNameInfo(methodName);
                }
              }
              //TODO comments
              //llvm::errs() << "Found " << methodName << "As " << name << "\n";
            }
            if (name == "create_method_process") {
              auto childSecond = firstStmt->child_begin();
              childSecond++;
              if (*childSecond) {
                auto strFind = childSecond->child_begin();
                auto strLit = clang::dyn_cast<clang::StringLiteral>(*strFind);
                if (strLit) {
                  llvm::errs() << "found = \n";
                  methodName = strLit->getString();
                  methodL.push_back(std::make_shared<scmethod>());
                  methodL.back()->setNameInfo(methodName);
                }
              }
              //TODO comments
              //llvm::errs() << "Found " << methodName << "As " << name << "\n";
            }
          }
          break;
        }
        if (*firstStmt)
          continue;
        else
          break;
      }
    }
  }

  base::solveStmt();
}

bool scmodule::solveMethod() {

  for (auto i : this_->methods()) {
    std::string name = i->getNameInfo().getAsString();
    for (auto j : scmethodList_) {
      if (j->getNameInfo() == name)
        j->setClangComponent(i);
    }
    for (auto j : scthreadList_) {
      if (j->getNameInfo() == name)
        j->setClangComponent(i);
    }
  }
  return true;
}

bool scmodule::solveCtor() {

  {
    auto ctor = this_->ctor_begin();
    if (*ctor) {
      ctor->dump();
      addCtor(std::make_shared<scctor>(*ctor));
    }
  }
  {
    auto ctor = getCtor();
    //ctor statement solving has two architectural purposes
    // (1)-> solve bind call
    //       bind is as stm, so bind call are filled in scCtor stmtList member
    // (2)-> solve SC_METHOD/SC_THREAD/SC_CTHREAD scmodule method qualifier
    //       each SC_METHOD/SC_THREAD/SC_CTHREAD creates a new scMethodBase
    //       derived type object in correspondant scmodule member list
    //       scMethodList_/scthreadList_
    // (3)-> sensitive <<
    //       for each SC_METHOD/SC_THREAD
    //       a solve operation is performed to find each sensitice << call
    //       sensitive signals are filled into sensitive list of previously
    //       created SC_METHOD/SC_THREAD obj
    //       The sensitive solving operation has to be done in the same phase
    //       than (2) because sensitive << does not indicates which SC_METHOD/SC_THREAD
    //       explicitely, but implicitely by stMt position (sensitive << is after SC_METHOD/SC_THREAD)
    ctor->solveStmt(scmethodList_, scthreadList_);
  }
  return true;
}

bool scmodule::solveMember(std::vector<scmodulePtrType> &scmList) {
  solveChildAsModule(scmList);
  solveChildAsScsignal();
  solveChildAsScin();
  solveChildAsScout();
  return true;
}

// find if childs are scmodule
bool scmodule::solveChildAsModule(std::vector<scmodulePtrType> &scmList) {
  for (clang::CXXRecordDecl::field_iterator fIt = this_->field_begin();
       fIt != this_->field_end(); fIt++) {
    //(*fIt)->dump();

    std::string fItName = (*fIt)->getType().getAsString();

    for (auto &i : scmList) {
      std::string scmName = i->getNameInfo();

      // scmodule
      if (fItName == ("struct " + scmName)) {

        addScModuleChild(std::make_shared<scm>(*fIt, i));
      }

      // scmodule *
      if (fItName == ("struct " + scmName + " *")) {

        addScModuleChild(std::make_shared<scm>(*fIt, i));
      }
    }
  }
  return true;
}

// find if childs are scmodule
bool scmodule::solveChildAsScsignal() {
  for (clang::CXXRecordDecl::field_iterator fIt = this_->field_begin();
       fIt != this_->field_end(); fIt++) {
    //(*fIt)->dump();

    std::string fItName = (*fIt)->getType().getAsString();

    // scmodule
    if (fItName.find("sc_signal") == 0)
      addScSignalChild(*fIt);
  }
  return true;
}

// find if childs are scmodule
bool scmodule::solveChildAsScin() {
  for (clang::CXXRecordDecl::field_iterator fIt = this_->field_begin();
       fIt != this_->field_end(); fIt++) {

    std::string fItName = (*fIt)->getType().getAsString();

    // scmodule
    if (fItName.find("sc_in") == 0)
      addScInChild(*fIt);
  }
  return true;
}

// find if childs are scmodule
bool scmodule::solveChildAsScout() {
  for (clang::CXXRecordDecl::field_iterator fIt = this_->field_begin();
       fIt != this_->field_end(); fIt++) {
    //(*fIt)->dump();

    std::string fItName = (*fIt)->getType().getAsString();

    // scmodule
    if (fItName.find("sc_out") == 0)
      addScOutChild(*fIt);
  }
  return true;
}
} // end namespace vlarch
