cmake_minimum_required(VERSION 3.22)
project(HostTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

set(APP_INC ../devboard/Gran_Ornament/App/Inc)

add_executable(test_blink test_blink.cpp)
target_include_directories(test_blink PRIVATE ${APP_INC})
add_test(NAME blink COMMAND test_blink)

add_executable(auto_off_test auto_off_test.cpp)
target_include_directories(auto_off_test PRIVATE ${APP_INC})
target_link_libraries(auto_off_test PRIVATE GTest::gtest_main)
gtest_discover_tests(auto_off_test)