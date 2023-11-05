#include "regions_type_select.h"

namespace esphome {
namespace ld2450 {

enum RegionsTypeStructure : uint8_t {
  NOT_USED = 0,
  DETECT = 1,
  IGNORE = 2,
};

static const std::map<std::string, uint8_t> REGIONS_TYPE_ENUM_TO_INT{
    {"Not used", NOT_USED},
    {"Detect", DETECT},
    {"Ignore", IGNORE}
};

void RegionsType::control(const std::string &value) {
  this->publish_state(value);
  this->parent_->set_regions_type(REGIONS_TYPE_ENUM_TO_INT.at(value));
}

}  // namespace ld2450
}  // namespace esphome