#pragma once

#include "esphome/components/button/button.h"
#include "../ld2450.h"

namespace esphome {
namespace ld2450 {

class RebootButton : public button::Button, public Parented<LD2450> {
    public:
        RebootButton() = default;

    protected:
        void press_action() override;
};

}  // namespace ld2450
}  // namespace esphome