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

class signal : public base {};

} // namespace qualifier

namespace decl {

class portBase {
public:
  using basePtrType = std::shared_ptr<portBase>;

  void setName(std::string name) { name_ = name; }

  void setQualifier(qualifier::base::basePtrType qual) { qual_ = qual; }

  virtual void dump() {}

protected:
  std::string name_;
  qualifier::base::basePtrType qual_;
};

class in : public portBase {
public:
  using basePtrType = std::shared_ptr<in>;

  void dump() override { // TODO : asserting
    if (qual_.get())
      llvm::errs() << name_ << " : in " << qual_.get()->getNameInfo();
  }
};

class out : public portBase {
public:
  using basePtrType = std::shared_ptr<in>;

  void dump() override { // TODO : asserting
    if (qual_.get())
      llvm::errs() << name_ << " : out " << qual_.get()->getNameInfo();
  }
};

class generic {
public:
  using basePtrType = std::shared_ptr<generic>;

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
  std::vector<builtin::base::basePtrType> builtinList;
};
} // namespace sensivity

namespace ssa {

class base {
public:
  using basePtrType = std::shared_ptr<base>;

  std::string calee;
  std::string op;
};

class bind : public base {
public:
  std::string ope = "=>";

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

  std::vector<decl::generic::basePtrType> generic;
  std::vector<decl::portBase::basePtrType> port;
};

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

  std::vector<builtin::base::basePtrType> builtinL;
  std::vector<component::basePtrType> componentL;
  body::basePtrType b;
};

class entity {
public:
  using basePtrType = std::shared_ptr<entity>;

  void make_name(std::string nameInfo);

  void make_componentItf(std::vector<vlarch::scin::scinPtrType> &in,
                         std::vector<vlarch::scout::scoutPtrType> &out);

  // interface is a component
  component::basePtrType componentIft;
  architecture::basePtrType arch;

public:
  std::string nameInfo_;
};

} // namespace architectural
} // namespace vhdl

#endif
