// Dimitri Gerin 2019

#ifndef __VHDL__
#define __VHDL__

#include "hierarchical.h"
#include <memory>
#include <vector>

// restricted vhdl gramar class hierarchy

namespace vhdl {
namespace select {

class base {
  using basePtrType = std::shared_ptr<base>;
};

class range : public base {
public:
  std::string beg;
  std::string end;
};

} // namespace select

// namespace select
namespace qualifier {

class base {
public:
  using basePtrType = std::shared_ptr<base>;
  std::string name_;

  void setNameInfo(std::string name) { name_ = name; }

  std::string getNameInfo() { return name_; }
};

} // namespace qualifier

namespace decl {

class portBase {
public:
  using basePtrType = std::shared_ptr<portBase>;

  void setNameInfo(std::string n) { nameInfo_ = n; }

  void setQualifier(qualifier::base::basePtrType qual) { qual_ = qual; }

  virtual std::string dump() { return ""; }

  virtual ~portBase() {}

protected:
  std::string nameInfo_;
  qualifier::base::basePtrType qual_;
};

class signal : public portBase {
public:
  using basePtrType = std::shared_ptr<portBase>;
  std::string dump() override { // TODO : asserting
    if (qual_.get())
      return "signal " + nameInfo_ + " : " + qual_.get()->getNameInfo();

    return "";
  }
};

class in : public portBase {
public:
  using basePtrType = std::shared_ptr<in>;

  std::string dump() override { // TODO : asserting
    if (qual_.get())
      return nameInfo_ + " : in " + qual_.get()->getNameInfo();

    return "";
  }
};

class out : public portBase {
public:
  using basePtrType = std::shared_ptr<out>;

  std::string dump() override { // TODO : asserting
    if (qual_.get())
      return nameInfo_ + " : out " + qual_.get()->getNameInfo();

    return "";
  }
};

class generic : public portBase {
public:
  using basePtrType = std::shared_ptr<generic>;

  std::string dump() override { // TODO : asserting
    if (qual_.get())
      return name_ + " : " + qual_.get()->getNameInfo();

    return "";
  }

  std::string name_;
  qualifier::base::basePtrType qualifier;
};
} // namespace decl

namespace builtin {

class base {
public:
  using basePtrType = std::shared_ptr<base>;

  qualifier::base::basePtrType qualifier;
};

class natural {
public:
};

class integer {
public:
};

class logicvector {
public:
  select::range range;
};

} // namespace builtin

namespace sensivity {
class base {
public:
  std::vector<decl::portBase::basePtrType> builtinList;
};
} // namespace sensivity

namespace ssa {

class base {
public:

   virtual  std::string dump(){

      return  "";
  }

  using basePtrType = std::shared_ptr<base>;
  std::string calee;
  std::string op;
  std::string ope;
};

class bind : public base {
public:

  std::string dump() override {

    return op + "=>" + calee ;

  }

  bind() {}
};

} // namespace ssa

namespace architectural {

namespace func {

class base {
public:
  using basePtrType = std::shared_ptr<base>;
};
class process : public base {};

class function : public base {};

} // namespace func

class component {
public:
  using basePtrType = std::shared_ptr<component>;

  using scmPtrType = vlarch::scmodule::scmPtrType;

  void addPort(decl::portBase::basePtrType p) { port.push_back(p); }

  void addGeneric(decl::generic::basePtrType p) { generic.push_back(p); }

  void setNameInfo(std::string n) { nameInfo_ = n; }

  std::string getNameInfo() { return nameInfo_; }

  std::string nameInfo_;

  vlarch::scmodule::scmodulePtrType  getScmDecl() { return scm_->getDecl(); }
  scmPtrType getScmField() { return scm_; }

  void setScmField( scmPtrType scf) {  scm_ = scf; }


  scmPtrType scm_;

  std::vector<decl::generic::basePtrType> generic;
  std::vector<decl::portBase::basePtrType> port;
};

vhdl::architectural::component::basePtrType
make_itf(std::vector<vlarch::scin::scinPtrType> &in,
         std::vector<vlarch::scout::scoutPtrType> &out);

class body {
public:
  using basePtrType = std::shared_ptr<body>;
  std::vector<component::basePtrType> mapL;
  std::vector<func::base::basePtrType> funcL;
  std::vector<func::base::basePtrType> processL;
  std::vector<ssa::base::basePtrType> ssaL;
};

class architecture {
public:
  using basePtrType = std::shared_ptr<architecture>;

  std::vector<decl::portBase::basePtrType> builtinL;
  std::vector<component::basePtrType> componentL;
  std::vector<ssa::base::basePtrType> bindL;

  body::basePtrType b;

