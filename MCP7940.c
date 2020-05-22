#include "MCP7940.h"
#include "twi.h"

uint8_t int2bcd(uint8_t value)
{
    return ((value / 10) << 4) | (value % 10);
}

uint8_t bcd2int(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void rtcSetTime(datetime_t datetime, bool startClock)
{
    twiSendStart();

    twiSendAddress(0xde);
    twiSendData(0x00);
    twiSendData(((startClock) ? 0x80 : 0x00) + int2bcd(datetime.second));
    twiSendData(int2bcd(datetime.minute));
    twiSendData(int2bcd(datetime.hour));
    twiSendData(0); //DoW
    twiSendData(int2bcd(datetime.day));
    twiSendData(int2bcd(datetime.month));
    twiSendData(int2bcd(datetime.year));

    twiSendStop();
}

void rtcStart(uint8_t sec)
{
    twiSendStart();

    twiSendAddress(0xde);
    twiSendData(0x00);
    twiSendData(0x80 + sec);

    twiSendStop();
}

uint8_t rtcRead(uint8_t reg)
{
    twiSendStart();

    twiSendAddress(0xde);
    twiSendData(reg);
    twiSendStart();
    twiSendAddress(0xdf);
    uint8_t ret = twiReadData(false).data;

    twiSendStop();

    return ret;
}

/* return in BCD */
datetime_t rtcReadTime()
{
    datetime_t result;
    uint8_t buf[7];

    twiSendStart();
    twiSendAddress(0xde);
    twiSendData(0x00);
    twiSendStart();
    twiSendAddress(0xdf);

    for (int i = 0; i < 7; i++)
    {
        buf[i] = twiReadData(i < 6).data;
    }

    result.second = buf[0] & 0x7F;
    result.minute = buf[1] & 0x7F;
    result.hour = buf[2] & 0x3F;
    result.day = buf[4] & 0x3F;
    result.month = buf[5] & 0x1F;
    result.year = buf[6];

    twiSendStop();

    return result;
}
