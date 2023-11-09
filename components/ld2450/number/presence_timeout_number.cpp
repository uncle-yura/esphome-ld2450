#include "presence_timeout_number.h"

namespace esphome {
namespace ld2450 {

void PresenceTimeoutNumber::setup() {
    float value;
    if (!this->restore_value_) {
        value = this->initial_value_;
    } else {
        this->pref_ = global_preferences->make_preference<float>(this->get_object_id_hash(), true);
        if (!this->pref_.load(&value)) {
            if (!std::isnan(this->initial_value_)) {
                value = this->initial_value_;
            } else {
                value = this->traits.get_min_value();
            }
        }
    }

    this->publish_state(value);
    this->parent_->set_presence_timeout_number();
}

void PresenceTimeoutNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_presence_timeout_number();
    if (this->restore_value_) this->pref_.save(&value);
}

}  // namespace ld2450
}  // namespace esphome