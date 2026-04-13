#pragma once

#include "vehicle.hpp"
#include <string>

namespace ci_example {

/**
 * @brief A simple passenger car: make, model, and door count.
 *
 * `describe()` builds a single line such as `Acme Sedan (4 doors)`.
 */
class Car final : public Vehicle {
public:
  /**
   * @param make  Manufacturer or brand name (copied into this car).
   * @param model Model name (copied into this car).
   * @param doors Number of doors (positive integer expected by callers).
   */
  Car(const std::string& make, const std::string& model, int doors);

  std::string describe() override;
  std::string type_name() override;

  /** @brief Copy of the make string (safe for callers, no internal aliasing). */
  std::string make() { return make_; }

  /** @brief Copy of the model string. */
  std::string model() { return model_; }

  /** @brief Door count stored at construction time. */
  int doors() { return doors_; }

private:
  std::string make_;
  std::string model_;
  int doors_;
};

}  // namespace ci_example
