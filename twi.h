#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t status;
    uint8_t data;
} twi_read_t;

#define CALCULATE_TWBR(ftwi_kHz) ((((F_CPU / 1000)/(2 * ftwi_kHz))) - 8)

/*
 * Initialize.
 * Usage: twiInit(CALCULATE_TWBR(200));
 */
void twiInit(uint8_t twbr);

/*
 * Sends start command.
 * Returns status (TWSR).
 */
uint8_t twiSendStart(void);

/*
 * Sends stop command.
 * Returns status (TWSR).
 */
uint8_t twiSendStop(void);

/*
 * Sends address.
 * Returns status (TWSR).
 */
uint8_t twiSendAddress(uint8_t address);

/*
 * Sends data.
 * Returns status (TWSR).
 */
uint8_t twiSendData(uint8_t data);

/*
 * Receives data.
 * Returns struct with data (TWDR) and status (TWSR).
 */
twi_read_t twiReadData(bool ack);

#endif /* TWI_H_ */
