#pragma once

#include "vehicle.hpp"

#include <string>
#include <vector>

namespace ci_example {

/**
 * @brief Holds vehicles as raw pointers in a vector (classic polymorphism style).
 *
 * `add` expects a pointer to a heap object (typically from `new`); the fleet
 * owns it and will `delete` it in `clear()` and in the destructor. Do not add
 * stack addresses, and do not delete a pointer after passing it to `add`.
 */
class Fleet {
public:
  Fleet() = default;

  ~Fleet();

  Fleet(const Fleet&) = delete;
  Fleet& operator=(const Fleet&) = delete;

  /**
   * @brief Appends a vehicle; nullptr is ignored.
   *
   * Ownership transfers to this fleet.
   */
  void add(Vehicle* vehicle);

  /** @brief How many vehicles are currently stored. */
  std::size_t size() { return vehicles_.size(); }

  /** @brief Deletes every vehicle and empties the fleet. */
  void clear();

  /**
   * @brief Builds one string with each vehicle's `describe()`, in order,
   * separated by `"; "`.
   */
  std::string describe_all();

private:
  std::vector<Vehicle*> vehicles_;
};

}  // namespace ci_example
