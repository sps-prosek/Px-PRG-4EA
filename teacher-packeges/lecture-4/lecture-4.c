#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

// Configuration
#define ADC_PIN 26      // Using ADC0 (GPIO26)
#define LED_PIN 16      // PWM-capable GPIO pin for LED
#define ADC_NUM 0       // ADC0 input number
#define SAMPLE_COUNT 10 // Number of samples to average

// Function to map values from one range to another
uint16_t map_value(uint16_t value, uint16_t in_min, uint16_t in_max,
                   uint16_t out_min, uint16_t out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function to get average ADC reading
uint16_t get_averaged_adc()
{
    uint32_t sum = 0;

    // Take multiple readings and sum them
    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        sum += adc_read();
        sleep_ms(1); // Small delay between readings
    }

    // Return average
    return sum / SAMPLE_COUNT;
}

int main()
{
    // Initialize stdio
    stdio_init_all();

    // Initialize ADC
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(ADC_NUM);

    // Initialize PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);

    // Set PWM configuration
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 65535); // Set wrap value to maximum (16-bit)
    pwm_init(slice_num, &config, true);

    while (1)
    {
        // Get averaged ADC reading
        uint16_t adc_value = get_averaged_adc();

        // Map ADC value (0-4095) to PWM range (0-65535)
        uint16_t target_pwm = map_value(adc_value, 0, 4095, 0, 65535);

        // Set PWM duty cycle
        pwm_set_chan_level(slice_num, PWM_CHAN_A, target_pwm);

        // Print values for debugging (optional)
        printf("ADC: %d, PWM: %d\n", adc_value, target_pwm);

        // Small delay to prevent overwhelming the system
        sleep_ms(10);
    }

    return 0;
}