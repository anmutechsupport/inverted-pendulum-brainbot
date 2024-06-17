/*
 * mpu6050.c
 *
 *  Created on: Jun 16, 2024
 *      Author: anushmutyala
 */


#include <mpu6050.h>
#include <main.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

void mpu6050_init()
{
	HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(&hi2c1, (DEVICE_ADDRESS <<1)+0, 1, 100);
	if (ret == HAL_OK)
	{
	  printf("The device is ready \n");
	}
	else
	{
	  printf("The device is not ready. Check cables \n");
	}

	uint8_t temp_data = FS_GYRO_500;
	ret = HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS <<1)+0, REG_CONFIG_GYRO, 1, &temp_data, 1, 100);
	if (ret == HAL_OK)
	{
	  printf("Configuring gyro \n");
	}
	else
	{
	  printf("Failed to configure gyro \n");
	}

	temp_data = FS_ACC_4G;
	ret = HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS <<1)+0, REG_CONFIG_ACC, 1, &temp_data, 1, 100);
	if (ret == HAL_OK)
	{
	  printf("Configuring acc \n");
	}
	else
	{
	  printf("Failed to configure acc \n");
	}

	temp_data = 0;
	ret = HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS <<1)+0, REG_USR_CTRL, 1, &temp_data, 1, 100);
	if (ret == HAL_OK)
	{
	  printf("Exiting sleep mode \n");
	}
	else
	{
	  printf("Failed to exit sleep mode \n");
	}
}

ReadVec mpu6050_read_gyro()
{
	uint8_t data[6];
	ReadVec gyro_data;

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS <<1)+1, REG_GYRO_DATA, 1, data, 6, 100);
	gyro_data.x = ((int16_t)data[0] << 8) + data[1];
	gyro_data.y = ((int16_t)data[2] << 8) + data[3];
	gyro_data.z = ((int16_t)data[4] << 8) + data[5];

	printf("x axis gyro: %d \n", gyro_data.x);
	printf("y axis gyro: %d \n", gyro_data.y);
	printf("z axis gyro: %d \n\n", gyro_data.z);

	return gyro_data;

}

ReadVec mpu6050_read_acc()
{
	uint8_t data[6];
	ReadVec acc_data;

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS <<1)+1, REG_ACC_DATA, 1, data, 6, 100);
	acc_data.x = ((int16_t)data[0] << 8) + data[1];
	acc_data.y = ((int16_t)data[2] << 8) + data[3];
	acc_data.z = ((int16_t)data[4] << 8) + data[5];

	printf("x axis acc: %d \n", acc_data.x);
	printf("y axis acc: %d \n", acc_data.y);
	printf("z axis acc: %d \n\n", acc_data.z);

	return acc_data;

}
