#include "entry_point.h"

namespace esphome {
namespace ld2450 {

double ENTRY_POINT_RADIUS = 50;

EntryPoint::EntryPoint(double x, double y): x_(x), y_(y) {}

bool EntryPoint::check_point(coordinates point) {
    double distance = std::sqrt(std::pow(point.x - x_, 2) + std::pow(point.y - y_, 2));
    return distance <= ENTRY_POINT_RADIUS;
}

}  // namespace ld2450
}  // namespace esphome