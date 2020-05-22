#include "avr_util.h"

#include "ds18b20.h"
#include "onewire.h"

void OWStartConversion(owire_port_t *owire_port, uint8_t block)
{
    if (!owire_port->waitForPresencePulse(&owire_port->status))
        return;

    OWireSendByte(owire_port, OW_SKIP_ROM);
    OWireSendByte(owire_port, OW_CONVERT);
    if (block)
        while (!owire_port->readBit(&owire_port->status))
            ;
}

int16_t OWGetTemperatureSingleDevice(owire_port_t *owire_port)
{
    if (!owire_port->waitForPresencePulse(&owire_port->status))
        return 0xFFFF;

    OWireSendByte(owire_port, OW_SKIP_ROM);
    if (owire_port->status != OW_OK)
        return 0xFFFF;

    OWireSendByte(owire_port, OW_READ_SCRATCHPAD);
    int16_t temp = OWireReadByte(owire_port);
    temp = temp + (OWireReadByte(owire_port) << 8);

    return temp;
}

int16_t OWGetTemperature(owire_port_t *owire_port, uint8_t *deviceId)
{
    if (!owire_port->waitForPresencePulse(&owire_port->status))
        return 0xFFFF;

    OWireSelectDevice(owire_port, deviceId);
    if (owire_port->status != OW_OK)
        return 0xFFFF;

    OWireSendByte(owire_port, OW_READ_SCRATCHPAD);
    uint16_t temp = OWireReadByte(owire_port);
    temp = temp + (OWireReadByte(owire_port) << 8);

    return temp;
}

temp_t OWConvert(int16_t temp)
{
    temp_t conv;

    conv.negative = temp & 0x8000;
    uint16_t abstemp = (conv.negative) ? -temp : temp;
    conv.integer = abstemp / 16;
    conv.fraction = 10 * (abstemp & 0x0F) / 16;

    return conv;
}

tempBCD_t OWConvertToBCD(int16_t temp)
{
    tempBCD_t bcdtemp;

    bcdtemp.negative = temp & 0x8000;
    uint16_t abstemp = (bcdtemp.negative) ? -temp : temp;
    bcdtemp.intBCD =
    /*  1's */((abstemp >> 4) % 10) |
    /* 10's */(((abstemp / 160) % 10) << 4) |
    /*100's */((abstemp / 1600) << 8);
    bcdtemp.fractBCD = 10 * (abstemp & 0x0F) / 16;

    return bcdtemp;
}
