#include "region_number.h"

namespace esphome {
namespace ld2450 {

RegionNumber::RegionNumber(uint8_t region) : region_(region) {}

void RegionNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_region(this->region_);
}

}  // namespace ld2450
}  // namespace esphome