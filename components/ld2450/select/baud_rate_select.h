#pragma once

#include "esphome/components/select/select.h"
#include "esphome/core/preferences.h"
#include "../ld2450.h"

namespace esphome {
namespace ld2450 {

class BaudRateSelect : public select::Select, public Component, public Parented<LD2450> {
    public:
        void setup() override;
        void set_initial_value(std::string initial_value) { this->initial_value_ = initial_value; }
        void set_restore_value(bool restore_value) { this->restore_value_ = restore_value; }

    protected:
        std::string initial_value_;
        bool restore_value_{true};
        ESPPreferenceObject pref_;
        void control(const std::string &value) override;
};

}  // namespace ld2450
}  // namespace esphome