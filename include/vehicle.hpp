#pragma once

#include <string>

namespace ci_example {

/**
 * @brief Base type for every vehicle in the example domain.
 *
 * You cannot construct a `Vehicle` directly because it is abstract: derived
 * classes must implement how a vehicle describes itself and what its type
 * label is.
 *
 * This class also keeps two pieces of shared (static) state:
 * - the next id handed out to a new vehicle
 * - how many vehicle objects are currently alive in the program
 *
 * The live count is what makes the `Fleet` integration tests easy to read: you
 * can assert that constructing cars changes the count and that clearing the
 * fleet puts the count back.
 */
class Vehicle {
public:
  /**
   * @brief Releases this vehicle and updates the global live-instance count.
   */
  virtual ~Vehicle();

  /**
   * @brief Human-readable summary (make/model, capacity, etc.).
   *
   * Each concrete vehicle decides what goes into the string.
   */
  virtual std::string describe() = 0;

  /**
   * @brief Short type tag, for example `"Car"`.
   */
  virtual std::string type_name() = 0;

  /**
   * @brief Stable integer id assigned when the object is constructed.
   *
   * Ids increase over time for every new vehicle, which helps tests tell two
   * instances apart.
   */
  int id() { return id_; }

  /**
   * @brief How many `Vehicle` objects are alive (updated in ctor/dtor).
   *
   * Implemented with a plain `int` for readability.
   */
  static int instance_count() { return instance_count_; }

protected:
  /**
   * @brief Called only from concrete subclasses; assigns a fresh id and bumps
   * the live-instance counter.
   */
  Vehicle();

  // Vehicles are not meant to be copied or moved in this teaching example:
  // a "copy" would duplicate identity and confuse the id / instance tracking.
  Vehicle(const Vehicle&) = delete;
  Vehicle& operator=(const Vehicle&) = delete;
  Vehicle(Vehicle&&) = delete;
  Vehicle& operator=(Vehicle&&) = delete;

private:
  int id_;
  static int next_id_;
  static int instance_count_;
};

}  // namespace ci_example
