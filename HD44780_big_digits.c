/*
 * HD44780_big_digits.c
 */

#include <stdint.h>

#include "project_config.h"
#include "HD44780.h"

/************   charset 1   ************/

#ifdef BIG_DIGITS_CHARSET_1

#define BIG_DIGITS_CHARS_WIDTH  3
#define BIG_DIGITS_CHARS_HEIGHT 3

static uint8_t charMap[10][BIG_DIGITS_CHARS_HEIGHT][BIG_DIGITS_CHARS_WIDTH] =
{
/* 0 */ {{3,7,4},{255,32,255},{5,7,6}},
/* 1 */ {{1,255,32},{32,255,32},{7,7,7}},
/* 2 */ {{1,7,4},{3,7,6},{5,7,6}},
/* 3 */ {{1,7,4},{32,7,255},{5,7,6}},
/* 4 */ {{255,32,255},{5,7,255},{32,32,7}},
/* 5 */ {{255,7,6},{7,7,4},{5,7,6}},
/* 6 */ {{3,7,2},{255,7,4},{5,7,6}},
/* 7 */ {{5,7,255},{32,3,6},{32,7,32}},
/* 8 */ {{3,7,4},{255,7,255},{5,7,6}},
/* 9 */ {{3,7,4},{5,7,255},{5,7,6}}
};

#define BIG_DIGITS_CUSTOM_CHARS_COUNT       7
#define BIG_DIGITS_CUSTOM_CHARS_START_INDEX 1

static uint8_t customCharDef[8 * BIG_DIGITS_CUSTOM_CHARS_COUNT] =
{
    0x03, 0x07, 0x0f, 0x1f, 0x00, 0x00, 0x00, 0x00, //1
    0x18, 0x1C, 0x1E, 0x1F, 0x00, 0x00, 0x00, 0x00, //2
    0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, //3
    0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, //4
    0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, //5
    0x1F, 0x1E, 0x1C, 0x18, 0x00, 0x00, 0x00, 0x00, //6
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00  //7
};
#endif

/************   charset 2   ************/

#ifdef BIG_DIGITS_CHARSET_2

#define BIG_DIGITS_CHARS_WIDTH  3
#define BIG_DIGITS_CHARS_HEIGHT 3

static uint8_t charMap[10][BIG_DIGITS_CHARS_HEIGHT][BIG_DIGITS_CHARS_WIDTH] =
{
/* 0 */ {{1,5,2},{255,32,255},{3,6,4}},
/* 1 */ {{6,1,32},{32,255,32},{6,255,6}},
/* 2 */ {{5,5,2},{1,5,5},{255,6,4}},
/* 3 */ {{5,5,2},{32,5,255},{3,6,4}},
/* 4 */ {{1,32,255},{5,5,255},{32,32,255}},
/* 5 */ {{255,5,5},{5,5,2},{3,6,4}},
/* 6 */ {{1,5,5},{255,5,2},{3,6,4}},
/* 7 */ {{5,5,255},{32,1,32},{32,255,32}},
/* 8 */ {{1,5,2},{255,5,255},{3,6,4}},
/* 9 */ {{1,5,2},{5,5,255},{3,6,4}}
};

#define BIG_DIGITS_CUSTOM_CHARS_COUNT       6
#define BIG_DIGITS_CUSTOM_CHARS_START_INDEX 1

static uint8_t customCharDef[8 * BIG_DIGITS_CUSTOM_CHARS_COUNT] =
{
    0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, //1
    0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, //2
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, 0x03, //3
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C, 0x18, //4
    0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, //5
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F  //6
};
#endif

/************   charset 3   ************/

#ifdef BIG_DIGITS_CHARSET_3

#define BIG_DIGITS_CHARS_WIDTH  2
#define BIG_DIGITS_CHARS_HEIGHT 3

static uint8_t charMap[10][BIG_DIGITS_CHARS_HEIGHT][BIG_DIGITS_CHARS_WIDTH] =
{
/* 0 */ {{0,1},{4,4},{2,3}},
/* 1 */ {{32,1},{32,4},{32,4}},
/* 2 */ {{5,1},{0,3},{2,5}},
/* 3 */ {{5,1},{5,7},{5,3}},
/* 4 */ {{4,4},{2,7},{32,4}},
/* 5 */ {{0,5},{2,1},{5,3}},
/* 6 */ {{0,5},{6,1},{2,3}},
/* 7 */ {{5,1},{32,4},{32,4}},
/* 8 */ {{0,1},{6,7},{2,3}},
/* 9 */ {{0,1},{2,7},{5,3}}
};

#define BIG_DIGITS_CUSTOM_CHARS_COUNT       8
#define BIG_DIGITS_CUSTOM_CHARS_START_INDEX 0

static uint8_t customCharDef[8 * BIG_DIGITS_CUSTOM_CHARS_COUNT] =
{
    0x00, 0x00, 0x00, 0x07, 0x0F, 0x0E, 0x0E, 0x0E, //0
    0x00, 0x00, 0x00, 0x1C, 0x1E, 0x0E, 0x0E, 0x0E, //1
    0x0E, 0x0E, 0x0E, 0x0F, 0x07, 0x00, 0x00, 0x00, //2
    0x0E, 0x0E, 0x0E, 0x1E, 0x1C, 0x00, 0x00, 0x00, //3
    0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, //4
    0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, //5
    0x0E, 0x0E, 0x0E, 0x07, 0x0E, 0x0E, 0x0E, 0x0E, //6
    0x0E, 0x0E, 0x0E, 0x1C, 0x0E, 0x0E, 0x0E, 0x0E  //7
};
#endif

