/**
 * @file as5600.c
 * @brief AS5600 12-Bit Programmable Contactless Potentiometer driver implementation
 */

#include "as5600.h"

/**
 * @brief Helper function to read a 16-bit value from two consecutive registers
 *
 * @param[in] dev Pointer to device structure
 * @param[in] reg_addr Register address (high byte)
 * @param[out] value Pointer to variable to store the 16-bit value
 *
 * @return AS5600_OK on success, error code on failure
 */
static as5600_err_t as5600_read_word(const as5600_dev_t *dev, uint8_t reg_addr, uint16_t *value)
{
    uint8_t data[2];
    int8_t rslt;

    if (!dev || !dev->read || !value)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    rslt = dev->read(AS5600_I2C_ADDR, reg_addr, data, 2);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    *value = ((uint16_t)data[0] << 8) | data[1];
    return AS5600_OK;
}

/**
 * @brief Helper function to write a 16-bit value to two consecutive registers
 *
 * @param[in] dev Pointer to device structure
 * @param[in] reg_addr Register address (high byte)
 * @param[in] value 16-bit value to write
 *
 * @return AS5600_OK on success, error code on failure
 */
static as5600_err_t as5600_write_word(const as5600_dev_t *dev, uint8_t reg_addr, uint16_t value)
{
    uint8_t data[2];
    int8_t rslt;

    if (!dev || !dev->write)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    data[0] = (uint8_t)(value >> 8);
    data[1] = (uint8_t)(value & 0xFF);

    rslt = dev->write(AS5600_I2C_ADDR, reg_addr, data, 2);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    return AS5600_OK;
}

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
                         as5600_delay_fptr_t delay_fptr)
{
    uint8_t check_byte;
    int8_t rslt;

    if (!dev || !write_fptr || !read_fptr || !delay_fptr)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    /* Assign function pointers */
    dev->write = write_fptr;
    dev->read = read_fptr;
    dev->delay_ms = delay_fptr;

    /* Wait for power-up time (10ms as per datasheet) */
    dev->delay_ms(10);

    /* Check if device is accessible by reading the ZMCO register */
    rslt = dev->read(AS5600_I2C_ADDR, AS5600_ZMCO_REG, &check_byte, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    /* Initialize configuration to default values */
    dev->config.power_mode = AS5600_PM_NOM;
    dev->config.hysteresis = AS5600_HYST_OFF;
    dev->config.output_stage = AS5600_OUT_ANALOG_FULL;
    dev->config.pwm_frequency = AS5600_PWM_FREQ_115HZ;
    dev->config.slow_filter = AS5600_SF_16X;
    dev->config.fast_filter_threshold = AS5600_FTH_SLOW_ONLY;
    dev->config.watchdog_enabled = 0;
    dev->config.start_position = 0;
    dev->config.stop_position = 0;
    dev->config.max_angle = 0;

    /* Read current configuration from device */
    as5600_get_config(dev, &dev->config);

    dev->initialized = 1;
    return AS5600_OK;
}

/**
 * @brief Configure the AS5600 device
 *
 * @param[in,out] dev Pointer to device structure
 * @param[in] config Pointer to configuration structure
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_set_config(as5600_dev_t *dev, const as5600_config_t *config)
{
    uint8_t conf_high, conf_low;
    int8_t rslt;

    if (!dev || !dev->write || !config)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* Prepare the configuration high byte */
    conf_high = (config->watchdog_enabled ? AS5600_CONF_WD : 0) |
                ((config->fast_filter_threshold << AS5600_CONF_FTH_POS) & AS5600_CONF_FTH_MASK) |
                (config->slow_filter & AS5600_CONF_SF_MASK);

    /* Prepare the configuration low byte */
    conf_low = ((config->pwm_frequency << AS5600_CONF_PWMF_POS) & AS5600_CONF_PWMF_MASK) |
               ((config->output_stage << AS5600_CONF_OUTS_POS) & AS5600_CONF_OUTS_MASK) |
               ((config->hysteresis << AS5600_CONF_HYST_POS) & AS5600_CONF_HYST_MASK) |
               (config->power_mode & AS5600_CONF_PM_MASK);

    /* Write configuration registers */
    rslt = dev->write(AS5600_I2C_ADDR, AS5600_CONF_HIGH_REG, &conf_high, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    rslt = dev->write(AS5600_I2C_ADDR, AS5600_CONF_LOW_REG, &conf_low, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    /* Set start position (ZPOS) if provided */
    if (config->start_position != dev->config.start_position)
    {
        rslt = as5600_set_start_position(dev, config->start_position);
        if (rslt != AS5600_OK)
        {
            return rslt;
        }
    }

    /* Set stop position (MPOS) if provided */
    if (config->stop_position != dev->config.stop_position)
    {
        rslt = as5600_set_stop_position(dev, config->stop_position);
        if (rslt != AS5600_OK)
        {
            return rslt;
        }
    }

    /* Set maximum angle (MANG) if provided */
    if (config->max_angle != dev->config.max_angle)
    {
        rslt = as5600_set_max_angle(dev, config->max_angle);
        if (rslt != AS5600_OK)
        {
            return rslt;
        }
    }

    /* Update the device configuration structure with new values */
    dev->config = *config;

    /* Wait for at least 1ms for the configuration to take effect */
    dev->delay_ms(1);

    return AS5600_OK;
}

/**
 * @brief Get the current configuration of the AS5600 device
 *
 * @param[in] dev Pointer to device structure
 * @param[out] config Pointer to configuration structure to store the current configuration
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_config(const as5600_dev_t *dev, as5600_config_t *config)
{
    uint8_t conf_high, conf_low;
    int8_t rslt;
    uint16_t value;

    if (!dev || !dev->read || !config)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized && dev != &dev) /* Allow reading config during initialization */
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* Read configuration registers */
    rslt = dev->read(AS5600_I2C_ADDR, AS5600_CONF_HIGH_REG, &conf_high, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    rslt = dev->read(AS5600_I2C_ADDR, AS5600_CONF_LOW_REG, &conf_low, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    /* Parse configuration bytes */
    config->watchdog_enabled = (conf_high & AS5600_CONF_WD) ? 1 : 0;
    config->fast_filter_threshold = (as5600_fast_filter_threshold_t)((conf_high & AS5600_CONF_FTH_MASK) >> AS5600_CONF_FTH_POS);
    config->slow_filter = (as5600_slow_filter_t)(conf_high & AS5600_CONF_SF_MASK);
    config->pwm_frequency = (as5600_pwm_freq_t)((conf_low & AS5600_CONF_PWMF_MASK) >> AS5600_CONF_PWMF_POS);
    config->output_stage = (as5600_output_stage_t)((conf_low & AS5600_CONF_OUTS_MASK) >> AS5600_CONF_OUTS_POS);
    config->hysteresis = (as5600_hysteresis_t)((conf_low & AS5600_CONF_HYST_MASK) >> AS5600_CONF_HYST_POS);
    config->power_mode = (as5600_power_mode_t)(conf_low & AS5600_CONF_PM_MASK);

    /* Read start position (ZPOS) */
    rslt = as5600_read_word(dev, AS5600_ZPOS_HIGH_REG, &value);
    if (rslt == AS5600_OK)
    {
        config->start_position = value;
    }
    else
    {
        return rslt;
    }

    /* Read stop position (MPOS) */
    rslt = as5600_read_word(dev, AS5600_MPOS_HIGH_REG, &value);
    if (rslt == AS5600_OK)
    {
        config->stop_position = value;
    }
    else
    {
        return rslt;
    }

    /* Read maximum angle (MANG) */
    rslt = as5600_read_word(dev, AS5600_MANG_HIGH_REG, &value);
    if (rslt == AS5600_OK)
    {
        config->max_angle = value;
    }
    else
    {
        return rslt;
    }

    return AS5600_OK;
}

/**
 * @brief Check if a magnet is detected
 *
 * @param[in] dev Pointer to device structure
 * @param[out] detected Pointer to variable to store detection status (1 if detected, 0 otherwise)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_check_magnet(const as5600_dev_t *dev, uint8_t *detected)
{
    uint8_t status;
    as5600_err_t rslt;

    if (!dev || !detected)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    rslt = as5600_get_status(dev, &status);
    if (rslt != AS5600_OK)
    {
        return rslt;
    }

    *detected = (status & AS5600_STATUS_MD) ? 1 : 0;
    return AS5600_OK;
}

/**
 * @brief Get the raw angle value
 *
 * @param[in] dev Pointer to device structure
 * @param[out] angle Pointer to variable to store the raw angle (0-4095)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_raw_angle(const as5600_dev_t *dev, uint16_t *angle)
{
    as5600_err_t rslt;
    uint8_t detected;

    if (!dev || !angle)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* First check if a magnet is detected */
    rslt = as5600_check_magnet(dev, &detected);
    if (rslt != AS5600_OK)
    {
        return rslt;
    }

    if (!detected)
    {
        return AS5600_ERR_NO_MAGNET;
    }

    return as5600_read_word(dev, AS5600_RAW_ANGLE_HIGH_REG, angle);
}

/**
 * @brief Get the processed angle value
 *
 * @param[in] dev Pointer to device structure
 * @param[out] angle Pointer to variable to store the processed angle (0-4095)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_angle(const as5600_dev_t *dev, uint16_t *angle)
{
    as5600_err_t rslt;
    uint8_t detected;

    if (!dev || !angle)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* First check if a magnet is detected */
    rslt = as5600_check_magnet(dev, &detected);
    if (rslt != AS5600_OK)
    {
        return rslt;
    }

    if (!detected)
    {
        return AS5600_ERR_NO_MAGNET;
    }

    return as5600_read_word(dev, AS5600_ANGLE_HIGH_REG, angle);
}

/**
 * @brief Get the angle value in degrees
 *
 * @param[in] dev Pointer to device structure
 * @param[out] angle_deg Pointer to variable to store the angle in degrees (0-360)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_angle_degrees(const as5600_dev_t *dev, float *angle_deg)
{
    uint16_t angle;
    as5600_err_t rslt;

    if (!dev || !angle_deg)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    rslt = as5600_get_angle(dev, &angle);
    if (rslt != AS5600_OK)
    {
        return rslt;
    }

    /* Convert raw angle (0-4095) to degrees (0-360) */
    *angle_deg = (float)angle * 360.0f / 4096.0f;
    return AS5600_OK;
}

