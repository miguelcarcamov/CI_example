#include "car.hpp"
#include "fleet.hpp"

#include <gtest/gtest.h>

TEST(FleetIntegrationTest, PolymorphicDescribeAll) {
  ci_example::Fleet fleet;
  fleet.add(new ci_example::Car("Acme", "Hatch", 5));
  fleet.add(new ci_example::Car("Beta", "Coupe", 2));

  EXPECT_EQ(fleet.size(), 2u);
  EXPECT_EQ(fleet.describe_all(),
            "Acme Hatch (5 doors); Beta Coupe (2 doors)");
}

TEST(FleetIntegrationTest, InstanceCountTracksFleetLifetimes) {
  int before = ci_example::Vehicle::instance_count();
  {
    ci_example::Fleet fleet;
    fleet.add(new ci_example::Car("A", "1", 2));
    fleet.add(new ci_example::Car("B", "2", 4));
    EXPECT_EQ(ci_example::Vehicle::instance_count(), before + 2);
    fleet.clear();
    EXPECT_EQ(ci_example::Vehicle::instance_count(), before);
  }
  EXPECT_EQ(ci_example::Vehicle::instance_count(), before);
}
