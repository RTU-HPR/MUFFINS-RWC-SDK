#pragma once

/**
 * @brief Vehicle I2C registers. Check main document fore more.
 *
 */
enum RWC_REGISTER
{
    STATE,
    KEEP_ALIVE,
    ORIENTATION_MODE,
    ORIENTATION_SETPOINT,
    SPEED_SETPOINT,
    ORIENTATION,
    ANG_SPEED,
    MOTOR_SPEED,
    ERROR,
    CALIBRATION_STATUS,
    MOTOR_TEMP,
    BATTERY_VOLTAGE,
};

/**
 * @brief Vehicle I2C registers size. Check main document for more.
 *
 */

enum RWC_REG_SIZE
{
    STATE_SIZE = 1,
    KEEP_ALIVE_SIZE = 1,
    ORIENTATION_MODE_SIZE = 1,
    ORIENTATION_SETPOINT_SIZE = 4,
    SPEED_SETPOINT_SIZE = 4,
    ORIENTATION_SIZE = 4,
    ANG_SPEED_SIZE = 4,
    MOTOR_SPEED_SIZE = 4,
    ERROR_SIZE = 1,
    CALIBRATION_STATUS_SIZE = 1,
    MOTOR_TEMP_SIZE = 4,
    BATTERY_VOLTAGE_SIZE = 4,
};

/**
 * @brief Vehicle state.
 *
 */

enum RWC_STATE
{
    IDLE,
    STAB
};

/**
 * @brief Vehicle orientation mode.
 *
 */

enum RWC_ORIENTATION_MODE
{
    ORIENTATION_HOLD,
    SPEED_HOLD
};

/**
 * @brief Vehicle telemetry new data flags.
 *
 */

enum RWC_NEW_DATA
{
    DATA_CLEAR,
    NEW_ORIENTATION = 1 << 0,
    NEW_ANG_SPEED = 1 << 1,
    NEW_MOTOR_SPEED = 1 << 2,
};

/**
 * @brief Vehicle telemetry errors.
 *
 */

enum RWC_ERROR
{
    ERR_CLEAR,
    CRC_ERR = 1 << 0,
    MOTOR_RUNAWAY = 1 << 1,
};