/**
 * @brief Get the AGC value
 *
 * @param[in] dev Pointer to device structure
 * @param[out] agc Pointer to variable to store the AGC value (0-255)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_agc(const as5600_dev_t *dev, uint8_t *agc)
{
    int8_t rslt;

    if (!dev || !dev->read || !agc)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    rslt = dev->read(AS5600_I2C_ADDR, AS5600_AGC_REG, agc, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    return AS5600_OK;
}

/**
 * @brief Get the magnitude value
 *
 * @param[in] dev Pointer to device structure
 * @param[out] magnitude Pointer to variable to store the magnitude value
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_magnitude(const as5600_dev_t *dev, uint16_t *magnitude)
{
    return as5600_read_word(dev, AS5600_MAGNITUDE_HIGH_REG, magnitude);
}

/**
 * @brief Set the zero position (ZPOS)
 *
 * @param[in] dev Pointer to device structure
 * @param[in] position Zero position value (0-4095)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_set_start_position(as5600_dev_t *dev, uint16_t position)
{
    as5600_err_t rslt;

    if (!dev)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* Position value must be in range 0-4095 */
    if (position > 4095)
    {
        position = 4095;
    }

    rslt = as5600_write_word(dev, AS5600_ZPOS_HIGH_REG, position);
    if (rslt == AS5600_OK)
    {
        dev->config.start_position = position;
        /* Wait for at least 1ms for the configuration to take effect */
        dev->delay_ms(1);
    }

    return rslt;
}

