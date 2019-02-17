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
};

class in : public base {};

class out : public base {};

class signal : public base {};

} // namespace qualifier

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

  std::vector<ssa::base::basePtrType> generic;
  std::vector<ssa::base::basePtrType> port;
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
