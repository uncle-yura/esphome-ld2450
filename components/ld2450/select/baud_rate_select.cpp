#include "baud_rate_select.h"

namespace esphome {
namespace ld2450 {

enum BaudRateStructure : uint8_t {
  BAUD_RATE_9600 = 1,
  BAUD_RATE_19200 = 2,
  BAUD_RATE_38400 = 3,
  BAUD_RATE_57600 = 4,
  BAUD_RATE_115200 = 5,
  BAUD_RATE_230400 = 6,
  BAUD_RATE_256000 = 7,
  BAUD_RATE_460800 = 8
};

static const std::map<std::string, uint8_t> BAUD_RATE_ENUM_TO_INT{
    {"9600", BAUD_RATE_9600},     {"19200", BAUD_RATE_19200},   {"38400", BAUD_RATE_38400},
    {"57600", BAUD_RATE_57600},   {"115200", BAUD_RATE_115200}, {"230400", BAUD_RATE_230400},
    {"256000", BAUD_RATE_256000}, {"460800", BAUD_RATE_460800}};

void BaudRateSelect::setup() {
    std::string value;
    if (!this->restore_value_) {
        value = this->initial_value_;
    } else {
        this->pref_ = global_preferences->make_preference<uint8_t>(this->get_object_id_hash(), true);
        uint8_t select_number;
        if (!this->pref_.load(&select_number)) {
          value = this->initial_value_;
        } else {
          for (auto it = BAUD_RATE_ENUM_TO_INT.rbegin(); it != BAUD_RATE_ENUM_TO_INT.rend(); ++it) {
              if (it->second == select_number) {
                  value = it->first;
                  break;
              }
          }
        }
    }

    this->publish_state(value);
}


void BaudRateSelect::control(const std::string &value) {
  this->publish_state(value);
  uint8_t state = BAUD_RATE_ENUM_TO_INT.at(value);
  this->parent_->set_baud_rate(state);
  if (this->restore_value_) this->pref_.save(&state);
}

}  // namespace ld2450
}  // namespace esphome