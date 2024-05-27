#include "rwc-sdk.h"

RWC_SDK::RWC_SDK(TwoWire *i2cHandle, uint8_t address)
    : Component_Base("Reaction Wheel"), _i2cHandle(i2cHandle), _address(address)
{
}

void RWC_SDK::_applyChecksum(const uint8_t *data, uint8_t size, uint8_t *checksum)
{
    if (sizeof(checksum) - sizeof(data) < 2)
    {
        return;
    }
    uint16_t chck = Checksum_CCITT_16::calculate(data, size);
    memcpy(checksum, data, size);
    memcpy(checksum + size, &chck, 2);
}

RWC_COMM_ERR RWC_SDK::_readRegister(uint8_t reg, uint8_t *data, uint8_t size)
{
    uint8_t *buffer = new uint8_t[size + 2];

    _i2cHandle->beginTransmission(_address);
    _i2cHandle->write(reg);
    _i2cHandle->endTransmission();
    _i2cHandle->requestFrom(_address, size);

    for (int i = 0; i < size + 2; i++)
    {
        buffer[i] = _i2cHandle->read();
    }

    if (Checksum_CCITT_16::verify(buffer, size + 2))
    {
        memcpy(data, buffer, size);
        return RWC_ERR_NONE;
    }
    else
        ;
    {
        return RWC_ERR_CRC;
    }
}

RWC_COMM_ERR RWC_SDK::_writeRegister(uint8_t reg, uint8_t *data, uint8_t size)
{
    uint8_t *buffer = new uint8_t[size + 2];
    _applyChecksum(data, size, buffer);

    _i2cHandle->beginTransmission(_address);
    _i2cHandle->write(reg);
    for (int i = 0; i < size + 2; i++)
    {
        _i2cHandle->write(buffer[i]);
    }
    _i2cHandle->endTransmission();

    delete[] buffer;

    return _checkWriteSuccess();
}

RWC_COMM_ERR RWC_SDK::_readFloatRegister(uint8_t reg, float *data)
{
    uint8_t buffer[4];
    RWC_COMM_ERR res = _readRegister(reg, buffer, 4);
    if (res != RWC_ERR_NONE)
        return res;
    memcpy(data, buffer, 4);
    return res;
}

RWC_COMM_ERR RWC_SDK::_writeFloatRegister(uint8_t reg, float *data)
{
    uint8_t buffer[4];
    memcpy(buffer, data, 4);
    return _writeRegister(reg, buffer, 4);
}

RWC_COMM_ERR RWC_SDK::_checkWriteSuccess()
{
    bool buffer;
    RWC_COMM_ERR res = error(&buffer, nullptr);
    RWC_COMM_ERR_CHECK(res);
    if (buffer)
    {
        return RWC_ERR_CRC;
    }
    return res;
}

RWC_COMM_ERR RWC_SDK::state(RWC_STATE *state)
{
    uint8_t buffer;
    RWC_COMM_ERR res = _readRegister(RWC_REGISTER::STATE, &buffer, 1);
    RWC_COMM_ERR_CHECK(res);
    *state = static_cast<RWC_STATE>(buffer);
    return res;
}

RWC_COMM_ERR RWC_SDK::state(RWC_STATE state)
{
    uint8_t buffer = static_cast<uint8_t>(state);
    RWC_COMM_ERR res = _writeRegister(RWC_REGISTER::STATE, &buffer, 1);
    return res;
}

RWC_COMM_ERR RWC_SDK::heartbeat()
{
    uint8_t buffer = 0;
    RWC_COMM_ERR res = _writeRegister(RWC_REGISTER::KEEP_ALIVE, &buffer, 1);
    return res;
}

RWC_COMM_ERR RWC_SDK::orientationMode(RWC_ORIENTATION_MODE *mode)
{
    uint8_t buffer;
    RWC_COMM_ERR res = _readRegister(RWC_REGISTER::ORIENTATION_MODE, &buffer, 1);
    RWC_COMM_ERR_CHECK(res);
    *mode = static_cast<RWC_ORIENTATION_MODE>(buffer);
    return res;
}

RWC_COMM_ERR RWC_SDK::orientationMode(RWC_ORIENTATION_MODE mode)
{
    uint8_t buffer = static_cast<uint8_t>(mode);
    RWC_COMM_ERR res = _writeRegister(RWC_REGISTER::ORIENTATION_MODE, &buffer, 1);
    return res;
}

RWC_COMM_ERR RWC_SDK::orientationSetpoint(float *orientation)
{
    return _readFloatRegister(RWC_REGISTER::ORIENTATION_SETPOINT, orientation);
}

RWC_COMM_ERR RWC_SDK::orientationSetpoint(float orientation)
{
    return _writeFloatRegister(RWC_REGISTER::ORIENTATION_SETPOINT, &orientation);
}

RWC_COMM_ERR RWC_SDK::speedSetpoint(float *speed)
{
    return _readFloatRegister(RWC_REGISTER::SPEED_SETPOINT, speed);
}

RWC_COMM_ERR RWC_SDK::speedSetpoint(float speed)
{
    return _writeFloatRegister(RWC_REGISTER::SPEED_SETPOINT, &speed);
}

RWC_COMM_ERR RWC_SDK::orientation(float *orientation)
{
    return _readFloatRegister(RWC_REGISTER::ORIENTATION, orientation);
}

RWC_COMM_ERR RWC_SDK::angularSpeed(float *speed)
{
    return _readFloatRegister(RWC_REGISTER::ANG_SPEED, speed);
}

RWC_COMM_ERR RWC_SDK::motorSpeed(float *speed)
{
    return _readFloatRegister(RWC_REGISTER::MOTOR_SPEED, speed);
}

RWC_COMM_ERR RWC_SDK::calibrationStatus(uint8_t *calibration)
{
    return _readRegister(RWC_REGISTER::CALIBRATION_STATUS, calibration, 1);
}

RWC_COMM_ERR RWC_SDK::calibrationStatus(uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag)
{
    uint8_t buffer;
    RWC_COMM_ERR res = calibrationStatus(&buffer);
    RWC_COMM_ERR_CHECK(res);
    *sys = (buffer & 0b11000000) >> 6;
    *gyro = (buffer & 0b00110000) >> 4;
    *accel = (buffer & 0b00001100) >> 2;
    *mag = buffer & 0b00000011;
    return res;
}

RWC_COMM_ERR RWC_SDK::motorTemp(float *temp)
{
    return _readFloatRegister(RWC_REGISTER::MOTOR_TEMP, temp);
}

RWC_COMM_ERR RWC_SDK::batteryVoltage(float *voltage)
{
    return _readFloatRegister(RWC_REGISTER::BATTERY_VOLTAGE, voltage);
}

RWC_COMM_ERR RWC_SDK::error(uint8_t *error)
{
    return _readRegister(RWC_REGISTER::ERROR, error, 1);
}

RWC_COMM_ERR RWC_SDK::error(bool *chckSum, bool *motorRunaway)
{
    uint8_t buffer;
    RWC_COMM_ERR res = error(&buffer);
    RWC_COMM_ERR_CHECK(res);
    *chckSum = static_cast<bool>(buffer & 0x01);
    *motorRunaway = static_cast<bool>(buffer & 0x02);
    return res;
}