/*
 * dht.c
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */
#include "dht.h"

extern I2C_HandleTypeDef hi2c1;

//Nguồn tham khảo : https://bangnguyendev.com/2023-07-28-I2C-DHT20-STM32/
//DHT20 có địa chỉ I2C là 0x38 -> master gửi lệnh đo tới cmd = 0xAC
HAL_StatusTypeDef DHT20_Init() {

	uint8_t init_data[3] = {0xAC, 0x33, 0x00};
	HAL_StatusTypeDef status;

	status = HAL_I2C_Master_Transmit(&hi2c1, DHT20_ADDRESS, init_data, 3, 2000); //gửi dữ liệu master qua slave
	//dịch qua bit 0 là bit R/W

	HAL_Delay(10); //đợi 10ms để gửi lệnh 0xAC (đo kích hoạt)
	return status;
}

HAL_StatusTypeDef DHT20_ReadData(DHT20_Data *data) {

    HAL_StatusTypeDef status;

    uint8_t command[3] = {0xAC, 0x33, 0x00};
    status = HAL_I2C_Master_Transmit(&hi2c1, DHT20_ADDRESS, command, 3, 2000);
    if (status != HAL_OK) return status;

    HAL_Delay(80);
    //chờ 80ms để cảm biến đo thành công

    uint8_t buffer_data[7];
    // Gửi lệnh yêu cầu đọc dữ liệu từ MASTER tới DHT20
    status = HAL_I2C_Master_Receive(&hi2c1, DHT20_ADDRESS, buffer_data, 7, 2000);
    if (status != HAL_OK) return status;

    // Bit[7] = 0 -> phép đo hoàn thành
    if ((buffer_data[0] & 0x80) != 0) return HAL_ERROR;

    // Độ ẩm : byte1 + byte2 + 4bit cao của byte3 (20-bit)
    // Nhiệt độ : 4bit thấp của byte3 + byte4 + byte5 (20-bit)
    uint32_t humidity_bit = ((buffer_data[1] << 16) | (buffer_data[2] << 8) | buffer_data[3]) >> 4;
    uint32_t temperature_bit = ((buffer_data[3] & 0x0F) << 16) | (buffer_data[4] << 8) | buffer_data[5];

    data->Humidity = ((float)humidity_bit / 1048576) * 100; //0 -> 1048576 (giá trị tối đa của 20 bit)
    data->Temperature = ((float)temperature_bit / 1048576) * 200 - 50;

    return HAL_OK;
}
