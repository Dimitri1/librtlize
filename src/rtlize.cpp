// Dimitri Gerin 2019

#include "hierarchical.h"
#include "vhdl.h"
using namespace rtlize;

static clang::FrontendPluginRegistry::Add<rtlizeAction>
    X("rtlize", "translate systemc rtl .cpp file to synthetizable (.v,.vhd).");

std::vector<vlarch::scmodule::scmodulePtrType> scmoduleList;

class SCModuleVisitor : public clang::RecursiveASTVisitor<SCModuleVisitor> {
public:
  // find sc_module
  bool VisitCXXRecordDecl(clang::CXXRecordDecl *cxxDecl) {

    if (!(cxxDecl->hasDefinition()))
      return true;

    for (clang::CXXRecordDecl::base_class_iterator bi = cxxDecl->bases_begin(),
                                                   be = cxxDecl->bases_end();
         bi != be; ++bi) {

      clang::QualType q = bi->getType();
      std::string baseName = q.getAsString();

      if (baseName == "::sc_core::sc_module" ||
          baseName == "sc_core::sc_module" ||
          baseName == "class sc_core::sc_module") {
        // cxxDecl->dump();
        auto scm = std::make_shared<vlarch::scmodule>(cxxDecl);

        // push SC Module into scmoduleList
        scmoduleList.push_back(scm);

        return true;
      }
    }
    return true;
  }
};

void rtlizeAction::EndSourceFileAction() {
  auto &ci = getCompilerInstance();
  auto &context = ci.getASTContext();

  auto &input = getCurrentInput();
  std::string fileName = input.getFile();
  llvm::outs() << "Filename in Action: " << fileName << "\n";

  auto *unit = context.getTranslationUnitDecl();
  SCModuleVisitor scvisit;

  scvisit.TraverseDecl(unit);
  // scmodule member need to be solved after scvisit
  for (auto &i : scmoduleList)
    i->solveMember(scmoduleList);

  // solve sc modules methodes
  for (auto &i : scmoduleList)
    i->solveCtor();

  // solve sc modules methodes
  for (auto &i : scmoduleList)
    i->solveMethod();

  // for (auto &i : scmoduleList) {
  //   for (auto &j : i->getScMethodList()) {

  //     llvm::errs() << &j << j->getNameInfo() << "\n";
  //     j->dump();
  //   }
  // }

  // dump SC Module List
  // llvm::errs() << "[SC Module List]\n";
  // for (auto &i : scmoduleList)
  //    i->dump();

  std::string rtlCode = vhdl::architectural::lib;
  for (auto &i : scmoduleList)
    rtlCode += i->rtlize() + "\n";

  llvm::errs() << rtlCode;

  clang::ASTFrontendAction::EndSourceFileAction();
}
