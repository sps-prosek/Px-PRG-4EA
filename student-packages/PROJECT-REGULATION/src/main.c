/**
 * @brief Example Raspberry Pi Pico (RP2040) implementation for the AS5600 driver
 *
 * This example shows how to implement the platform-specific functions
 * for an RP2040 microcontroller using the Pico SDK.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "AS5600.h"
#include "utils.h"

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1
#define I2C_FREQ 400000 // 400 KHz

// Global device structure
as5600_dev_t as5600_dev;

// Function prototypes
static int8_t pico_i2c_write(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint32_t len);
static int8_t pico_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint32_t len);
static void pico_delay_ms(uint32_t ms);
static void i2c_init_pico(void);
static void print_diagnostics(as5600_dev_t *dev);

int main()
{
    // Initialize standard I/O
    stdio_init_all();

    // Allow time for USB to initialize
    sleep_ms(3000);

    printf("\nAS5600 Magnetic Rotary Encoder Example for Raspberry Pi Pico\n");

    // Initialize I2C
    i2c_init_pico();
    printf("I2C initialized: SDA=GPIO%d, SCL=GPIO%d at %d Hz\n",
           I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ);

    // Initialize AS5600
    as5600_err_t rslt = as5600_init(&as5600_dev, pico_i2c_write, pico_i2c_read, pico_delay_ms);
    if (rslt != AS5600_OK)
    {
        printf("AS5600 initialization failed with error code: %d\n", rslt);
        while (1)
        {
            sleep_ms(100);
        } // Stop execution
    }

    printf("AS5600 initialized successfully\n");

    // Check if magnet is detected
    uint8_t detected;
    rslt = as5600_check_magnet(&as5600_dev, &detected);
    if (rslt == AS5600_OK)
    {
        if (detected)
        {
            printf("Magnet detected!\n");
        }
        else
        {
            printf("WARNING: No magnet detected! Please place a magnet above the sensor.\n");
        }
    }

    // Configure the sensor
    as5600_config_t config;
    rslt = as5600_get_config(&as5600_dev, &config);
    if (rslt != AS5600_OK)
    {
        printf("Failed to get sensor configuration: %d\n", rslt);
    }
    else
    {
        // Modify configuration
        config.power_mode = AS5600_PM_NOM;                   // Normal power mode
        config.hysteresis = AS5600_HYST_1LSB;                // 1 LSB hysteresis
        config.slow_filter = AS5600_SF_4X;                   // Medium filter setting
        config.fast_filter_threshold = AS5600_FTH_SLOW_ONLY; // Use only slow filter

        rslt = as5600_set_config(&as5600_dev, &config);
        if (rslt != AS5600_OK)
        {
            printf("Failed to set sensor configuration: %d\n", rslt);
        }
        else
        {
            printf("Sensor configured successfully\n");
        }
    }

    // Print diagnostics
    print_diagnostics(&as5600_dev);

    // Main loop
    uint64_t last_print_time = 0;
    const uint64_t print_interval_us = 1000; // 1 ms - 1000 Hz

    while (1)
    {
        uint64_t current_time = micros();

        if (current_time - last_print_time >= print_interval_us)
        {
            last_print_time = current_time;

            // Read raw angle
            uint16_t raw_angle;
            rslt = as5600_get_raw_angle(&as5600_dev, &raw_angle);
            if (rslt == AS5600_OK)
            {
                printf("Raw angle: %u", raw_angle);
            }
            else
            {
                printf("Error reading raw angle: %d", rslt);
            }

            // Read processed angle
            uint16_t angle;
            rslt = as5600_get_angle(&as5600_dev, &angle);
            if (rslt == AS5600_OK)
            {
                printf("\tScaled angle: %u", angle);
            }

            // Read angle in degrees
            float angle_deg;
            rslt = as5600_get_angle_degrees(&as5600_dev, &angle_deg);
            if (rslt == AS5600_OK)
            {
                printf("\tDegrees: %.2f°\n", angle_deg);
            }
        }
    }

    return 0;
}

/**
 * @brief Pico SDK I2C write implementation
 *
 * @param dev_addr Device I2C address
 * @param reg_addr Register address
 * @param data Pointer to data to write
 * @param len Length of data
 * @return 0 on success, non-zero on failure
 */
