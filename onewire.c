#include <stdbool.h>
#include <util\delay.h>
#include <util\crc16.h>
#include "onewire.h"

void OWireInit(owire_port_t *owire_port) {
    owire_port->init();
}

uint8_t OWireReadByte(owire_port_t *owire_port) {
    unsigned char byte = 0;

    for (uint8_t i = 0; i < 8; i++) {
        byte >>= 1;

        if (owire_port->readBit(&owire_port->status))
            byte |= 0x80;

        _delay_us(150);
    }
    return byte;
}

void OWireSendByte(owire_port_t *owire_port, uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        owire_port->sendBit(byte & 0x01);
        byte >>= 1;
        _delay_us(130);
    }
}

uint8_t OWireSearch(owire_port_t *owire_port, uint8_t *searchIdBuf, uint8_t discrepancyPos) {
    uint8_t idBit;
    uint8_t idBitComplementary;
    uint8_t currentBitMask;
    uint8_t newDiscrepancyPos = 0;
    uint8_t currentPos;

    owire_port->status = OW_OK;

    owire_port->waitForPresencePulse(&owire_port->status);
    OWireSendByte(owire_port, OW_SEARCH_ROM);

    for (uint8_t currentByte = 0; currentByte < 8; currentByte++) {
        for (uint8_t currentBit = 0; currentBit < 8; currentBit++) {
            currentPos = 8 * currentByte + currentBit + 1;
            currentBitMask = (1 << currentBit);

            _delay_us(60);
            idBit = owire_port->readBit(&owire_port->status);
            _delay_us(60);
            idBitComplementary = owire_port->readBit(&owire_port->status);
            _delay_us(60);

            if (idBit && idBitComplementary) {
                owire_port->status = OW_ERROR_NO_RESPONSE;
                return 0;
            }

            if (idBit ^ idBitComplementary) {
                if (idBit)
                    searchIdBuf[currentByte] |= currentBitMask;
                else
                    searchIdBuf[currentByte] &= ~currentBitMask;
            } else {
                if (currentPos == discrepancyPos)
                    searchIdBuf[currentByte] |= currentBitMask;

                if (currentPos > discrepancyPos) {
                    searchIdBuf[currentByte] &= ~currentBitMask;
                    newDiscrepancyPos = currentPos;
                } else if (!(searchIdBuf[currentByte] & currentBitMask))
                    newDiscrepancyPos = currentPos;
            }

            owire_port->sendBit(searchIdBuf[currentByte] & currentBitMask);
        }
    }

    return newDiscrepancyPos;
}

void OWireSelectDevice(owire_port_t *owire_port, const uint8_t *deviceID) {
    if (!owire_port->waitForPresencePulse(&owire_port->status))
        return;

    OWireSendByte(owire_port, OW_MATCH_ROM);
    uint8_t crc = 0;

    for (uint8_t a = 0; a < 7; a++) {
        crc = _crc_ibutton_update(crc, deviceID[a]);
        OWireSendByte(owire_port, deviceID[a]);
    }

    OWireSendByte(owire_port, crc);
}
