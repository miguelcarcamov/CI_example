#include "fleet.hpp"

#include <sstream>

namespace ci_example {

Fleet::~Fleet() { clear(); }

void Fleet::add(Vehicle* vehicle) {
  if (vehicle != nullptr) {
    vehicles_.push_back(vehicle);
  }
}

void Fleet::clear() {
  for (Vehicle* vehicle : vehicles_) {
    delete vehicle;
  }
  vehicles_.clear();
}

std::string Fleet::describe_all() {
  std::ostringstream out;
  for (std::size_t i = 0; i < vehicles_.size(); ++i) {
    if (i > 0) {
      out << "; ";
    }
    out << vehicles_[i]->describe();
  }
  return out.str();
}

}  // namespace ci_example
