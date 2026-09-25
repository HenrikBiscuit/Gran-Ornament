#include "blink.hpp"
#include <cstdio>

int main()
{
    int failures = 0;

    if (!led_on(0, 1000))    { std::printf("FAIL: should be on at 0 ms\n");    ++failures; }
    if (!led_on(499, 1000))  { std::printf("FAIL: should be on at 499 ms\n");  ++failures; }
    if (led_on(500, 1000))   { std::printf("FAIL: should be off at 500 ms\n"); ++failures; }
    if (!led_on(1000, 1000)) { std::printf("FAIL: should wrap at 1000 ms\n");  ++failures; }

    std::printf(failures == 0 ? "All tests passed\n" : "Tests failed\n");
    return failures == 0 ? 0 : 1;
}