#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/preferences.h"
#include "../ld2450.h"

namespace esphome {
namespace ld2450 {

class BluetoothSwitch : public switch_::Switch, public Component, public Parented<LD2450> {
    public:
        void setup() override;
        void set_initial_value(bool initial_value) { this->initial_value_ = initial_value; }
        void set_restore_value(bool restore_value) { this->restore_value_ = restore_value; }

    protected:
        float initial_value_{NAN};
        bool restore_value_{true};
        ESPPreferenceObject pref_;
        void write_state(bool state) override;
};

}  // namespace ld2450
}  // namespace esphome