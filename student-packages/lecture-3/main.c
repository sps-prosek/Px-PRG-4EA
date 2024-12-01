// Pico SDK libraries
#include "pico/stdlib.h"

// Standard libraries
#include <stdio.h>

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    // Wait for serial monitor to reconnect
    sleep_ms(2000);
    printf("Starting...\n");

    return 0;
}
