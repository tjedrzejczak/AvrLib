#include <stdbool.h>
#include <avr/io.h>

#include "twi.h"

void twiInit(uint8_t twbr)
{
    TWSR = 0x00;        // prescaler = 1
    TWBR = twbr;
    TWCR = (1 << TWEN); // enable
}

uint8_t twiSendStart(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

    while (!(TWCR & (1 << TWINT)))
        ;

    return TWSR;
}

uint8_t twiSendStop(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

    while (TWCR & (1 << TWSTO))
        ;

    return TWSR;
}

uint8_t twiSendAddress(uint8_t address)
{
    TWDR = address;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
        ;

    return TWSR;
}

uint8_t twiSendData(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
        ;

    return TWSR;
}

twi_read_t twiReadData(bool ack)
{
    twi_read_t ret;

    TWCR = (1 << TWINT) | (1 << TWEN) | ((ack) ? (1 << TWEA) : 0);

    while (!(TWCR & (1 << TWINT)))
        ;

    ret.data = TWDR;
    ret.status = TWSR;

    return ret;
}
