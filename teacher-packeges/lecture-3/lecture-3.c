#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define LED_PIN 16

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting...\n");

    // Initialize pwm pin
    gpio_init(LED_PIN);
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);

    uint sliceNum = pwm_gpio_to_slice_num(LED_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125);
    pwm_config_set_wrap(&config, 1000);
    pwm_init(sliceNum, &config, true);

    int level = 0;
    bool up = true;

    while (true)
    {
        pwm_set_gpio_level(LED_PIN, level);
        level += up ? 1 : -1;
        if (level == 1000)
            up = false;
        else if (level == 0)
            up = true;
        sleep_ms(10);
    }
}
