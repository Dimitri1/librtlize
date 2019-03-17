// Dimitri Gerin 2019

#include "vhdl.h"
#include "hierarchical.h"
#include "qualTypeBuilder.h"

// vhdl specific converter
static std::map<qt, std::string> typeMap = {{scint, "std_logic_vector"},
                                            {scuint, "std_logic_vector"},
                                            {scbool, "std_logic_vector"}};

std::string getTypeAsString(qt t, uint32_t dim) {
  std::string str = "";

  str += typeMap[t];
  str += "(";
  str += std::to_string(dim);
  str += " downto ";
  str += "0";
  str += ")";
  return str;
}

void vhdl::architectural::entity::make_name(std::string nameInfo) {
  nameInfo_ = nameInfo;
}

void vhdl::architectural::architecture::make_builtinList(
    std::vector<vlarch::scsignal::scsignalPtrType> &sL) {

  // make comonent decl for each child component of this architecture
  for (auto &i : sL) {

    auto clangComponent = i.get()->getComponent();

    std::string nameInfo = i.get()->getNameInfo();

    vhdl::decl::signal::basePtrType vhdlComponent =
        std::make_shared<decl::signal>();

    auto qual = std::make_shared<vhdl::qualifier::base>();

    // type processing
    std::string fItName = clangComponent->getType().getAsString();

    // llvm::errs() << fItName << "\n";
    qt type;
    int dim;
    std::tie(type, dim) = getParams(fItName);

    // sugar from C++ 17
    // auto [type, dim] = getParams(fItName);

    auto strType = getTypeAsString(type, dim);

    // llvm::errs () << strType  << "\n";
    // clangComponent->dump();
    qual.get()->setNameInfo(strType);

    // build qualifier
    vhdlComponent->setQualifier(qual);

    vhdlComponent->setNameInfo(nameInfo);

    //vhdlComponent->dump(); // setNameInfo(nameInfo);

    builtinL.push_back(vhdlComponent);
  }
}

vhdl::architectural::component::basePtrType
vhdl::architectural::make_itf(std::vector<vlarch::scin::scinPtrType> &in,
                              std::vector<vlarch::scout::scoutPtrType> &out) {

  component::basePtrType itf = std::make_shared<component>();
  // solve sc modules methodes
  for (auto &i : in) {
    auto clangComponent = i.get()->getComponent();

    // vhdl port
    auto vhdlComponent = std::make_shared<vhdl::decl::in>();

    // build name
    vhdlComponent->setNameInfo(i.get()->getNameInfo());

    auto qual = std::make_shared<vhdl::qualifier::base>();

    // type processing
    std::string fItName = clangComponent->getType().getAsString();

    qt type;
    int dim;
    std::tie(type, dim) = getParams(fItName);

    // sugar from C++ 17
    // auto [type, dim] = getParams(fItName);

    auto strType = getTypeAsString(type, dim);

    // clangComponent->dump();
    qual.get()->setNameInfo(strType);

    // build qualifier
    vhdlComponent->setQualifier(qual);

    itf->addPort(vhdlComponent);
  }

  // solve sc modules methodes
  for (auto &i : out) {
    auto clangComponent = i.get()->getComponent();

    // vhdl port
    auto vhdlComponent = std::make_shared<vhdl::decl::out>();

    // build name
    vhdlComponent->setNameInfo(i.get()->getNameInfo());

    auto qual = std::make_shared<vhdl::qualifier::base>();

    // type processing
    std::string fItName = clangComponent->getType().getAsString();

    qt type;
    int dim;
    std::tie(type, dim) = getParams(fItName);

    // sugar from C++ 17
    // auto [type, dim] = getParams(fItName);

    auto strType = getTypeAsString(type, dim);

    // clangComponent->dump();
    qual.get()->setNameInfo(strType);

    // build qualifier
    vhdlComponent->setQualifier(qual);

    itf->addPort(vhdlComponent);
  }

  return itf;
}
