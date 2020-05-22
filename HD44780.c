/*
 * HD44780.c
 */

#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>

#include "project_config.h"
#include "HD44780.h"
#include "avr_util.h"

#define HD44780_CLEARDISPLAY        0x01
#define HD44780_GOTOHOME            0x02

#define HD44780_ENTRY_MODE          0x04
#define HD44780_EM_SHIFT_CURSOR     0
#define HD44780_EM_SHIFT_DISPLAY    1
#define HD44780_EM_DECREMENT        0
#define HD44780_EM_INCREMENT        2

#define HD44780_BLANK_DISPLAY       0x08
#define HD44780_RESTORE_DISPLAY     0x0C

#define HD44780_CURSOR_OFF          0
#define HD44780_CURSOR_ON           2
#define HD44780_CURSOR_NOBLINK      0
#define HD44780_CURSOR_BLINK        1

#define HD44780_FUNCTION_SET_TWO_LINE_4_BIT     0x28

#define HD44780_CGRAM_SET           0x40
#define HD44780_DDRAM_SET           0x80

uint8_t lcdReadNibble()
{
    uint8_t nibble = 0;

    if (GET_PIN(LCD_DATA_4_PORT, LCD_DATA_4_BIT))
        nibble |= (1 << 0);

    if (GET_PIN(LCD_DATA_5_PORT, LCD_DATA_5_BIT))
        nibble |= (1 << 1);

    if (GET_PIN(LCD_DATA_6_PORT, LCD_DATA_6_BIT))
        nibble |= (1 << 2);

    if (GET_PIN(LCD_DATA_7_PORT, LCD_DATA_7_BIT))
        nibble |= (1 << 3);

    return nibble;
}

void lcdWriteNibble(uint8_t nibble)
{
    if (nibble & 0x01)
        SET_PIN_HIGH(LCD_DATA_4_PORT, LCD_DATA_4_BIT);
    else
        SET_PIN_LOW(LCD_DATA_4_PORT, LCD_DATA_4_BIT);

    if (nibble & 0x02)
        SET_PIN_HIGH(LCD_DATA_5_PORT, LCD_DATA_5_BIT);
    else
        SET_PIN_LOW(LCD_DATA_5_PORT, LCD_DATA_5_BIT);

    if (nibble & 0x04)
        SET_PIN_HIGH(LCD_DATA_6_PORT, LCD_DATA_6_BIT);
    else
        SET_PIN_LOW(LCD_DATA_6_PORT, LCD_DATA_6_BIT);

    if (nibble & 0x08)
        SET_PIN_HIGH(LCD_DATA_6_PORT, LCD_DATA_7_BIT);
    else
        SET_PIN_LOW(LCD_DATA_6_PORT, LCD_DATA_7_BIT);
}

uint8_t lcdReadByte()
{
    uint8_t byte = 0;
    SET_PIN_IN(LCD_DATA_4_PORT, LCD_DATA_4_BIT);
    SET_PIN_IN(LCD_DATA_5_PORT, LCD_DATA_5_BIT);
    SET_PIN_IN(LCD_DATA_6_PORT, LCD_DATA_6_BIT);
    SET_PIN_IN(LCD_DATA_7_PORT, LCD_DATA_7_BIT);

    SET_PIN_HIGH(LCD_RW_PORT, LCD_RW_BIT);
    SET_PIN_HIGH(LCD_E_PORT, LCD_E_BIT);

    byte |= (lcdReadNibble() << 4);

    SET_PIN_LOW(LCD_E_PORT, LCD_E_BIT);
    SET_PIN_HIGH(LCD_E_PORT, LCD_E_BIT);

    byte |= lcdReadNibble();

    SET_PIN_LOW(LCD_E_PORT, LCD_E_BIT);

    return byte;
}

uint8_t lcdReadStatus()
{
    SET_PIN_LOW(LCD_RS_PORT, LCD_RS_BIT);
    return lcdReadByte();
}

