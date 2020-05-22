/*
 * project_config.h
 */

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_

/** 1wire port & pin **/

#define OW_1_PORT D
#define OW_1_BIT  0

/** HD44780 port & pins **/

#define LCD_RS_PORT D
#define LCD_RS_BIT  1
#define LCD_RW_PORT D
#define LCD_RW_BIT  2
#define LCD_E_PORT  D
#define LCD_E_BIT   3

#define LCD_DATA_4_PORT D
#define LCD_DATA_4_BIT  4
#define LCD_DATA_5_PORT D
#define LCD_DATA_5_BIT  5
#define LCD_DATA_6_PORT D
#define LCD_DATA_6_BIT  6
#define LCD_DATA_7_PORT D
#define LCD_DATA_7_BIT  7

/** HD44780 big digits **/

#define BIG_DIGITS_CHARSET_5

#endif /* PROJECT_CONFIG_H_ */
