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

add_executable(pwm_test pwm_test.cpp)
target_include_directories(pwm_test PRIVATE ${APP_INC})
target_link_libraries(pwm_test PRIVATE GTest::gtest_main)
gtest_discover_tests(pwm_test)

add_executable(button_test button_test.cpp)
target_include_directories(button_test PRIVATE ${APP_INC})
target_link_libraries(button_test PRIVATE GTest::gtest_main)
gtest_discover_tests(button_test)

add_executable(modes_test modes_test.cpp)
target_include_directories(modes_test PRIVATE ${APP_INC})
target_link_libraries(modes_test PRIVATE GTest::gtest_main)
gtest_discover_tests(modes_test)

add_executable(candle_test candle_test.cpp)
target_include_directories(candle_test PRIVATE ${APP_INC})
target_link_libraries(candle_test PRIVATE GTest::gtest_main)
gtest_discover_tests(candle_test)

add_executable(battery_test battery_test.cpp)
target_include_directories(battery_test PRIVATE ${APP_INC})
target_link_libraries(battery_test PRIVATE GTest::gtest_main)
gtest_discover_tests(battery_test)

add_executable(fade_test fade_test.cpp)
target_include_directories(fade_test PRIVATE ${APP_INC})
target_link_libraries(fade_test PRIVATE GTest::gtest_main)
gtest_discover_tests(fade_test)