void lcdWriteByte(uint8_t byte)
{
    SET_PIN_OUT(LCD_DATA_4_PORT, LCD_DATA_4_BIT);
    SET_PIN_OUT(LCD_DATA_5_PORT, LCD_DATA_5_BIT);
    SET_PIN_OUT(LCD_DATA_6_PORT, LCD_DATA_6_BIT);
    SET_PIN_OUT(LCD_DATA_7_PORT, LCD_DATA_7_BIT);

    SET_PIN_LOW(LCD_RW_PORT, LCD_RW_BIT);
    SET_PIN_HIGH(LCD_E_PORT, LCD_E_BIT);

    lcdWriteNibble(byte >> 4);

    SET_PIN_LOW(LCD_E_PORT, LCD_E_BIT);
    SET_PIN_HIGH(LCD_E_PORT, LCD_E_BIT);

    lcdWriteNibble(byte);

    SET_PIN_LOW(LCD_E_PORT, LCD_E_BIT);

    while (lcdReadStatus() & 0x80)
        ;
}

uint8_t lcdReadData()
{
    SET_PIN_HIGH(LCD_RS_PORT, LCD_RS_BIT);
    return lcdReadByte();
}

void lcdWriteCommand(uint8_t command)
{
    SET_PIN_LOW(LCD_RS_PORT, LCD_RS_BIT);
    lcdWriteByte(command);
}

void lcdWriteData(uint8_t data)
{
    SET_PIN_HIGH(LCD_RS_PORT, LCD_RS_BIT);
    lcdWriteByte(data);
}

void lcdInit()
{
    SET_PIN_OUT(LCD_DATA_4_PORT, LCD_DATA_4_BIT);
    SET_PIN_OUT(LCD_DATA_5_PORT, LCD_DATA_5_BIT);
    SET_PIN_OUT(LCD_DATA_6_PORT, LCD_DATA_6_BIT);
    SET_PIN_OUT(LCD_DATA_7_PORT, LCD_DATA_7_BIT);

    SET_PIN_OUT(LCD_RS_PORT, LCD_RS_BIT);
    SET_PIN_OUT(LCD_RW_PORT, LCD_RW_BIT);
    SET_PIN_OUT(LCD_E_PORT, LCD_E_BIT);

    _delay_ms(20);

    SET_PIN_LOW(LCD_RS_PORT, LCD_RS_BIT);
    SET_PIN_LOW(LCD_RW_PORT, LCD_RW_BIT);
    SET_PIN_LOW(LCD_E_PORT, LCD_E_BIT);

    for (int i = 0; i < 3; i++)
    {
        SET_PIN_HIGH(LCD_E_PORT, LCD_E_BIT);
        lcdWriteNibble(0x03);
        SET_PIN_LOW(LCD_E_PORT, LCD_E_BIT);
        _delay_ms(5);
    }

    SET_PIN_HIGH(LCD_E_PORT, LCD_E_BIT);
    lcdWriteNibble(0x02);
    SET_PIN_LOW(LCD_E_PORT, LCD_E_BIT);
    _delay_ms(1);
    lcdWriteCommand(HD44780_FUNCTION_SET_TWO_LINE_4_BIT);

    lcdWriteCommand(HD44780_BLANK_DISPLAY);
    lcdWriteCommand(HD44780_CLEARDISPLAY);
    lcdWriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
    lcdWriteCommand(HD44780_RESTORE_DISPLAY | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK);
}

void lcdDefineChars(uint8_t index, uint8_t len, uint8_t *buf)
{
    lcdWriteCommand(HD44780_CGRAM_SET + 8 * index);

    for (int i = 0; i < 8 * len; i++)
    {
        lcdWriteData(buf[i]);
    }

    lcdWriteCommand(HD44780_DDRAM_SET);
}

void lcdClear()
{
    lcdWriteCommand(HD44780_CLEARDISPLAY);
    _delay_ms(2);
}

void lcdHome()
{
    lcdWriteCommand(HD44780_GOTOHOME);
    _delay_ms(2);
}

void lcdGoTo(uint8_t x, uint8_t y)
{
    if (y > 1)
    {
        y = y % 2;
        x += 20;
    }

    lcdWriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}

void lcdPrintText(char *text)
{
    while (*text)
        lcdWriteData(*text++);
}

void lcdSprintf(uint8_t col, uint8_t row, const char *format, ...)
{
    static char msgBuffer[16];

    va_list args;
    va_start(args, format);
    vsprintf(msgBuffer, format, args);
    va_end(args);

    lcdGoTo(col, row);
    lcdPrintText(msgBuffer);
}

void lcdPrintBuf(uint8_t len, uint8_t *buf)
{
    for (uint8_t i = 0; i < len; i++)
        lcdWriteData(buf[i]);
}
