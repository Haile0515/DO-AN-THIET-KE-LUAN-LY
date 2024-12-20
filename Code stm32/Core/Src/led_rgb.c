/*
 * led_rgb.c
 *
 *  Created on: Dec 20, 2024
 *      Author: ADMIN
 */

#include "led_rgb.h"

// Hàm ánh xạ dữ liệu màu sang PWM
void WS2812_SetColor(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = (green << 16) | (red << 8) | blue; // Dữ liệu GRB
    uint16_t idx = 0;

    for (int i = 23; i >= 0; i--) {
        if (color & (1 << i)) {
            pwm_data[idx++] = 6; // T1H: 0.7 μs
        } else {
            pwm_data[idx++] = 3; // T0H: 0.35 μs
        }
    }

    // Reset (50 μs, tối thiểu 40 khoảng trống)
    for (int i = idx; i < sizeof(pwm_data) / sizeof(uint16_t); i++) {
        pwm_data[i] = 0;
    }
}

// Hàm gửi dữ liệu PWM
void WS2812_Send(void) {
    // Bắt đầu PWM với DMA
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)pwm_data, sizeof(pwm_data) / sizeof(uint16_t));

    // Chờ DMA hoàn tất
    HAL_Delay(1); // Đảm bảo hơn 50 μs reset
}

void Led_RGB_temp(){
	if(new_temp < -20){
		WS2812_SetColor(0, 0, 255);
		WS2812_Send();
	}
	else if(new_temp < 0){
		WS2812_SetColor(0, 128, 128);
		WS2812_Send();
	}
	else if(new_temp < 20){
		WS2812_SetColor(0, 255, 0);
		WS2812_Send();
	}
	else if(new_temp < 40){
		WS2812_SetColor(85, 170, 0);
		WS2812_Send();
	}
	else if(new_temp < 60){
		WS2812_SetColor(170, 85, 0);
		WS2812_Send();
	}
	else{
		WS2812_SetColor(255, 0, 0);
		WS2812_Send();
	}
}
