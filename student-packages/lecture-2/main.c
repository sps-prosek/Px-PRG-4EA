// Pico SDK libraries
#include "pico/stdlib.h"

// Standard libraries
#include <stdio.h>
#include <stdbool.h>

#define LED_PIN 6
#define BUTTON_PIN 20

void handler(uint gpio, uint32_t events);

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting...\n");

    // Initialize GPIO pins
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Enable interrupts for button pin
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, &handler);

    // Infinite loop
    while (1)
    {
        sleep_ms(100);
    }
    return 0;
}

void handler(uint gpio, uint32_t events)
{
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
}