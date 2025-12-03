#include "regions_type_select.h"

namespace esphome {
namespace ld2450 {

enum RegionsTypeEnum: uint8_t {
  NOT_USED = 0,
  DETECT = 1,
  IGNORE = 2,
};

struct RegionsTypeStructure {
  const char *name;
  uint8_t redion_type;
};

static constexpr RegionsTypeStructure REGIONS_TYPE_ENUM_TO_INT[] = {
    {"Not used", NOT_USED},
    {"Detect", DETECT},
    {"Ignore", IGNORE}
};

void RegionsType::control(const std::string &value) {
  this->publish_state(value);

  uint8_t state = 0;

  for (const auto &rt : REGIONS_TYPE_ENUM_TO_INT) {
      if (strcmp(rt.name, value.c_str()) == 0)  {
          state = rt.redion_type;
          break;
      }
  }

  this->parent_->set_regions_type(state);
}

}  // namespace ld2450
}  // namespace esphome