#include "bluetooth_switch.h"

namespace esphome {
namespace ld2450 {

void BluetoothSwitch::setup() {
    bool value;
    if (!this->restore_value_) {
        value = this->initial_value_;
    } else {
        this->pref_ = global_preferences->make_preference<float>(this->get_object_id_hash(), true);
        if (!this->pref_.load(&value)) {
            if (!std::isnan(this->initial_value_)) {
                value = this->initial_value_;
            } else {
                value = true;
            }
        }
    }

    this->publish_state(value);
}

void BluetoothSwitch::write_state(bool state) {
    this->publish_state(state);
    this->parent_->set_bluetooth(state);
    if (this->restore_value_) this->pref_.save(&state);
}

}  // namespace ld2450
}  // namespace esphome