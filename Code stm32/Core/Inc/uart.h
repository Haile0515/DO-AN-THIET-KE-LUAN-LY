/*
 * uart.h
 *
 *  Created on: Dec 20, 2024
 *      Author: ADMIN
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f1xx_hal.h"
#include "string.h"

HAL_StatusTypeDef UART_SendString(const char *data);

#endif /* INC_UART_H_ */
