# Minimum CMake version required
cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set the project name
set(PROJECT_NAME pid_dc_motor_position_control)

# Define the project and its languages
project(${PROJECT_NAME} C CXX ASM)

# Set the C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add the main executable and its source files
add_executable(${PROJECT_NAME}
        src/main.c
        ../utils/src/utils.c
)

# Link the standard library and hardware libraries to the executable
target_link_libraries(${PROJECT_NAME}
        pico_stdlib
        hardware_pwm
        )

# Include directories for header files
include_directories(
        ./include
        ../utils/include
        )

# Enable USB output and disable UART output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create additional output formats (map/bin/hex/uf2)
pico_add_extra_outputs(${PROJECT_NAME})