/**
 * @brief Set the maximum position (MPOS)
 *
 * @param[in] dev Pointer to device structure
 * @param[in] position Maximum position value (0-4095)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_set_stop_position(as5600_dev_t *dev, uint16_t position)
{
    as5600_err_t rslt;

    if (!dev)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* Position value must be in range 0-4095 */
    if (position > 4095)
    {
        position = 4095;
    }

    rslt = as5600_write_word(dev, AS5600_MPOS_HIGH_REG, position);
    if (rslt == AS5600_OK)
    {
        dev->config.stop_position = position;
        /* Wait for at least 1ms for the configuration to take effect */
        dev->delay_ms(1);
    }

    return rslt;
}

/**
 * @brief Set the maximum angle (MANG)
 *
 * @param[in] dev Pointer to device structure
 * @param[in] angle Maximum angle value (0-4095, corresponds to 0-360 degrees)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_set_max_angle(as5600_dev_t *dev, uint16_t angle)
{
    as5600_err_t rslt;

    if (!dev)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* Angle value must be in range 0-4095 */
    if (angle > 4095)
    {
        angle = 4095;
    }

    /* Ensure the angle is at least 18 degrees (minimum allowed by AS5600) */
    if (angle < 204) /* 18° = (18/360) * 4096 ≈ 204 */
    {
        angle = 204;
    }

    rslt = as5600_write_word(dev, AS5600_MANG_HIGH_REG, angle);
    if (rslt == AS5600_OK)
    {
        dev->config.max_angle = angle;
        /* Wait for at least 1ms for the configuration to take effect */
        dev->delay_ms(1);
    }

    return rslt;
}

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
as5600_err_t as5600_burn_angle(as5600_dev_t *dev)
{
    uint8_t command = AS5600_BURN_ANGLE;
    uint8_t detected, burn_count;
    as5600_err_t rslt;

    if (!dev || !dev->write)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* Check if a magnet is detected */
    rslt = as5600_check_magnet(dev, &detected);
    if (rslt != AS5600_OK)
    {
        return rslt;
    }

    if (!detected)
    {
        return AS5600_ERR_NO_MAGNET;
    }

    /* Check burn count */
    rslt = as5600_get_burn_count(dev, &burn_count);
    if (rslt != AS5600_OK)
    {
        return rslt;
    }

    if (burn_count >= 3)
    {
        return AS5600_ERR_OTP_PROG; /* Already burned maximum times */
    }

    /* Send the burn command */
    rslt = dev->write(AS5600_I2C_ADDR, AS5600_BURN_REG, &command, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    /* Wait for the burn operation to complete (at least 1ms) */
    dev->delay_ms(1);

    return AS5600_OK;
}

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
as5600_err_t as5600_burn_setting(as5600_dev_t *dev)
{
    uint8_t command = AS5600_BURN_SETTING;
    uint8_t burn_count;
    as5600_err_t rslt;

    if (!dev || !dev->write)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized)
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    /* Check burn count - MANG can only be written if ZMCO = 00 */
    rslt = as5600_get_burn_count(dev, &burn_count);
    if (rslt != AS5600_OK)
    {
        return rslt;
    }

    if (burn_count > 0)
    {
        return AS5600_ERR_OTP_PROG; /* Cannot burn settings if ZPOS/MPOS already burned */
    }

    /* Send the burn command */
    rslt = dev->write(AS5600_I2C_ADDR, AS5600_BURN_REG, &command, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    /* Wait for the burn operation to complete (at least 1ms) */
    dev->delay_ms(1);

    return AS5600_OK;
}

/**
 * @brief Get the number of times the angle settings have been burned
 *
 * @param[in] dev Pointer to device structure
 * @param[out] count Pointer to variable to store the burn count (0-3)
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_burn_count(const as5600_dev_t *dev, uint8_t *count)
{
    int8_t rslt;
    uint8_t zmco;

    if (!dev || !dev->read || !count)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized && dev != &dev) /* Allow reading during initialization */
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    rslt = dev->read(AS5600_I2C_ADDR, AS5600_ZMCO_REG, &zmco, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    *count = zmco & 0x03; /* ZMCO is in the lower 2 bits */
    return AS5600_OK;
}

/**
 * @brief Get the status register value
 *
 * @param[in] dev Pointer to device structure
 * @param[out] status Pointer to variable to store the status value
 *
 * @return AS5600_OK on success, error code on failure
 */
as5600_err_t as5600_get_status(const as5600_dev_t *dev, uint8_t *status)
{
    int8_t rslt;

    if (!dev || !dev->read || !status)
    {
        return AS5600_ERR_INVALID_PARAM;
    }

    if (!dev->initialized && dev != &dev) /* Allow reading during initialization */
    {
        return AS5600_ERR_NOT_INITIALIZED;
    }

    rslt = dev->read(AS5600_I2C_ADDR, AS5600_STATUS_REG, status, 1);
    if (rslt != 0)
    {
        return AS5600_ERR_COMM;
    }

    return AS5600_OK;
}