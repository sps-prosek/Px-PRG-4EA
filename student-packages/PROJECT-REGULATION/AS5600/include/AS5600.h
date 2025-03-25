/**
 * @file as5600.h
 * @brief AS5600 12-Bit Programmable Contactless Potentiometer driver header
 *
 * This library provides an interface for the AS5600 magnetic rotary position sensor.
 * It is designed to be platform-independent, requiring only platform-specific
 * I2C read/write and delay functions to be provided by the user.
 */

#ifndef AS5600_H
#define AS5600_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief AS5600 I2C Slave Address
 */
#define AS5600_I2C_ADDR 0x36

/**
 * @brief AS5600 Register Addresses
 */
/* Configuration Registers */
#define AS5600_ZMCO_REG 0x00
#define AS5600_ZPOS_HIGH_REG 0x01
#define AS5600_ZPOS_LOW_REG 0x02
#define AS5600_MPOS_HIGH_REG 0x03
#define AS5600_MPOS_LOW_REG 0x04
#define AS5600_MANG_HIGH_REG 0x05
#define AS5600_MANG_LOW_REG 0x06
#define AS5600_CONF_HIGH_REG 0x07
#define AS5600_CONF_LOW_REG 0x08

/* Output Registers */
#define AS5600_RAW_ANGLE_HIGH_REG 0x0C
#define AS5600_RAW_ANGLE_LOW_REG 0x0D
#define AS5600_ANGLE_HIGH_REG 0x0E
#define AS5600_ANGLE_LOW_REG 0x0F

/* Status Registers */
#define AS5600_STATUS_REG 0x0B
#define AS5600_AGC_REG 0x1A
#define AS5600_MAGNITUDE_HIGH_REG 0x1B
#define AS5600_MAGNITUDE_LOW_REG 0x1C

/* Burn Commands */
#define AS5600_BURN_REG 0xFF
#define AS5600_BURN_ANGLE 0x80
#define AS5600_BURN_SETTING 0x40

/**
 * @brief Configuration Register Bit Definitions
 */
/* CONF register high byte (0x07) */
#define AS5600_CONF_WD (1 << 5)   /* Watchdog */
#define AS5600_CONF_FTH_MASK 0x1C /* Fast Filter Threshold mask */
#define AS5600_CONF_FTH_POS 2     /* Fast Filter Threshold position */
#define AS5600_CONF_SF_MASK 0x03  /* Slow Filter mask */

/* CONF register low byte (0x08) */
#define AS5600_CONF_PWMF_MASK 0xC0 /* PWM Frequency mask */
#define AS5600_CONF_PWMF_POS 6     /* PWM Frequency position */
#define AS5600_CONF_OUTS_MASK 0x30 /* Output Stage mask */
#define AS5600_CONF_OUTS_POS 4     /* Output Stage position */
#define AS5600_CONF_HYST_MASK 0x0C /* Hysteresis mask */
#define AS5600_CONF_HYST_POS 2     /* Hysteresis position */
#define AS5600_CONF_PM_MASK 0x03   /* Power Mode mask */

/**
 * @brief Status Register Bit Definitions
 */
