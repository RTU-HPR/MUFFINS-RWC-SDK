#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "MUFFINS_Component_Base.h"
#include "MUFFINS_Checksums.h"
#include "rwc-regs.h"

enum RWC_COMM_ERR
{
    RWC_ERR_NONE = 0,
    RWC_ERR_CRC = 1,
};

#define RWC_COMM_ERR_CHECK(err) \
    if (err != RWC_ERR_NONE)    \
    return err

class RWC_SDK : public Component_Base
{
private:
    TwoWire *_i2cHandle;
    uint8_t _address;

    void _applyChecksum(const uint8_t *data, uint8_t size, uint8_t *checksum);
    RWC_COMM_ERR _readRegister(uint8_t reg, uint8_t *data, uint8_t size);
    RWC_COMM_ERR _writeRegister(uint8_t reg, uint8_t *data, uint8_t size);
    RWC_COMM_ERR _readFloatRegister(uint8_t reg, float *data);
    RWC_COMM_ERR _writeFloatRegister(uint8_t reg, float *data);
    RWC_COMM_ERR _checkWriteSuccess(void);

public:
    RWC_SDK(TwoWire *i2cHandle, uint8_t address);

    RWC_COMM_ERR state(RWC_STATE *state);
    RWC_COMM_ERR state(RWC_STATE state);

    RWC_COMM_ERR heartbeat();

    RWC_COMM_ERR orientationMode(RWC_ORIENTATION_MODE *mode);
    RWC_COMM_ERR orientationMode(RWC_ORIENTATION_MODE mode);

    RWC_COMM_ERR orientationSetpoint(float *orientation);
    RWC_COMM_ERR orientationSetpoint(float orientation);

    RWC_COMM_ERR speedSetpoint(float *speed);
    RWC_COMM_ERR speedSetpoint(float speed);

    RWC_COMM_ERR orientation(float *orientation);

    RWC_COMM_ERR angularSpeed(float *speed);

    RWC_COMM_ERR motorSpeed(float *speed);

    RWC_COMM_ERR calibrationStatus(uint8_t *calibration);
    RWC_COMM_ERR calibrationStatus(uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag);

    RWC_COMM_ERR motorTemp(float *temp);

    RWC_COMM_ERR batteryVoltage(float *voltage);

    RWC_COMM_ERR error(uint8_t *err);
    RWC_COMM_ERR error(bool *chckSum, bool *motorRunaway);
};