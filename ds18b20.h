#ifndef _DS18B20_H_
#define _DS18B20_H_

#include <stdint.h>
#include "avr_util.h"
#include "onewire.h"

#define OW_CONVERT           0x44
#define OW_COPY_SCRATCHPAD   0x48
#define OW_WRITE_SCRATCHPAD  0x4E
#define OW_READ_POWER_SUPPLY 0xB4
#define OW_RECALL_E2         0xB8
#define OW_READ_SCRATCHPAD   0xBE

typedef struct
{
    uint8_t integer :8;
    uint8_t fraction :4;
    bool negative :1;
} temp_t;

typedef struct
{
    uint16_t intBCD :12;
    uint8_t :0; //byte alignment
    uint8_t fractBCD :4;
    bool negative :1;
} tempBCD_t;

void OWStartConversion(owire_port_t *owire_port, uint8_t block);
int16_t OWGetTemperatureSingleDevice(owire_port_t *owire_port);
int16_t OWGetTemperature(owire_port_t *owire_port, uint8_t *deviceId);

temp_t OWConvert(int16_t temp);
tempBCD_t OWConvertToBCD(int16_t temp);

#endif /* _DS18B20_H_ */
