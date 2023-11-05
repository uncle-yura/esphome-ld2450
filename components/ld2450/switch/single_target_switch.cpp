#include "single_target_switch.h"

namespace esphome {
namespace ld2450 {

void SingleTargetSwitch::write_state(bool state) {
    this->publish_state(state);
    this->parent_->set_single_target(state);
}

}  // namespace ld2450
}  // namespace esphome