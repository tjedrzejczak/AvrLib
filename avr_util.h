#ifndef _AVR_UTIL_H_
#define _AVR_UTIL_H_

#include <stdbool.h>
#include <avr/io.h>

#define REGISTER(type, letter) type##letter

#define SET_PIN_IN(portletter, bit) REGISTER(DDR, portletter) &= ~(1 << bit)
#define SET_PIN_OUT(portletter, bit) REGISTER(DDR, portletter) |= (1 << bit)
#define SET_PIN_LOW(portletter, bit) REGISTER(PORT, portletter) &= ~(1 << bit)
#define SET_PIN_HIGH(portletter, bit) REGISTER(PORT, portletter) |= (1 << bit)
#define GET_PIN(portletter, bit) (REGISTER(PIN, portletter) & (1 << bit))

#define BIT_FUNC_NAME(type_name, portletter, bit) type_name##portletter##bit

#endif /* _AVR_UTIL_H_ */
