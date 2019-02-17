#include "vhdl.h"
#include "hierarchical.h"

void vhdl::architectural::entity::make_name(std::string nameInfo) {
  nameInfo_ = nameInfo;
}
void vhdl::architectural::entity::make_componentItf(
    std::vector<vlarch::scin::scinPtrType> &in,
    std::vector<vlarch::scout::scoutPtrType> &out) {}
