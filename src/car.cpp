#include "car.hpp"

#include <sstream>

namespace ci_example {

// Constructor: copy parameters into the car's fields (names with a trailing _).
Car::Car(const std::string& make, const std::string& model, int doors) {
  this->make_ = make;
  this->model_ = model;
  this->doors_ = doors;
}

// One-line summary for humans and tests.
std::string Car::describe() {
  std::ostringstream line;
  line << make_ << ' ' << model_ << " (" << doors_ << " doors)";
  return line.str();
}

// Fixed label used by tests and any generic vehicle UI.
std::string Car::type_name() { return "Car"; }

}  // namespace ci_example
