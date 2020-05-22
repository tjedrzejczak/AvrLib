#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#include <util\atomic.h>
#include <util/delay.h>
#include "avr_util.h"

#define OW_READ_ROM                0x33
#define OW_MATCH_ROM               0x55
#define OW_SKIP_ROM                0xCC
#define OW_ALARM_SEARCH            0xEC
#define OW_SEARCH_ROM              0xF0

#define OW_OK                      0
#define OW_ERROR_NO_PRESENCE_PULSE 10
#define OW_ERROR_BUS_SHORTED       20
#define OW_ERROR_NO_RESPONSE       50

#define MAX_COUNT_GET_PIN          255

typedef struct
{
    uint8_t status;
    void (*init)();
    bool (*readBit)(uint8_t *status);
    void (*sendBit)(bool bit);
    bool (*waitForPresencePulse)(uint8_t *status);
} owire_port_t;

/**
 *
 * Using the macro causes that SBI/CLI instructions are used to set/clear single bit in IO port.
 *
 * If the port was a function parameter, the entire byte would be read,
 * the value would be changed in the register
 * and the byte would be written back to the port register.
 *
 **/
#define DEFINE_OWIRE_PORT(_variable_name, _port, _bit)                                        \
static inline void BIT_FUNC_NAME(_OWireInit_, _port, _bit)() {                                \
    SET_PIN_IN(_port, _bit);                                                                  \
    SET_PIN_LOW(_port, _bit);                                                                 \
};                                                                                            \
static inline bool BIT_FUNC_NAME(_OWireReadBit_, _port, _bit)(uint8_t *status) {              \
    bool value = false;                                                                       \
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )                                                       \
    {                                                                                         \
        SET_PIN_OUT(_port, _bit);                                                             \
        _delay_us(3);                                                                         \
        SET_PIN_IN(_port, _bit);                                                              \
        _delay_us(15);                                                                        \
        value = GET_PIN(_port, _bit);                                                         \
    }                                                                                         \
    uint8_t i = 0;                                                                            \
    for (; (i < MAX_COUNT_GET_PIN) && !GET_PIN(_port, _bit); i++)                             \
        _delay_us(2);                                                                         \
    if (i == MAX_COUNT_GET_PIN)                                                               \
        *status = OW_ERROR_BUS_SHORTED;                                                       \
    return value;                                                                             \
};                                                                                            \
static inline void BIT_FUNC_NAME(_OWireSendBit_, _port, _bit)(bool bit) {                     \
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )                                                       \
    {                                                                                         \
        SET_PIN_OUT(_port, _bit);                                                             \
        _delay_us(3);                                                                         \
        if (bit)                                                                              \
            SET_PIN_IN(_port, _bit);                                                          \
        _delay_us(60);                                                                        \
        SET_PIN_IN(_port, _bit);                                                              \
    }                                                                                         \
};                                                                                            \
static inline bool BIT_FUNC_NAME(_OWireWaitForPresencePulse_, _port, _bit)(uint8_t *status) { \
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )                                                       \
    {                                                                                         \
        SET_PIN_OUT(_port, _bit);                                                             \
        _delay_us(480);                                                                       \
        SET_PIN_IN(_port, _bit);                                                              \
        _delay_us(30);                                                                        \
        uint8_t i = 0;                                                                        \
        for (; (i < MAX_COUNT_GET_PIN) && GET_PIN(_port, _bit); i++)                          \
            _delay_us(1);                                                                     \
        if (i == MAX_COUNT_GET_PIN) {                                                         \
            *status = OW_ERROR_NO_PRESENCE_PULSE;                                             \
            return false;                                                                     \
        }                                                                                     \
        for (i = 0; (i < MAX_COUNT_GET_PIN) && !GET_PIN(_port, _bit); i++)                    \
            _delay_us(2);                                                                     \
        if (i == MAX_COUNT_GET_PIN) {                                                         \
            *status = OW_ERROR_BUS_SHORTED;                                                   \
            return false;                                                                     \
        }                                                                                     \
        status = OW_OK;                                                                       \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    return false;                                                                             \
};                                                                                            \
static owire_port_t _variable_name = (owire_port_t) {                                         \
    .init = &BIT_FUNC_NAME(_OWireInit_, _port, _bit),                                         \
    .readBit = &BIT_FUNC_NAME(_OWireReadBit_, _port, _bit),                                   \
    .sendBit = &BIT_FUNC_NAME(_OWireSendBit_, _port, _bit),                                   \
    .waitForPresencePulse = &BIT_FUNC_NAME(_OWireWaitForPresencePulse_, _port, _bit)          \
};

void OWireInit(owire_port_t *owire_port);
uint8_t OWireReadByte(owire_port_t *owire_port);
void OWireSendByte(owire_port_t *owire_port, uint8_t byte);
uint8_t OWireSearch(owire_port_t *owire_port, uint8_t *searchIdBuf, uint8_t discrepancyPos);
void OWireSelectDevice(owire_port_t *owire_port, const uint8_t *deviceID);

#endif /* _ONEWIRE_H_ */
