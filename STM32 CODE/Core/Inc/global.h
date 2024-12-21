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

typedef enum {
	INIT,
    READ,
    UPDATE,
	CONVERT,
    ERROR_READ
} State;

HAL_StatusTypeDef UART_SendData(const char *data);

extern UART_HandleTypeDef huart2;

extern State status;
extern DHT20_Data dht20_sensor;
extern char temp[16];
extern char humd[16];
extern char BufferToESP32[40];

extern float new_temp;
extern float new_humd;
#endif /* INC_GLOBAL_H_ */
