#include <stdio.h>
#include "pico/stdlib.h"

static inline uint32_t millis()
{
    return to_ms_since_boot(get_absolute_time());
}

void process_command(const char *cmd)
{
    int value;
    if (sscanf(cmd, "SET %d", &value) == 1)
    {
        printf("Setting value to %d\n", value);
    }
    else
    {
        printf("Unknown command: %s\n", cmd);
    }
}

int main()
{
    stdio_init_all();

    sleep_ms(2000); // Wait for UART to settle

    char command_buffer[32]; // Buffer for user input
    int index = 0;           // Input position

    uint32_t t0 = millis();
    int x = 0;

    while (true)
    {
        int c = getchar_timeout_us(0); // Non-blocking read

        if (c != PICO_ERROR_TIMEOUT)
        {
            if (c == '\n' || c == '\r')
            { // End of command
                command_buffer[index] = '\0';
                if (index > 0)
                {
                    process_command(command_buffer);
                }
                index = 0; // Reset buffer
            }
            else if (index < sizeof(command_buffer) - 1)
            {
                command_buffer[index++] = (char)c;
            }
        }

        if (millis() - t0 > 1000)
        {
            t0 = millis();
            printf("Hello, world! %d\n", x++);
        }

        sleep_ms(10); // Prevent 100% CPU usage
    }

    return 0;
}
