/*
 * fsm_dht.c
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */

#include "fsm_dht.h"
#include <stdbool.h>

bool Fahrenheit = false;
uint32_t last_time = 0;

void fsm_run() {
	switch(status) {
		case INIT:
			lcd_init();
			status = READ;
			break;

		case READ:
			if (DHT20_ReadData(&dht20_sensor) == HAL_OK) {
				status = UPDATE;
			} else {
				status = ERROR_READ;
			}
			break;

		case UPDATE:
			sprintf(temp, "TEMP: %.2f", dht20_sensor.Temperature);
			sprintf(humd, "HUMI: %.2f", dht20_sensor.Humidity);
			sprintf(BufferToESP32, "%s;%s\n", temp, humd);
			UART_SendData(BufferToESP32); //send ESP32

			//update temperature
			if (dht20_sensor.Temperature != new_temp) {
				lcd_goto_XY(1, 0);
				lcd_send_string("TEMP: ");
				char temp[8];
				sprintf(temp, "%.2f\xDF""C", dht20_sensor.Temperature); //xDF -> kí tự degree
				lcd_send_string(temp);
				new_temp = dht20_sensor.Temperature;
			}

			//update humidity
			if (dht20_sensor.Humidity != new_humd) {
				lcd_goto_XY(0, 0);
				lcd_send_string("HUMI: ");
				char humd[8];
				sprintf(humd, "%.2f%%", dht20_sensor.Humidity);
				lcd_send_string(humd);
				new_humd = dht20_sensor.Humidity;
			}

			Led_RGB_temp();

			status = CONVERT;
			break;
		case CONVERT:
		    if (HAL_GetTick() - last_time >= 1000) {
		        lcd_goto_XY(1, 0);
		        lcd_send_string("TEMP: ");
		        char temp[8];

		        if (Fahrenheit) {
		            float temp_f = new_temp * 1.8 + 32;
		            sprintf(temp, "%.2f\xDF""F", temp_f);
		        } else {
		            sprintf(temp, "%.2f\xDF""C", new_temp);
		        }

		        lcd_send_string(temp);
		        Fahrenheit = !Fahrenheit;
		        last_time = HAL_GetTick();
		    }

		    if (dht20_sensor.Humidity != new_humd) {
		    	lcd_goto_XY(0, 0);
		    	lcd_send_string("HUMI: ");
		    	char humd[8];
		    	sprintf(humd, "%.2f%%", dht20_sensor.Humidity);
		    	lcd_send_string(humd);
		    	new_humd = dht20_sensor.Humidity;
		    }
		    status = READ;
		    break;

		case ERROR_READ:
			lcd_goto_XY(1, 0);
			lcd_send_string("READ ERROR!");

			HAL_Delay(1000);
			lcd_clear_display();
			status = READ;
		default:
			status = INIT;
			break;
	}
}
