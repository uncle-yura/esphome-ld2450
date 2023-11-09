#pragma once

#include "esphome/components/number/number.h"
#include "../typedefs.h"

#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif

namespace esphome {
namespace ld2450 {

class PresenceRegion {
#ifdef USE_BINARY_SENSOR
    SUB_BINARY_SENSOR(presence)
#endif

    public:
        PresenceRegion(int16_t x_begin, int16_t y_begin, int16_t x_end, int16_t y_end);

    void check_target(coordinates target[3]);

    private:
        const int16_t x_begin_, y_begin_, x_end_, y_end_;
};

}  // namespace ld2450
}  // namespace esphome