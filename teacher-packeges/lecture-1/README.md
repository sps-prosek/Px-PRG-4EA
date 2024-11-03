# Lecture 1

Introduction to Rpi pico programming with C/C++ pico-sdk.

## File Structure

- `main.c` : C source file (sample blink program)
- `CMakeLists.txt`: Cmake build configuration

## Assigment

Using the pico-sdk documentation, online resources, and pico-sdk examples, edit the sample blink program (`main.c`) so that the LED follows the SOS Morse code pattern. Between every SOS, implement a 3-second break where the LED is OFF.

### Pseudocode for SOS Morse Code Pattern

1. Define LED pin.
2. Initialize LED pin for output.
3. Create function for dot:
   - Turn LED on.
   - Wait for a short duration.
   - Turn LED off.
   - Wait for a short duration.
4. Create function for dash:
   - Turn LED on.
   - Wait for a longer duration.
   - Turn LED off.
   - Wait for a short duration.
5. Create function for SOS pattern:
   - Call dot function three times for 'S'.
   - Call dash function three times for 'O'.
   - Call dot function three times for 'S'.
6. In the main loop:
   - Call SOS function.
   - Wait for a longer duration (3 seconds).

### Resources
- [Raspberry Pi Pico C/C++ SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)
- [Getting started with Raspberry Pi Pico-series](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- [Pico-sdk datasheet](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Pico Examples](https://github.com/raspberrypi/pico-examples)
- [Morse Code Timing](https://en.wikipedia.org/wiki/Morse_code#Timing)

## Building the Project

1. Open terminal.

2. Navigate to this folder:

   ```sh
   cd student-packages/lecture-1/
   ```

3. Create a build directory and navigate into it:

   ```sh
   mkdir build
   cd build
   ```

4. Configure the project with CMake:

   ```sh
   cmake ..
   ```

5. Build the project (call this to rebuild the project):
   ```sh
   cmake --build .
   ```
## Flashing the Firmware

1. Connect the RP2040 board to your computer.
2. Press and hold the BOOTSEL button while plugging the board into USB.
3. Copy the generated `.uf2` file from the `build` directory to the RP2040 USB mass storage device.

### Flashing the Firmware with Picotool

1. Connect the RP2040 board to your computer.
2. Ensure the board is in BOOTSEL mode by pressing and holding the BOOTSEL button while plugging the board into USB.
3. Install `picotool` if you haven't already. Follow the instructions [here](https://github.com/raspberrypi/picotool).
4. Use `picotool` to load the firmware:

   ```sh
   picotool load -f lec1.uf2
   ```

5. Reset the board to start running the new firmware.