#define AS5600_STATUS_MD (1 << 0) /* Magnet Detected */
#define AS5600_STATUS_ML (1 << 1) /* AGC Maximum Gain Overflow */
#define AS5600_STATUS_MH (1 << 2) /* AGC Minimum Gain Overflow */

    /**
     * @brief Enumeration for Power Mode settings
     */
    typedef enum
    {
        AS5600_PM_NOM = 0x00,  /* Normal Operation Mode (Default) */
        AS5600_PM_LPM1 = 0x01, /* Low Power Mode 1 (Polling time: 5ms) */
        AS5600_PM_LPM2 = 0x02, /* Low Power Mode 2 (Polling time: 20ms) */
        AS5600_PM_LPM3 = 0x03  /* Low Power Mode 3 (Polling time: 100ms) */
    } as5600_power_mode_t;

    /**
     * @brief Enumeration for Hysteresis settings
     */
    typedef enum
    {
        AS5600_HYST_OFF = 0x00,  /* Hysteresis disabled */
        AS5600_HYST_1LSB = 0x01, /* 1 LSB hysteresis */
        AS5600_HYST_2LSB = 0x02, /* 2 LSB hysteresis */
        AS5600_HYST_3LSB = 0x03  /* 3 LSB hysteresis */
    } as5600_hysteresis_t;

    /**
     * @brief Enumeration for Output Stage settings
     */
    typedef enum
    {
        AS5600_OUT_ANALOG_FULL = 0x00,    /* Analog (full range: 0% to 100% between GND and VDD) */
        AS5600_OUT_ANALOG_REDUCED = 0x01, /* Analog (reduced range: 10% to 90% between GND and VDD) */
        AS5600_OUT_PWM = 0x02             /* Digital PWM */
    } as5600_output_stage_t;

    /**
     * @brief Enumeration for PWM Frequency settings
     */
    typedef enum
    {
        AS5600_PWM_FREQ_115HZ = 0x00, /* 115Hz */
        AS5600_PWM_FREQ_230HZ = 0x01, /* 230Hz */
        AS5600_PWM_FREQ_460HZ = 0x02, /* 460Hz */
        AS5600_PWM_FREQ_920HZ = 0x03  /* 920Hz */
    } as5600_pwm_freq_t;

    /**
     * @brief Enumeration for Slow Filter settings
     */
    typedef enum
    {
        AS5600_SF_16X = 0x00, /* 16x (slowest, least noise) */
        AS5600_SF_8X = 0x01,  /* 8x */
        AS5600_SF_4X = 0x02,  /* 4x */
        AS5600_SF_2X = 0x03   /* 2x (fastest, most noise) */
    } as5600_slow_filter_t;

    /**
     * @brief Enumeration for Fast Filter Threshold settings
     */
    typedef enum
    {
        AS5600_FTH_SLOW_ONLY = 0x00, /* Slow filter only */
        AS5600_FTH_6LSB = 0x01,      /* 6 LSBs */
        AS5600_FTH_7LSB = 0x02,      /* 7 LSBs */
        AS5600_FTH_9LSB = 0x03,      /* 9 LSBs */
        AS5600_FTH_18LSB = 0x04,     /* 18 LSBs */
        AS5600_FTH_21LSB = 0x05,     /* 21 LSBs */
        AS5600_FTH_24LSB = 0x06,     /* 24 LSBs */
        AS5600_FTH_10LSB = 0x07      /* 10 LSBs */
    } as5600_fast_filter_threshold_t;

    /**
     * @brief Enumeration for function return codes
     */
    typedef enum
    {
        AS5600_OK = 0,                  /* Operation completed successfully */
        AS5600_ERR_COMM = -1,           /* Communication error */
        AS5600_ERR_INVALID_PARAM = -2,  /* Invalid parameter */
        AS5600_ERR_NO_MAGNET = -3,      /* No magnet detected */
        AS5600_ERR_MAGNET_WEAK = -4,    /* Magnet too weak */
        AS5600_ERR_MAGNET_STRONG = -5,  /* Magnet too strong */
        AS5600_ERR_OTP_PROG = -6,       /* Error in OTP programming */
        AS5600_ERR_NOT_INITIALIZED = -7 /* Device not initialized */
    } as5600_err_t;

    /**
     * @brief AS5600 device configuration structure
     */
    typedef struct
    {
        as5600_power_mode_t power_mode;                       /* Power mode */
        as5600_hysteresis_t hysteresis;                       /* Hysteresis */
        as5600_output_stage_t output_stage;                   /* Output stage */
        as5600_pwm_freq_t pwm_frequency;                      /* PWM frequency */
        as5600_slow_filter_t slow_filter;                     /* Slow filter */
        as5600_fast_filter_threshold_t fast_filter_threshold; /* Fast filter threshold */
        uint8_t watchdog_enabled;                             /* Watchdog enabled flag */
        uint16_t start_position;                              /* Zero position (ZPOS) */
        uint16_t stop_position;                               /* Maximum position (MPOS) */
        uint16_t max_angle;                                   /* Maximum angle (MANG) */
    } as5600_config_t;

    /**
     * @brief Function pointer for platform-specific I2C write operations
     *
     * @param[in] dev_addr Device I2C address
     * @param[in] reg_addr Register address to write to
     * @param[in] reg_data Pointer to data to write
     * @param[in] len Number of bytes to write
     *
     * @return 0 on success, non-zero on failure
     */
    typedef int8_t (*as5600_i2c_write_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t len);

    /**
     * @brief Function pointer for platform-specific I2C read operations
     *
     * @param[in] dev_addr Device I2C address
     * @param[in] reg_addr Register address to read from
     * @param[out] reg_data Pointer to store read data
     * @param[in] len Number of bytes to read
     *
     * @return 0 on success, non-zero on failure
     */
    typedef int8_t (*as5600_i2c_read_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t len);

    /**
     * @brief Function pointer for platform-specific delay function
     *
     * @param[in] ms Delay time in milliseconds
     */
    typedef void (*as5600_delay_fptr_t)(uint32_t ms);

    /**
     * @brief AS5600 device structure
     */
    typedef struct
    {
        as5600_i2c_write_fptr_t write; /* I2C write function */
        as5600_i2c_read_fptr_t read;   /* I2C read function */
        as5600_delay_fptr_t delay_ms;  /* Delay function */
        as5600_config_t config;        /* Device configuration */
        uint8_t initialized;           /* Initialization flag */
    } as5600_dev_t;

    /**
     * @brief Initialize the AS5600 device
     *
     * @param[in,out] dev Pointer to device structure
     * @param[in] write_fptr Pointer to platform-specific I2C write function
     * @param[in] read_fptr Pointer to platform-specific I2C read function
     * @param[in] delay_fptr Pointer to platform-specific delay function
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_init(as5600_dev_t *dev,
                             as5600_i2c_write_fptr_t write_fptr,
                             as5600_i2c_read_fptr_t read_fptr,
                             as5600_delay_fptr_t delay_fptr);

    /**
     * @brief Configure the AS5600 device
     *
     * @param[in,out] dev Pointer to device structure
     * @param[in] config Pointer to configuration structure
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_set_config(as5600_dev_t *dev, const as5600_config_t *config);

    /**
     * @brief Get the current configuration of the AS5600 device
     *
     * @param[in] dev Pointer to device structure
     * @param[out] config Pointer to configuration structure to store the current configuration
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_config(const as5600_dev_t *dev, as5600_config_t *config);

    /**
     * @brief Check if a magnet is detected
     *
     * @param[in] dev Pointer to device structure
     * @param[out] detected Pointer to variable to store detection status (1 if detected, 0 otherwise)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_check_magnet(const as5600_dev_t *dev, uint8_t *detected);

    /**
     * @brief Get the raw angle value
     *
     * @param[in] dev Pointer to device structure
     * @param[out] angle Pointer to variable to store the raw angle (0-4095)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_raw_angle(const as5600_dev_t *dev, uint16_t *angle);

    /**
     * @brief Get the processed angle value
     *
     * @param[in] dev Pointer to device structure
     * @param[out] angle Pointer to variable to store the processed angle (0-4095)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_angle(const as5600_dev_t *dev, uint16_t *angle);

    /**
     * @brief Get the angle value in degrees
     *
     * @param[in] dev Pointer to device structure
     * @param[out] angle_deg Pointer to variable to store the angle in degrees (0-360)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_angle_degrees(const as5600_dev_t *dev, float *angle_deg);

    /**
     * @brief Get the AGC value
     *
     * @param[in] dev Pointer to device structure
     * @param[out] agc Pointer to variable to store the AGC value (0-255)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_agc(const as5600_dev_t *dev, uint8_t *agc);

    /**
     * @brief Get the magnitude value
     *
     * @param[in] dev Pointer to device structure
     * @param[out] magnitude Pointer to variable to store the magnitude value
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_magnitude(const as5600_dev_t *dev, uint16_t *magnitude);

    /**
     * @brief Set the zero position (ZPOS)
     *
     * @param[in] dev Pointer to device structure
     * @param[in] position Zero position value (0-4095)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_set_start_position(as5600_dev_t *dev, uint16_t position);

    /**
     * @brief Set the maximum position (MPOS)
     *
     * @param[in] dev Pointer to device structure
     * @param[in] position Maximum position value (0-4095)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_set_stop_position(as5600_dev_t *dev, uint16_t position);

    /**
     * @brief Set the maximum angle (MANG)
     *
     * @param[in] dev Pointer to device structure
     * @param[in] angle Maximum angle value (0-4095, corresponds to 0-360 degrees)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_set_max_angle(as5600_dev_t *dev, uint16_t angle);

    /**
     * @brief Burn the angle settings (ZPOS and MPOS) to OTP memory
     *
     * @note This operation can be performed at most 3 times and is irreversible.
     *       A magnet must be present (MD = 1) for this operation to succeed.
     *
     * @param[in] dev Pointer to device structure
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_burn_angle(as5600_dev_t *dev);

    /**
     * @brief Burn the configuration settings (MANG and CONFIG) to OTP memory
     *
     * @note This operation can be performed only once and is irreversible.
     *       MANG can be written only if ZPOS and MPOS have never been permanently
     *       written (ZMCO = 00).
     *
     * @param[in] dev Pointer to device structure
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_burn_setting(as5600_dev_t *dev);

    /**
     * @brief Get the number of times the angle settings have been burned
     *
     * @param[in] dev Pointer to device structure
     * @param[out] count Pointer to variable to store the burn count (0-3)
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_burn_count(const as5600_dev_t *dev, uint8_t *count);

    /**
     * @brief Get the status register value
     *
     * @param[in] dev Pointer to device structure
     * @param[out] status Pointer to variable to store the status value
     *
     * @return AS5600_OK on success, error code on failure
     */
    as5600_err_t as5600_get_status(const as5600_dev_t *dev, uint8_t *status);

#ifdef __cplusplus
}
#endif

#endif /* AS5600_H */