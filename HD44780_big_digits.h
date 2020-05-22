/*
 * HD44780_big_digits.h
 */

#ifndef HD44780_BIG_DIGITS_H_
#define HD44780_BIG_DIGITS_H_

void lcdBigDigitsInit();
uint8_t lcdBigDigitsPrintBCD(uint8_t col, uint8_t row, uint8_t bcd);
uint8_t lcdBigDigitsPrintNumber(uint8_t col, uint8_t row, uint8_t number);

#endif /* HD44780_BIG_DIGITS_H_ */
