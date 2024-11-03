// Pico SDK libraries
#include "pico/stdlib.h"

#define LED_PIN 6
#define DOT_DELAY_MS 200
#define DASH_DELAY_MS 600
#define SYMBOL_SPACE_DELAY_MS 200
#define LETTER_SPACE_DELAY_MS 600
#define WORD_SPACE_DELAY_MS 3000

void blink_dot()
{
    gpio_put(LED_PIN, 1);
    sleep_ms(DOT_DELAY_MS);
    gpio_put(LED_PIN, 0);
    sleep_ms(SYMBOL_SPACE_DELAY_MS);
}

void blink_dash()
{
    gpio_put(LED_PIN, 1);
    sleep_ms(DASH_DELAY_MS);
    gpio_put(LED_PIN, 0);
    sleep_ms(SYMBOL_SPACE_DELAY_MS);
}

int main()
{
    // Initialize GPIO pins
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Infinite loop
    while (1)
    {
        // Blink S (dot dot dot)
        blink_dot();
        blink_dot();
        blink_dot();
        sleep_ms(LETTER_SPACE_DELAY_MS);

        // Blink O (dash dash dash)
        blink_dash();
        blink_dash();
        blink_dash();
        sleep_ms(LETTER_SPACE_DELAY_MS);

        // Blink S (dot dot dot)
        blink_dot();
        blink_dot();
        blink_dot();
        sleep_ms(WORD_SPACE_DELAY_MS);
    }
    return 0;
}
