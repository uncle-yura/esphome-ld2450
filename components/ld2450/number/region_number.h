#pragma once

#include "esphome/components/number/number.h"
#include "../ld2450.h"

namespace esphome {
namespace ld2450 {

class RegionNumber : public number::Number, public Parented<LD2450> {
    public:
        RegionNumber(uint8_t region);

    protected:
        uint8_t region_;
        void control(float value) override;
};

}  // namespace ld2450
}  // namespace esphome