static int8_t pico_i2c_write(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint32_t len)
{
    int ret;
    uint8_t buffer[len + 1];

    // Prepare buffer: register address followed by data
    buffer[0] = reg_addr;
    for (uint32_t i = 0; i < len; i++)
    {
        buffer[i + 1] = data[i];
    }

    // Send data
    ret = i2c_write_blocking(I2C_PORT, dev_addr, buffer, len + 1, false);

    return (ret == PICO_ERROR_GENERIC || ret != (len + 1)) ? -1 : 0;
}

/**
 * @brief Pico SDK I2C read implementation
 *
 * @param dev_addr Device I2C address
 * @param reg_addr Register address
 * @param data Pointer to store read data
 * @param len Length of data to read
 * @return 0 on success, non-zero on failure
 */
static int8_t pico_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint32_t len)
{
    int ret;

    // Send register address
    ret = i2c_write_blocking(I2C_PORT, dev_addr, &reg_addr, 1, true); // true to keep master control of bus
    if (ret == PICO_ERROR_GENERIC || ret != 1)
    {
        return -1;
    }

    // Read data
    ret = i2c_read_blocking(I2C_PORT, dev_addr, data, len, false);
    if (ret == PICO_ERROR_GENERIC || ret != len)
    {
        return -1;
    }

    return 0;
}

/**
 * @brief Pico SDK delay implementation
 *
 * @param ms Delay time in milliseconds
 */
static void pico_delay_ms(uint32_t ms)
{
    sleep_ms(ms);
}

/**
 * @brief Initialize the I2C interface for RP2040
 */
static void i2c_init_pico(void)
{
    // Initialize I2C port at 400 kHz
    i2c_init(I2C_PORT, I2C_FREQ);

    // Setup GPIO pins
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Enable pull-ups
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

/**
 * @brief Print sensor diagnostics
 *
 * @param dev Pointer to AS5600 device structure
 */
static void print_diagnostics(as5600_dev_t *dev)
{
    uint8_t status, detected, agc, burn_count;
    uint16_t raw_angle, magnitude;
    as5600_err_t rslt;

    printf("\nAS5600 Diagnostics:\n");

    // Check magnet status
    rslt = as5600_get_status(dev, &status);
    if (rslt == AS5600_OK)
    {
        printf("Status Register: 0x%02X\n", status);
        printf("  Magnet detected: %s\n", (status & AS5600_STATUS_MD) ? "Yes" : "No");
        printf("  Magnet too weak: %s\n", (status & AS5600_STATUS_ML) ? "Yes" : "No");
        printf("  Magnet too strong: %s\n", (status & AS5600_STATUS_MH) ? "Yes" : "No");
    }
    else
    {
        printf("Failed to read status: %d\n", rslt);
    }

    // Get AGC value (automatic gain control)
    rslt = as5600_get_agc(dev, &agc);
    if (rslt == AS5600_OK)
    {
        printf("AGC value: %u\n", agc);
        if (agc < 80)
        {
            printf("  Magnet is too strong - increase distance\n");
        }
        else if (agc > 180)
        {
            printf("  Magnet is too weak - decrease distance\n");
        }
        else
        {
            printf("  Magnet strength is good\n");
        }
    }

    // Get magnitude
    rslt = as5600_get_magnitude(dev, &magnitude);
    if (rslt == AS5600_OK)
    {
        printf("Magnitude: %u\n", magnitude);
    }

    // Get OTP burn count
    rslt = as5600_get_burn_count(dev, &burn_count);
    if (rslt == AS5600_OK)
    {
        printf("OTP Burn Count: %u/3\n", burn_count);
    }
}
