# Lecture 3: PWM Control with RP2040

## Introduction to PWM (Pulse Width Modulation)

Pulse Width Modulation (PWM) is a technique for creating an analog-like signal using digital outputs. A PWM signal switches between high (on) and low (off) states at a fixed frequency, where the ratio of "on time" to "total time" (duty cycle) determines the effective analog value.

### Key PWM Concepts
- **Frequency**: Number of complete on/off cycles per second (Hz)
- **Duty Cycle**: Percentage of time the signal is high (0-100%)
- **Resolution**: Number of steps available for duty cycle control (0-65535 on RP2040)

## RP2040 PWM Hardware

The RP2040 microcontroller features:
- 8 PWM slices (numbered 0-7)
- Each slice controls two output channels (A and B)
- 16-bit resolution for duty cycle control
- Programmable frequency using system clock divider

### PWM Slice Organization
- GPIOs 0-15: Connected to PWM channels A (PWM0A-PWM7A)
- GPIOs 16-29: Connected to PWM channels B (PWM0B-PWM7B)

Example: 
- GPIO 0 and 1 share PWM slice 0 (as PWM0A and PWM0B)
- GPIO 2 and 3 share PWM slice 1 (as PWM1A and PWM1B)
- And so on...

### Frequency Selection Guide

Different applications require different PWM frequencies:

1. **LED Dimming (Human Visible)**
   - Recommended: 100Hz - 1kHz
   - Why? Above human flicker perception, but not wastefully fast
   - Example parameters:
     - 1kHz: divider = 125, wrap = 1000
     - 100Hz: divider = 125, wrap = 10000

2. **DC Motor Control**
   - Recommended: 20kHz - 50kHz
   - Why? Above audible frequency range (reduces motor whine)
   - Example parameters:
     - 25kHz: divider = 5, wrap = 1000

3. **High-Resolution Control**
   - For maximum smoothness (e.g., precise LED dimming)
   - Example: 10-bit resolution (1024 steps)
   - Trade-off between frequency and resolution

## Assignment: LED Brightness Control

Create a program that smoothly fades an LED between off and full brightness using PWM.

### Requirements

1. Configure one GPIO pin for PWM output
2. Set appropriate PWM frequency for visible LED dimming (hint: try 1kHz)
3. Implement smooth brightness transitions
4. Create an infinite loop of fading up and down

### File Structure

- `main.c`: Empty C source file (contains only main function)
- `CMakeLists.txt`: Basic CMake configuration

### Required Headers
```c
#include "hardware/pwm.h"
#include "hardware/gpio.h"
```

### CMake Configuration
Add hardware PWM support in CMakeLists.txt:
```cmake
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pwm
)
```

## SDK Functions and Implementation Guide

### 1. PWM Slice Management
```c
uint slice_num = pwm_gpio_to_slice_num(gpio);
uint channel = pwm_gpio_to_channel(gpio);
```
These functions convert a GPIO pin number to its corresponding PWM slice and channel. Essential for initializing the correct PWM hardware.

Documentation:
- [`pwm_gpio_to_slice_num()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1ga8b2117d67a544f64c001f4f32c723eef)
- [`pwm_gpio_to_channel()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1ga96ba504fe00cb8e0d88f0730cdfdaafc)

### 2. GPIO Configuration
```c
gpio_set_function(gpio, GPIO_FUNC_PWM);
```
Configures a GPIO pin for PWM functionality instead of regular digital I/O.

Documentation:
- [`gpio_set_function()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_gpio_1ga22f6b90c4c7e0013b4b5341de0a44fc4)

### 3. PWM Configuration Functions
Essential functions for setting up PWM:

- [`pwm_get_default_config()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1gad76f94a6dd6133c52284b6be8ae98e16)
  - Creates configuration structure with safe default values

- [`pwm_config_set_clkdiv()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1ga679802450ac829eff368e8f109df387e)
  - Sets PWM clock divider
  - Divides system clock (125MHz)
  - Affects final PWM frequency

- [`pwm_config_set_wrap()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1gad6cf6d9237144234732a50eb6d5e4fe9)
  - Sets maximum count value
  - Affects resolution and frequency
  - Maximum value is 65535

- [`pwm_init()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1ga01d3e90bb7aa9a8b6884eddbfbebc175)
  - Initializes PWM with configuration

### 4. PWM Control
```c
pwm_set_gpio_level(gpio, level);
```
Sets PWM duty cycle (0 to wrap value).

Documentation:
- [`pwm_set_gpio_level()`](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1ga279d1ba7dcc8f19619f389317efb41fd)

## Practical Implementation Tips

1. **Frequency Calculation**
   ```
   PWM Frequency = System Clock / (Wrap × Clock Divider)
   ```
   Example: For 1kHz with divider=125 and wrap=1000:
   ```
   1kHz = 125MHz / (1000 × 125)
   ```

2. **Resolution vs Frequency Trade-off**
   - Higher wrap values = more brightness levels
   - But also means lower maximum frequency
   - Find the balance for your application

3. **Smooth Fading**
   - Use small increments for level changes
   - Add small delays between changes
   - Consider using wrap value for maximum resolution

## Extra Challenges

1. Add button control to:
   - Start/stop the fade effect
   - Change fade speed
   - Switch between different patterns

2. Create different fade patterns:
   - "Breathing" effect
   - Stepped brightness levels
   - Random brightness changes

### Resources

- [Raspberry Pi Pico C/C++ SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)
- [Getting started with Raspberry Pi Pico-series](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- [Pico-sdk datasheet](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Pico Examples](https://github.com/raspberrypi/pico-examples)

- PWM
  - [Pico SDK PWM Examples](https://github.com/raspberrypi/pico-examples/tree/master/pwm)
  - [Hardware PWM API](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#hardware_pwm)

## Building the Project

Refer to the `README.md` file located in the root of this repository for detailed build instructions.

## Flashing the Firmware

1. Connect the RP2040 board to your computer.
2. Press and hold the BOOTSEL button while plugging the board into USB.
3. Copy the generated `.uf2` file from the `build` directory to the RP2040 USB mass storage device.