/************   charset 4   ************/

#ifdef BIG_DIGITS_CHARSET_4

#define BIG_DIGITS_CHARS_WIDTH  2
#define BIG_DIGITS_CHARS_HEIGHT 3

static uint8_t charMap[10][BIG_DIGITS_CHARS_HEIGHT][BIG_DIGITS_CHARS_WIDTH] =
{
/* 0 */ {{0,1},{4,4},{2,3}},
/* 1 */ {{32,5},{32,4},{32,4}},
/* 2 */ {{176,1},{0,3},{2,176}},
/* 3 */ {{176,1},{176,7},{176,3}},
/* 4 */ {{5,5},{2,7},{32,4}},
/* 5 */ {{0,176},{2,1},{176,3}},
/* 6 */ {{0,176},{6,1},{2,3}},
/* 7 */ {{176,1},{32,4},{32,4}},
/* 8 */ {{0,1},{6,7},{2,3}},
/* 9 */ {{0,1},{2,7},{176,3}}
};

#define BIG_DIGITS_CUSTOM_CHARS_COUNT       8
#define BIG_DIGITS_CUSTOM_CHARS_START_INDEX 0

static uint8_t customCharDef[8 * BIG_DIGITS_CUSTOM_CHARS_COUNT] =
{
    0x00, 0x00, 0x00, 0x07, 0x0E, 0x0E, 0x0E, 0x0E, //0
    0x00, 0x00, 0x00, 0x1C, 0x0E, 0x0E, 0x0E, 0x0E, //1
    0x0E, 0x0E, 0x0E, 0x07, 0x00, 0x00, 0x00, 0x00, //2
    0x0E, 0x0E, 0x0E, 0x1C, 0x00, 0x00, 0x00, 0x00, //3
    0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, //4
    0x00, 0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, //5
    0x0E, 0x0E, 0x0E, 0x07, 0x0E, 0x0E, 0x0E, 0x0E, //6
    0x0E, 0x0E, 0x0E, 0x1C, 0x0E, 0x0E, 0x0E, 0x0E  //7
};
#endif

/************   charset 5   ************/

#ifdef BIG_DIGITS_CHARSET_5

#define BIG_DIGITS_CHARS_WIDTH  3
#define BIG_DIGITS_CHARS_HEIGHT 2

static uint8_t charMap[10][BIG_DIGITS_CHARS_HEIGHT][BIG_DIGITS_CHARS_WIDTH] =
{
/* 0 */ {{4,1,5},{6,2,7}},
/* 1 */ {{1,5,32},{2,255,2}},
/* 2 */ {{3,3,5},{255,3,3}},
/* 3 */ {{1,3,5},{2,3,7}},
/* 4 */ {{255,2,255},{32,32,255}},
/* 5 */ {{255,3,3},{3,3,7}},
/* 6 */ {{4,3,3},{6,3,7}},
/* 7 */ {{4,1,5},{32,32,255}},
/* 8 */ {{4,3,5},{6,3,7}},
/* 9 */ {{4,3,5},{3,3,7}}
};

#define BIG_DIGITS_CUSTOM_CHARS_COUNT       7
#define BIG_DIGITS_CUSTOM_CHARS_START_INDEX 1

static uint8_t customCharDef[8 * BIG_DIGITS_CUSTOM_CHARS_COUNT] =
{
    0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //1
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, //2
    0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, //3
    0x07, 0x0F, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, //4
    0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, //5
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, //6
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C  //7
};
#endif

void lcdBigDigitsInit()
{
    lcdDefineChars(BIG_DIGITS_CUSTOM_CHARS_START_INDEX, BIG_DIGITS_CUSTOM_CHARS_COUNT, customCharDef);
}

uint8_t lcdBigDigitsPrintBCD(uint8_t col, uint8_t row, uint8_t bcd)
{
    uint8_t chten = (bcd >> 4);
    uint8_t chone = bcd & 0x0F;

    for (uint8_t ay = 0; ay < BIG_DIGITS_CHARS_HEIGHT; ay++)
    {
        lcdGoTo(col, row + ay);
        lcdPrintBuf(BIG_DIGITS_CHARS_WIDTH, charMap[chten][ay]);
        lcdPrintBuf(BIG_DIGITS_CHARS_WIDTH, charMap[chone][ay]);
    }

    return col + (2 * BIG_DIGITS_CHARS_WIDTH);
}

uint8_t lcdBigDigitsPrintNumber(uint8_t col, uint8_t row, uint8_t number)
{
    uint8_t chhun = number / 100;
    uint8_t chten = (number / 10) % 10;
    uint8_t chone = number % 10;
    uint8_t digits = (chhun > 0) ? 3 : ((chten > 0) ? 2 : 1);

    for (uint8_t ay = 0; ay < BIG_DIGITS_CHARS_HEIGHT; ay++)
    {
        lcdGoTo(col, row + ay);
        if (digits == 3)
            lcdPrintBuf(BIG_DIGITS_CHARS_WIDTH, charMap[chhun][ay]);
        if (digits == 2)
            lcdPrintBuf(BIG_DIGITS_CHARS_WIDTH, charMap[chten][ay]);
        lcdPrintBuf(BIG_DIGITS_CHARS_WIDTH, charMap[chone][ay]);
    }

    return col + (digits * BIG_DIGITS_CHARS_WIDTH);
}
