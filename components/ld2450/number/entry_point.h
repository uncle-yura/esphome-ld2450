#pragma once

#include "esphome/components/number/number.h"
#include "../typedefs.h"

namespace esphome {
namespace ld2450 {

// typedef coordinates;

class EntryPoint {
    public:
        EntryPoint(double x, double y);

        bool check_point(coordinates point);

    private:
        const double x_, y_;
};

}  // namespace ld2450
}  // namespace esphome