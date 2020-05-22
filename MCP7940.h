#ifndef MCP7940_H_
#define MCP7940_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} datetime_t;

uint8_t int2bcd(uint8_t bcd);
uint8_t bcd2int(uint8_t bcd);

void rtcSetTime(datetime_t datetime, bool startClock);

void rtcStart(uint8_t sec);

uint8_t rtcRead(uint8_t reg);

datetime_t rtcReadTime();

#endif /* MCP7940_H_ */
