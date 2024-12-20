/*
 * global.c
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */

#include "global.h"

DHT20_Data dht20_sensor;

char temp[16];
char humd[16];
char sendBuffer[40];

State status = INIT;
uint16_t pwm_data[NUM_LEDS * 24 + 50];

float new_temp = 0.0; //biến lưu giá trị
float new_humd = 0.0;
