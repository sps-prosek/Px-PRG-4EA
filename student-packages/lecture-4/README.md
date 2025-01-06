# LECTURE-4: ADC Control with RP2040

## Introduction to ADC (Analog-to-Digital Conversion)

Analog-to-Digital Conversion (ADC) is a technique for converting continuous analog signals into discrete digital values. The RP2040's ADC can measure voltage levels between 0V and 3.3V and convert them into digital values ranging from 0 to 4095 (12-bit resolution).

### Key ADC Concepts
- **Resolution**: 12-bit (4096 distinct levels)
- **Input Range**: 0V to 3.3V
- **Sampling Rate**: Up to 500kS/s (kilosamples per second)
- **Conversion Formula**: Digital Value = (Input Voltage / 3.3V) × 4095

## RP2040 ADC Hardware

The RP2040 microcontroller features:
- 4 ADC input channels (ADC0-ADC3)
- 1 internal temperature sensor (ADC4)
- 12-bit resolution
- 500kS/s maximum sampling rate
- Built-in temperature sensor

### ADC Pin Mapping
- ADC0: GPIO26
- ADC1: GPIO27
- ADC2: GPIO28
- ADC3: GPIO29
- ADC4: Internal temperature sensor

## Assignment: Potentiometer-Controlled LED Brightness

Create a program that reads analog values from a potentiometer and uses them to control the brightness of an LED using PWM.

### Requirements

1. Connect a potentiometer to one of the ADC inputs (GPIO26-29)
2. Connect an LED with appropriate resistor to a PWM-capable GPIO
3. Configure the ADC for continuous reading
4. Configure PWM for LED brightness control
5. Map ADC values (0-4095) to PWM duty cycle values
6. Update LED brightness based on potentiometer position

### Hardware Setup

1. **Potentiometer Connection**:
   - Outer pins: 3.3V and GND
   - Middle pin (wiper): ADC input (GPIO26-29)

2. **LED Connection**:
   - Anode: GPIO pin (through resistor)
   - Cathode: GND

### Required Headers
```c
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
```

### CMake Configuration
Add hardware support in CMakeLists.txt:
```cmake
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_adc
    hardware_pwm
)
```

## Development Environment Setup

1. Install Visual Studio Code
2. Install the "Raspberry Pi Pico" extension (raspberry-pi.raspberry-pi-pico)

## Building with VS Code

1. Create new C/C++ project with Raspberry Pi extension (mark the option `Console over USB (disables other USB use)`)
2. Edit the .c program
3. Press `Compile` button on right bottom corner

## Flashing the Firmware

1. Connect the RP2040 board to your computer.
2. Press and hold the BOOTSEL button while plugging the board into USB.
3. Copy the generated `.uf2` file from the `build` directory to the RP2040 USB mass storage device.

## SDK Functions and Implementation Guide

### 1. ADC Initialization
```c
adc_init();
adc_gpio_init(26);  // Initialize GPIO26 for ADC use
adc_select_input(0); // Select ADC0 input
```
These functions initialize the ADC hardware and configure the GPIO for analog input.

### 2. Reading ADC Values
```c
uint16_t result = adc_read();
```
Reads the current ADC value (0-4095).

### 3. Value Mapping Function
```c
uint16_t map_value(uint16_t value, uint16_t in_min, uint16_t in_max, 
                   uint16_t out_min, uint16_t out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
```

## Implementation Tips

1. **Reading Stability**
   - Take multiple readings and average them
   - Add small delays between readings
   - Consider implementing basic filtering

2. **Value Mapping**
   - ADC range: 0-4095
   - PWM range: 0-65535 (or your chosen wrap value)
   - Ensure proper type conversion to avoid overflow

3. **Smooth Updates**
   - Consider implementing hysteresis
   - Add small delays between updates
   - Filter out noise in ADC readings

## Extra Challenges

1. Add features like:
   - Button to toggle between linear and logarithmic brightness control
   - Multiple brightness patterns triggered by potentiometer positions
   - Smooth transitions between values

2. Extend functionality:
   - Control multiple LEDs
   - Add a display showing current values
   - Implement different response curves

### Resources

- [Raspberry Pi Pico C/C++ SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)
- [VS Code Pico Extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico)
- [Pico Examples](https://github.com/raspberrypi/pico-examples)
- [Hardware ADC API](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_adc)

## Troubleshooting

Common issues and solutions:

1. VS Code Build Issues
   - Verify Pico SDK path is correctly set
   - Ensure all required tools are in PATH
   - Check CMake configuration
   - Try cleaning and rebuilding

2. No ADC readings
   - Check wiring connections
   - Verify GPIO initialization
   - Ensure proper power supply to potentiometer

3. Erratic LED behavior
   - Check PWM configuration
   - Verify value mapping
   - Add smoothing/filtering to ADC readings