# --- project ----------------------------------------------------------
# Minimum CMake version this file is written for.
cmake_minimum_required(VERSION 3.16)

# Declares the project name and that we only use C++ (no C language).
project(ci_example LANGUAGES CXX)

# Writes compile_commands.json in the build dir (and we can symlink it to the
# repo root) so clangd / IDE know include paths without guessing.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ISO C++17 everywhere; do not fall back to older standards or GNU extensions.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- library --------------------------------------------------------
# Static (or shared, depending on platform defaults) library built from our sources.
add_library(ci_example
  src/vehicle.cpp
  src/car.cpp
  src/fleet.cpp
)

# PUBLIC: anyone who links this library also gets -I.../include automatically.
target_include_directories(ci_example PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# --- tests (optional) -----------------------------------------------
# CTest is CMake's test runner; `ctest` from the build directory runs discovered tests.
include(CTest)

# BUILD_TESTING is ON by default; turn off with -DBUILD_TESTING=OFF to skip tests.
if(BUILD_TESTING)
  # FetchContent downloads GoogleTest at configure time and adds it as a subproject.
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
  )
  # Windows / MSVC: avoid mixing CRT settings when linking gtest.
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  # Two executables so unit vs integration tests stay separate in CI output.
  add_executable(unit_tests tests/unit_car_test.cpp)
  target_link_libraries(unit_tests PRIVATE ci_example GTest::gtest_main)

  add_executable(integration_tests tests/integration_fleet_test.cpp)
  target_link_libraries(integration_tests PRIVATE ci_example GTest::gtest_main)

  # Registers each TEST() with CTest so `ctest --output-on-failure` works.
  include(GoogleTest)
  gtest_discover_tests(unit_tests)
  gtest_discover_tests(integration_tests)
endif()
