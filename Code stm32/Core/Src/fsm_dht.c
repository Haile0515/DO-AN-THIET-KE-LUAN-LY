/*
 * fsm_dht.c
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */

#include "fsm_dht.h"

void fsm_run() {
	switch(status) {
		case INIT:
			lcd_init();
			status = READING;
			break;

		case READING:
			if (DHT20_ReadData(&dht20_sensor) == HAL_OK) {
				status = UPDATE;
			} else {
				status = ERROR_READ;
				lcd_send_string("READ ERROR");
			}
			break;

		case UPDATE:
			sprintf(temp, "TEMP: %.2f", dht20_sensor.Temperature);
			sprintf(humd, "HUMI: %.2f", dht20_sensor.Humidity);
			sprintf(sendBuffer, "%s;%s\n", temp, humd);
			UART_SendString(sendBuffer);

			//update temperature
			if (dht20_sensor.Temperature != new_temp) {
				lcd_goto_XY(1, 0);
				lcd_send_string("TEMP:");
				char temp[8];
				sprintf(temp, "%.2f \xDF""C", dht20_sensor.Temperature); //xDF -> kí tự degree trong bảng mã LCD
				lcd_send_string(temp);
				new_temp = dht20_sensor.Temperature;
			}

			//update humidity
			if (dht20_sensor.Humidity != new_humd) {
				lcd_goto_XY(0, 0);
				lcd_send_string("HUMI:");
				char humd[8];
				sprintf(humd, "%.2f %%", dht20_sensor.Humidity);
				lcd_send_string(humd);
				new_humd = dht20_sensor.Humidity;
			}

			Led_RGB_temp();

			status = READING;
			break;
		case ERROR_READ:
			lcd_clear_display();
			lcd_goto_XY(0, 0);
			lcd_send_string("ERROR");

			HAL_Delay(1000);
			status = READING;
		default:
			status = INIT;
			break;
	}
}
