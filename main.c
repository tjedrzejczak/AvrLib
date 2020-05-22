#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "project_config.h"

#include "avr_util.h"

#include "HD44780.h"
#include "HD44780_big_digits.h"

#include "twi.h"
#include "MCP7940.h"

#include "onewire.h"
#include "ds18b20.h"

void setLedPins(uint8_t led)
{
    if ((led & 0x01) == 0)
        SET_PIN_HIGH(B, 0);
    else
        SET_PIN_LOW(B, 0);

    if ((led & 0x02) == 0)
        SET_PIN_HIGH(B, 1);
    else
        SET_PIN_LOW(B, 1);

    if ((led & 0x04) == 0)
        SET_PIN_HIGH(B, 2);
    else
        SET_PIN_LOW(B, 2);
}

DEFINE_OWIRE_PORT(ow_pin, OW_1_PORT, OW_1_BIT);

int main_demo_1()
{
    char buf[9];
    uint8_t led = 0;

    // port: B pins: 0,1,2 - connected to LEDs
    SET_PIN_OUT(B, 0);
    SET_PIN_LOW(B, 0);
    SET_PIN_OUT(B, 1);
    SET_PIN_HIGH(B, 1);
    SET_PIN_OUT(B, 2);
    SET_PIN_LOW(B, 2);

    // init display HD44780 with big digits
    lcdInit();
    lcdClear();
    lcdBigDigitsInit();

    // init 1Wire port described by ow_pin struct (type owire_port_t)
    OWireInit(&ow_pin);

    // init TWI port (used by rtc)
    twiInit(CALCULATE_TWBR(200));

    // rtc: set time to 2019-12-15 14:59:30 and start the clock
    datetime_t starttime;
    starttime.year = 19;
    starttime.month = 12;
    starttime.day = 15;
    starttime.hour = 14;
    starttime.minute = 59;
    starttime.second = 30;
    rtcSetTime(starttime, false);
    rtcStart(0);

    while (1)
    {
        _delay_ms(500);

        led++;
        led %= 8;
        setLedPins(led);

        // read time from rtc and display hour and minutes using big digits
        datetime_t now = rtcReadTime(buf);
        uint8_t col = 0;
        col = lcdBigDigitsPrintBCD(col, 0, now.hour);
        col = lcdBigDigitsPrintBCD(col + 1, 0, now.minute);

        // seconds and date display normally
        lcdSprintf(col, 0, ":%02d", bcd2int(now.second));
        lcdSprintf(0, 2, "20%02d-%02d-%02d", bcd2int(now.year), bcd2int(now.month), bcd2int(now.day));

        // start reading temperature
        OWStartConversion(&ow_pin, 1);
        _delay_ms(1000);

        // read and display temperature
        int16_t temp = OWGetTemperatureSingleDevice(&ow_pin);
        tempBCD_t tbcd = OWConvertToBCD(temp);
        lcdBigDigitsPrintBCD(12, 2, (uint8_t) tbcd.intBCD);
        lcdSprintf(11, 2, "%c", (tbcd.negative) ? '-' : '+');
        lcdSprintf(18, 2, ".%d", tbcd.fractBCD);
    }
}

int main_demo_2()
{
    SET_PIN_OUT(B, 0);
    SET_PIN_LOW(B, 0);

    // init display HD44780
    lcdInit();
    lcdClear();

    // init 1Wire port described by ow_pin struct (type owire_port_t)
    OWireInit(&ow_pin);

#define MAX_DEV_COUNT 4
    uint8_t id[8];
    uint8_t ids[MAX_DEV_COUNT][7];

    while (1)
    {
        lcdClear();
        SET_PIN_HIGH(B, 0);

        // search up to 4 1wire devices
        uint8_t nDevs = 0;
        uint8_t pos = 0;
        for (uint8_t i = 0; i < MAX_DEV_COUNT; i++)
        {
            pos = OWireSearch(&ow_pin, id, pos);
            if (ow_pin.status != OW_OK)
                break;

            memcpy(&ids[i][0], id, 7);
            nDevs++;

            // display device address
            for (uint8_t c = 0; c < 8; c++)
                lcdSprintf(14 - (c * 2), i, "%02X", id[c]);

            if (pos == 0)
                break;
        }

        // start reading temperature
        OWStartConversion(&ow_pin, 1);
        _delay_ms(1000);
        SET_PIN_LOW(B, 0);

        // read and display temperature for each device
        for (uint8_t d = 0; d < nDevs; d++)
        {
            int16_t temp = OWGetTemperature(&ow_pin, &ids[d][0]);
            tempBCD_t tbcd = OWConvertToBCD(temp);

            lcdSprintf(0, d, "%c%d.%d ", (tbcd.negative) ? '-' : '+', bcd2int((uint8_t) tbcd.intBCD), tbcd.fractBCD);
        }

        _delay_ms(5000);
    }
}

int main()
{
    main_demo_1();  // HD44780 + big digits + single DS18B20 + MCP7940
    //main_demo_2();  // HD44780 + multi DS18B20
}
