#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define IN1 22
#define IN2 21
#define ENA 20

#define MAX_LEVEL 100
#define CLK_DIV 25.0f

void set_motor_speed(int speed)
{
    pwm_set_gpio_level(ENA, speed);
    if (speed > 0)
    {
        gpio_put(IN1, 1);
        gpio_put(IN2, 0);
    }
    else if (speed < 0)
    {
        gpio_put(IN1, 0);
        gpio_put(IN2, 1);
    }
    else
    {
        gpio_put(IN1, 0);
        gpio_put(IN2, 0);
    }
}

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting...\n");

    // Initialize pwm pin for LED
    gpio_init(ENA);
    gpio_set_function(ENA, GPIO_FUNC_PWM);

    uint sliceNum = pwm_gpio_to_slice_num(ENA);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, CLK_DIV);
    pwm_config_set_wrap(&config, MAX_LEVEL);
    pwm_init(sliceNum, &config, true);

    // Initialize motor pins
    gpio_init(IN1);
    gpio_set_dir(IN1, GPIO_OUT);

    gpio_init(IN2);
    gpio_set_dir(IN2, GPIO_OUT);

    int level = 0;
    bool up = true;

    while (true)
    {
        printf("Speed: %d\n", level);
        set_motor_speed(level);
        level += up ? 1 : -1;
        if (level == MAX_LEVEL)
            up = false;
        else if (level == -MAX_LEVEL)
            up = true;
        sleep_ms(50);
    }
}