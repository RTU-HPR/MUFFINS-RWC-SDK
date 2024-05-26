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

void RWC_SDK::_readRegister(uint8_t reg, uint8_t *data, uint8_t size)
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
    }
    else
    {
        return;
    }
}

void RWC_SDK::_writeRegister(uint8_t reg, uint8_t *data, uint8_t size)
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
}

void RWC_SDK::_readFloatRegister(uint8_t reg, float *data)
{
    uint8_t buffer[4];
    _readRegister(reg, buffer, 4);
    memcpy(data, buffer, 4);
}

void RWC_SDK::_writeFloatRegister(uint8_t reg, float *data)
{
    uint8_t buffer[4];
    memcpy(buffer, data, 4);
    _writeRegister(reg, buffer, 4);
}