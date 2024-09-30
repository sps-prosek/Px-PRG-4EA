// Standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Pico SDK libraries
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

// Custom libraries
#include "utils.h"

// Configuration
#include "config.h"

// Global variables
static int steps = 0;
static int dir = 1;
static uint32_t pulsDt = 0;

// Function prototypes
void EncoderIsr(uint gpio, uint32_t events);
void SetMotorSpeed(double speed);

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting PID DC motor control...\n");

    // Initialize GPIO pins
    gpio_init(MOTOR_DIR1_PIN);
    gpio_init(MOTOR_DIR2_PIN);
    gpio_set_dir(MOTOR_DIR1_PIN, GPIO_OUT);
    gpio_set_dir(MOTOR_DIR2_PIN, GPIO_OUT);

    // Initialize PWM
    gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);
    uint sliceNum = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_init(sliceNum, &config, true);

    // Enable interrupts for encoder pins
    gpio_set_irq_enabled_with_callback(ENCODER_A_PIN, GPIO_IRQ_EDGE_RISE, true, &EncoderIsr);

    // Variables for PID control
    uint32_t t, lastT, switchTime = 0, printTime = 0, rpmCheckTime = 0;
    t = lastT = micros();
    double dt, setpoint, rpm, e, integral, derivative, output, lastE;
    setpoint = 50.0;
    rpm = lastE = integral = derivative = 0.0;
    bool up = true;
    int lastSteps = 0;

    // Infinite loop
    while (1)
    {
        t = micros();
        dt = US_TO_S(t - lastT);
        lastT = t;

        if (pulsDt == 0)
            rpm = 0.0;
        else
            rpm = dir * ((60 * 1e6) / (pulsDt * ENCODER_RESOLUTION));

        if (t - rpmCheckTime > 5e4)
        {
            rpmCheckTime = t;
            if (steps - lastSteps == 0)
            {
                rpm = 0.0;
                pulsDt = 0;
            }
            lastSteps = steps;
        }

        if (t - switchTime > 5e6)
        {
            switchTime = t;
            setpoint = up ? 80.0 : 0.0;
            up = !up;
        }

        e = setpoint - rpm;

        integral += e * dt;
        integral = CROP(integral, -10.0, 10.0);

        derivative = (e - lastE) / dt;
        lastE = e;

        output = Kp * e + Ki * integral + Kd * derivative;
        output = CROP(output, -1.0, 1.0);

        if (output < 0.1 && output > -0.1)
        {
            output = 0.0;
        }

        // Generate ramp signal for output
        /*
        if (t - switchTime > 5e6)
        {
            switchTime = t;
            up = !up;
        }

        if (up)
        {
            output = (t - switchTime) / 5e6;
        }
        else
        {
            output = 1.0 - (t - switchTime) / 5e6;
        }
        */

        SetMotorSpeed(output);

        // Print values every 50 milliseconds
        if (micros() - printTime > 5e4)
        {
            printTime = micros();
            printf("Setpoint: %f, rpm: %f, Time: %f, output: %f\n", setpoint, rpm, US_TO_S(t), output);
        }

        sleep_ms(1);
    }

    // Return 0 upon successful completion
    return 0;
}

// Interrupt service routine for encoder pins
void EncoderIsr(uint gpio, uint32_t events)
{
    static uint32_t lastPulsTime = 0;
    uint32_t currentTime = micros();
    uint32_t dt = currentTime - lastPulsTime;
    if (dt < 2e3)
        return;
    pulsDt += dt;
    pulsDt /= 2;
    lastPulsTime = currentTime;
    if (gpio_get(ENCODER_B_PIN))
    {
        steps--;
        dir = -1;
    }
    else
    {
        steps++;
        dir = 1;
    }
}

// Set motor speed using PWM
void SetMotorSpeed(double speed)
{
    if (speed < 0)
    {
        gpio_put(MOTOR_DIR1_PIN, 0);
        gpio_put(MOTOR_DIR2_PIN, 1);
    }
    else
    {
        gpio_put(MOTOR_DIR1_PIN, 1);
        gpio_put(MOTOR_DIR2_PIN, 0);
    }

    pwm_set_gpio_level(MOTOR_PWM_PIN, (uint16_t)(fabs(speed) * 65535));
}