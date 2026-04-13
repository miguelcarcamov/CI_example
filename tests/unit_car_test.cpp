#include "car.hpp"

#include <gtest/gtest.h>
#include <string>

// Tests stay at global scope so we do not nest namespaces; types are spelled
// with the ci_example:: prefix for clarity.

TEST(CarTest, DescribeIncludesMakeModelAndDoors) {
  ci_example::Car car("Acme", "Roadster", 2);
  std::string description = car.describe();
  EXPECT_NE(description.find("Acme"), std::string::npos);
  EXPECT_NE(description.find("Roadster"), std::string::npos);
  EXPECT_NE(description.find("2 doors"), std::string::npos);
}

TEST(CarTest, TypeNameIsCar) {
  ci_example::Car car("Acme", "Sedan", 4);
  EXPECT_EQ(car.type_name(), "Car");
}

TEST(CarTest, IdIsPositive) {
  ci_example::Car first("A", "1", 2);
  ci_example::Car second("B", "2", 4);
  EXPECT_GT(first.id(), 0);
  EXPECT_GT(second.id(), 0);
  EXPECT_NE(first.id(), second.id());
}

TEST(CarTest, Accessors) {
  ci_example::Car car("X", "Y", 3);
  EXPECT_EQ(car.make(), "X");
  EXPECT_EQ(car.model(), "Y");
  EXPECT_EQ(car.doors(), 3);
}
