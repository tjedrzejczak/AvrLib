/*
 * HD44780.c
 */

#ifndef HD44780_H_
#define HD44780_H_

#include <stdint.h>

uint8_t lcdReadData(void);
void lcdWriteCommand(uint8_t command);
void lcdWriteData(uint8_t data);

void lcdInit();
void lcdDefineChars(uint8_t index, uint8_t len, uint8_t *buf);

void lcdClear();
void lcdHome();
void lcdGoTo(uint8_t x, uint8_t y);

void lcdPrintText(char *text);
void lcdSprintf(uint8_t col, uint8_t row, const char *format, ...);
void lcdPrintBuf(uint8_t len, uint8_t *buf);

#endif /* HD44780_H_ */