  //todo use ref to avoid vector copy
  void make_bindList( std::vector<vlstmt::stmt::stmtPtrType> sL){

    for (auto &j : sL) {


      auto componentBuild = std::make_shared<ssa::bind>();
      std::string op = j.get()->getOp()->getMemberNameInfo().getAsString();
      std::string calee = j.get()->getCalee()->getMemberNameInfo().getAsString();

      componentBuild.get()->op = op;
      componentBuild.get()->calee = calee;


        /* std::string */

      /* std::string nameInfo = j->getNameInfo(); */

      /* auto scinList = j->getScinList(); */
      /* auto scoutList = j->getScoutList(); */

      /* vhdl::architectural::component::basePtrType componentBuild = */
      /*     vhdl::architectural::make_itf(scinList, scoutList); */
      /* componentBuild->setNameInfo(nameInfo); */
      //componentL.push_back(componentBuild);

      bindL.push_back(componentBuild);
    }
  }

  void make_builtinList(std::vector<vlarch::scsignal::scsignalPtrType> &sL);

  void make_componentList(std::vector<vlarch::scmodule::scmPtrType> &mL) {

    // make comonent decl for each child component of this architecture
    for (auto &j : mL) {

      std::string nameInfo = j->getDecl()->getNameInfo();

      auto scinList = j->getDecl()->getScinList();
      auto scoutList = j->getDecl()->getScoutList();

      vhdl::architectural::component::basePtrType componentBuild =
        vhdl::architectural::make_itf(scinList, scoutList);
      componentBuild->setNameInfo(nameInfo);

      componentBuild->setScmField(j);

      //is Field already present in the list, break
      bool findIfField = false;
      for(auto & i :componentL){

        /* llvm::errs() << (i->getScmField()->getDecl()) << "=\n"; */
        /* llvm::errs() << ( j->getDecl()) << "\n"; */


        //llvm::errs() <<  (i->getScmField()) <<  "\n";

        findIfField = (i->getScmField()->getDecl() == j->getDecl());
        if(findIfField)
          break;
      }

      if (!findIfField)
        componentL.push_back(componentBuild);

    }
  }

  std::string dumpField(){
    return  "";//scm_->getNameInfo();
  }

  std::string dump() {
    std::string archStr = "";

    std::string headerStr = "";

    for (auto &componentBuild : componentL) {
      std::string componentGenerics = "";
      std::string componentPorts = "";
      for (auto &i : componentBuild.get()->port) {
        componentPorts += i->dump() + ";\n";
      }

      for (auto &i : componentBuild.get()->generic) {
        componentGenerics += i->dump() + ";\n";
      }

      std::string decl =
          "component " + componentBuild.get()->getNameInfo() + " \n" +

          "generic(\n" + componentGenerics + ");\n" +

          "port(\n" + componentPorts + ");\n" + "end component;\n";

      headerStr += decl;
    }

    std::string builtInStrL = "";
    for (auto &componentBuild : builtinL) {
      builtInStrL += componentBuild->dump() + ";\n";
    }
    headerStr += builtInStrL;

    std::string bodyStr = "begin\n";

    for (auto &componentBuild : bindL) {
      bodyStr += componentBuild->dump() + ";\n";
    }

    return archStr = headerStr + bodyStr;
  }
};

static std::string lib = "library ieee;\nuse ieee.std_logic_1164.all;\nuse "
                         "ieee.numeric_std.all;\n";
class entity {
public:
  using basePtrType = std::shared_ptr<entity>;

  void make_name(std::string nameInfo);

  void set_itf(component::basePtrType itf) { componentIft = itf; }

  entity()
      : componentIft(std::make_shared<component>()),
        arch(std::make_shared<architecture>()) {}

  // interface is a component
  component::basePtrType componentIft;
  architecture::basePtrType arch;

  architecture::basePtrType getArchitecture() { return arch; }

  std::string dump() {
    std::string str;

    std::string componentGenerics = "";
    std::string componentPorts = "";

    for (auto &i : componentIft->port) {
      componentPorts += i->dump() + ",\n";
    }

    for (auto &i : componentIft->generic) {
      componentGenerics += i->dump() + ",\n";
    }

    std::string decl = "entity " + nameInfo_ + " is\n" +

                       "generic(\n" + componentGenerics + ");\n" +

                       "port(\n" + componentPorts + ");\n" + "end " +
                       nameInfo_ + ";\n";

    std::string archStr = "architecture arch of " + nameInfo_ + " is\n" +
                          arch->dump() + "\n" + "end arch ;\n";

    str = decl + archStr;

    return str;
  }

public:
  std::string nameInfo_;
};

} // namespace architectural
} // namespace vhdl

#endif
