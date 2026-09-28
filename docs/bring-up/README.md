```markdown
# 2026-09-24 · First blink

**Goal:** Program the board and blink LEDs
**Setup:** board, firmware commit, instruments
**What happened:** Time will tell
**Next:** Setting up git, then getting to it

# 2026-09-25 · First blink

**Goal:** Program the board and blink LEDs
**Setup:** board, firmware commit, instruments
**What happened:** The board blinked
**Next:** Implementing CI and Googletest

# 2026-09-25 · CI and Googletest

**Goal:** Have Github actions build project on multiple platforms and report any failures 
**Setup:** Github
**What happened:** Github was setup according to goals, and tested with fail cases
**Next:** Implementing PWM

# 2026-09-28 · PWM implementation

**Goal:** Have a dimable LED from A PWM signal
**Setup:** board, firmware commit, instruments
**What happened:** TIM3_CH1 was setup on pin PA6 (previously used for testing the LED), and is able to walk through a full duty cycle using internal timers (0-100% brigthness)
**Next:** Write useable function and setup tests for it

```
