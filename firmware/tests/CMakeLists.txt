cmake_minimum_required(VERSION 3.22)
project(HostTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_executable(test_blink test_blink.cpp)
target_include_directories(test_blink PRIVATE ../devboard/Gran_Ornament/App/Inc)

add_test(NAME blink COMMAND test_blink)