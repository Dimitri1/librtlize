// Dimitri Gerin 2019

#include "vhdl.h"
#include "hierarchical.h"
#include "qualTypeBuilder.h"

// vhdl specific converter
static std::map<qt, std::string>  typeMap = {{scint, "std_logic_vector"},
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

vhdl::architectural::component::basePtrType  vhdl::architectural::make_itf(
    std::vector<vlarch::scin::scinPtrType> &in,
    std::vector<vlarch::scout::scoutPtrType> &out) {

  component::basePtrType itf =  std::make_shared<component>();
  // solve sc modules methodes
  for (auto &i : in) {
    auto clangComponent = i.get()->getComponent();

    // vhdl port
    auto vhdlComponent = std::make_shared<vhdl::decl::in>();

    // build name
    vhdlComponent->setName(i.get()->getNameInfo());

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
    vhdlComponent->setName(i.get()->getNameInfo());

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
