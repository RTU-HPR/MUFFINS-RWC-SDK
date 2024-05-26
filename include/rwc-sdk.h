#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "MUFFINS_Component_Base.h"
#include "MUFFINS_Checksums.h"
#include "rwc-regs.h"

class RWC_SDK : public Component_Base
{
private:
    TwoWire *_i2cHandle;
    uint8_t _address;

    void _applyChecksum(const uint8_t *data, uint8_t size, uint8_t *checksum);
    void _readRegister(uint8_t reg, uint8_t *data, uint8_t size);
    void _writeRegister(uint8_t reg, uint8_t *data, uint8_t size);
    void _readFloatRegister(uint8_t reg, float *data);
    void _writeFloatRegister(uint8_t reg, float *data);

public:
    RWC_SDK(TwoWire *i2cHandle, uint8_t address);

    RWCState state();
    
    void state(RWCState state);
    void heartbeat();
    
    RWCOrientationMode orientationMode();
    void orientationMode(RWCOrientationMode mode);
    
    float orientationSetpoint();
    void orientationSetpoint(float orientation);
    
    float speedSetpoint();
    void speedSetpoint(float speed);

    float orientation();

    float angularSpeed();

    float motorSpeed();
    
    uint8_t calibrationStatus();
    void calibrationStatus(uint8_t* sys, uint8_t* gyro, uint8_t* accel, uint8_t* mag);

    float motorTemp();

    float batteryVoltage();

    uint8_t error();
    void error(uint8_t* crcErr, uint8_t* motorRunaway);
};