/*
 * global.h
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "stdio.h"
#include "string.h"
#include <stdint.h>

#include "main.h"
#include "softwareTimer.h"
#include "lcd.h"
#include "dht.h"
#include "fsm_dht.h"
#include "uart.h"
#include "led_rgb.h"

typedef enum {
	INIT,
    READING,
    UPDATE,
    ERROR_READ
} State;

#define NUM_LEDS 1
extern uint16_t pwm_data[NUM_LEDS * 24 + 50];

extern State status;
extern DHT20_Data dht20_sensor;
extern char temp[16];
extern char humd[16];
extern char sendBuffer[40];

extern float new_temp;
extern float new_humd;
#endif /* INC_GLOBAL_H_ */
