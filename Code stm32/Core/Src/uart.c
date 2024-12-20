/*
 * uart.c
 *
 *  Created on: Dec 20, 2024
 *      Author: ADMIN
 */

#include "uart.h"

extern UART_HandleTypeDef huart2;

HAL_StatusTypeDef UART_SendString(const char *data) {
    return HAL_UART_Transmit(&huart2, (uint8_t *)data, strlen(data), 1000);
}
