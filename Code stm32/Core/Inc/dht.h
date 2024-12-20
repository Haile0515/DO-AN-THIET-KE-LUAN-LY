/*
 * dht.h
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include "stm32f1xx_hal.h"
#define DHT20_ADDRESS 0x38 << 1

typedef struct {
    float Temperature;
    float Humidity;
} DHT20_Data;

HAL_StatusTypeDef DHT20_Init();
HAL_StatusTypeDef DHT20_ReadData(DHT20_Data *data);

#endif /* INC_DHT_H_ */
