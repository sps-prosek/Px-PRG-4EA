# Set the project name
set(NAME lec3)

# Add the main executable and its source files
add_executable(${NAME}
        main.c
)

# Link the standard library and hardware libraries to the executable
target_link_libraries(${NAME}
        pico_stdlib
        )

# Enable USB output and disable UART output
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# Create additional output formats (map/bin/hex/uf2)
pico_add_extra_outputs(${NAME})
