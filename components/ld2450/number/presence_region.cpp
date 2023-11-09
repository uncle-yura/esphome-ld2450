#include "presence_region.h"

namespace esphome {
namespace ld2450 {

PresenceRegion::PresenceRegion(int16_t x_begin, int16_t y_begin, int16_t x_end, int16_t y_end)
    : x_begin_(x_begin), y_begin_(y_begin), x_end_(x_end), y_end_(y_end) {}

void PresenceRegion::check_target(coordinates target[3]) {
    bool presence = false;
    for(int i=0; i<3; i++) {
        if(x_begin_ < target[i].x && target[i].x < x_end_ && y_begin_ < target[i].y && target[i].y < y_end_) {
            presence = true;
            break;
        }
    }

#ifdef USE_BINARY_SENSOR
    if (presence_binary_sensor_ && presence_binary_sensor_->state != (presence)) {
        presence_binary_sensor_->publish_state(presence);
    }
#endif
}

}  // namespace ld2450
}  // namespace esphome