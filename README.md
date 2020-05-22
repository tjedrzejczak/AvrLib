# 1-Wire (DS18B20 temperature sensor)

## 1.1

Define static instance of ``owire_port_t`` struct (before main finction):

```c
DEFINE_OWIRE_PORT(ow_pin, C, 5);
```

## 1.2 Multiple devices on the bus

```c
// init
OWireInit(&ow_pin);

// read addresses
#define MAX_DEV_COUNT 4
uint8_t id[8];
uint8_t ids[MAX_DEV_COUNT][7];
uint8_t p = 0;
for (uint8_t i = 0; i < MAX_DEV_COUNT; i++)
{
    p = OWireSearch(&ow_pin, id, p);

    if (ow_pin.status != OW_OK)
        break;

    memcpy(&ids[i][0], id, 7);

    if (p == 0)
        break;
}

// start reading temperature
OWStartConversion(&ow_pin, 1);
_delay_ms(1000);

// read temperature
int16_t temp = OWGetTemperature(&ow_pin, &ids[0 /*dev nr*/][0]);
```

## 1.3 Single slave on the bus

```c
// init
OWireInit(&ow_pin);

// start reading temperature
OWStartConversion(&ow_pin, 1);
_delay_ms(1000);

// read temperature
int16_t temp = OWGetTemperatureSingleDevice(&ow_pin);
```
