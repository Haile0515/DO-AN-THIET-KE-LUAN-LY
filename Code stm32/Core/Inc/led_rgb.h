/*
 * led_rgb.h
 *
 *  Created on: Dec 20, 2024
 *      Author: ADMIN
 */

#ifndef INC_LED_RGB_H_
#define INC_LED_RGB_H_

#include "stm32f1xx_hal.h"
#include "global.h"

extern TIM_HandleTypeDef htim2;

void WS2812_SetColor(uint8_t red, uint8_t green, uint8_t blue);
void WS2812_Send(void);
void Led_RGB_temp();

#endif /* INC_LED_RGB_H_ */
