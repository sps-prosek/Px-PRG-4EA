// Pico SDK libraries
#include "pico/stdlib.h"
#include "pico/time.h"

// Standard libraries
#include <stdio.h>
#include <stdbool.h>

#define UP_PIN 20
#define DW_PIN 21

bool reprint = true;
int count = 0;
uint32_t lastIrgTime = 0;

void handler(uint gpio, uint32_t events);
uint32_t millis();

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting...\n");

    // Initialize GPIO pins
    gpio_init(UP_PIN);
    gpio_set_dir(UP_PIN, GPIO_IN);

    gpio_init(DW_PIN);
    gpio_set_dir(DW_PIN, GPIO_IN);

    // Enable interrupts for button pin
    gpio_set_irq_enabled_with_callback(UP_PIN, GPIO_IRQ_EDGE_RISE, true, &handler);
    gpio_set_irq_enabled_with_callback(DW_PIN, GPIO_IRQ_EDGE_RISE, true, &handler);

    // Infinite loop
    while (1)
    {
        if (reprint)
        {
            printf("\r            \rCount: %d", count);
            reprint = false;
        }
        sleep_ms(100);
    }
    return 0;
}

void handler(uint gpio, uint32_t events)
{
    switch (gpio)
    {
    case UP_PIN:
        count++;
        break;
    case DW_PIN:
        count--;
        break;
    default:
        break;
    }
    reprint = true;
}

uint32_t millis()
{
    return to_ms_since_boot(get_absolute_time());
}