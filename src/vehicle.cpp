#include "vehicle.hpp"

namespace ci_example {

// One shared copy of each static for the whole program (defined in the .cpp).

int Vehicle::next_id_ = 0;
int Vehicle::instance_count_ = 0;

Vehicle::Vehicle() {
  id_ = ++next_id_;
  ++instance_count_;
}

Vehicle::~Vehicle() {
  --instance_count_;
}

}  // namespace ci_example
