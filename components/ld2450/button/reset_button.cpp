#include "reset_button.h"

namespace esphome {
namespace ld2450 {

void ResetButton::press_action() { this->parent_->restore_factory(); }

}  // namespace ld2450
}  // namespace esphome