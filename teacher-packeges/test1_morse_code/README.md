# Assignment: Morse Code LED Controller with Variable Brightness

## Objective
Extend a basic text-to-Morse-code converter to implement LED signaling with brightness control using a potentiometer.

## Learning Outcomes
- Understanding ADC (Analog-to-Digital Conversion) configuration and usage
- Implementing PWM (Pulse Width Modulation) for LED brightness control
- Working with hardware peripherals in embedded systems
- Real-time signal processing and timing management

## Hardware Requirements
1. Raspberry Pi Pico
2. LED (with appropriate current-limiting resistor)
3. Potentiometer
4. Breadboard and jumper wires

## CMake Configuration
Don't forget to add appropriate libraries to `CMakeLists.txt`.

## Implementation Tasks 

### 1. Hardware Configuration (25 points)
- Choose appropriate GPIO pin for LED
- Select suitable ADC input for potentiometer
- Determine appropriate PWM frequency (consider LED characteristics)
- Choose PWM wrap value based on required brightness resolution

### 2. ADC Configuration (25 points)
- Initialize ADC hardware for potentiometer reading
- Configure GPIO for analog input
- Implement proper value scaling from ADC (0-4095) to PWM range

### 3. Morse Code Functions (50 points)
- Implement display_dot() with variable brightness
- Implement display_dash() with variable brightness
- Implement display_character() with proper handling of:
  - Uppercase and lowercase letters
  - Spaces between words
  - Invalid characters
- Ensure proper timing ratios between symbols
- Read potentiometer value during display for real-time brightness control

### 4. Code Quality and Integration (additional grade)
- Proper error handling
- Clear code organization
- Well-documented functions
- Smooth brightness transitions
- Comments

## Technical Specifications

### Timing Requirements
- Dot duration: 200ms
- Dash duration: 3 × dot duration
- Space between parts of letter: 1 × dot duration
- Space between letters: 3 × dot duration
- Space between words: 7 × dot duration

## Starter Code
A base implementation is provided in 'morse_code_base.c'. You need to implement:
1. Functions marked with TODO comments
2. PWM initialization and control
3. ADC reading and scaling
4. LED brightness control

## Testing Guidelines
1. Basic functionality:
   - Verify correct Morse code timing
   - Check LED on/off operation

2. Brightness control:
   - Test minimum brightness (potentiometer at 0)
   - Test maximum brightness (potentiometer at max)
   - Verify smooth brightness transitions

3. Error conditions:
   - Handle invalid input characters
   - Manage boundary conditions for ADC/PWM values

## Troubleshooting Tips
1. No LED output:
   - Check physical connections
   - Verify PWM initialization
   - Confirm GPIO configuration

2. Incorrect brightness control:
   - Verify ADC readings
   - Check scaling calculations
   - Test PWM duty cycle range

3. Timing issues:
   - Use oscilloscope
   - Add debug print statements
   - Verify timer calculations