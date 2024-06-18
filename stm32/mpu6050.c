/*
 * mpu6050.c
 *
 *  Created on: Jun 16, 2024
 *      Author: anushmutyala
 */


#include <mpu6050.h>

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

void mpu6050_read_gyro(ReadVec *gyro_data)
{
	uint8_t data[6];
//	ReadVec gyro_data;

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS <<1)+1, REG_GYRO_DATA, 1, data, 6, 100);
//	gyro_data.x = (((int16_t)data[0] << 8) + data[1]) / G_R;
//	gyro_data.y = (((int16_t)data[2] << 8) + data[3]) / G_R;
//	gyro_data.z = (((int16_t)data[4] << 8) + data[5]) / G_R;
//
//	printf("x axis gyro: %f \n", gyro_data.x);
//	printf("y axis gyro: %f \n", gyro_data.y);
//	printf("z axis gyro: %f \n\n", gyro_data.z);

	gyro_data->x = (((int16_t)data[0] << 8) + data[1]);
	gyro_data->y = (((int16_t)data[2] << 8) + data[3]);
	gyro_data->z = (((int16_t)data[4] << 8) + data[5]);

//	printf("x axis gyro: %d \n", gyro_data->x);
//	printf("y axis gyro: %d \n", gyro_data->y);
//	printf("z axis gyro: %d \n\n", gyro_data->z);


//	return gyro_data;

}

void mpu6050_read_acc(ReadVec *acc_data)
{
	uint8_t data[6];
//	ReadVec acc_data;

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS <<1)+1, REG_ACC_DATA, 1, data, 6, 100);
//	acc_data.x = (((int16_t)data[0] << 8) + data[1]) / A_R;
//	acc_data.y = (((int16_t)data[2] << 8) + data[3]) / A_R;
//	acc_data.z = (((int16_t)data[4] << 8) + data[5]) / A_R;
//
//	printf("x axis acc: %f \n", acc_data.x);
//	printf("y axis acc: %f \n", acc_data.y);
//	printf("z axis acc: %f \n\n", acc_data.z);

	acc_data->x = (((int16_t)data[0] << 8) + data[1]);
	acc_data->y = (((int16_t)data[2] << 8) + data[3]);
	acc_data->z = (((int16_t)data[4] << 8) + data[5]);

//	printf("x axis acc: %d \n", acc_data->x);
//	printf("y axis acc: %d \n", acc_data->y);
//	printf("z axis acc: %d \n\n", acc_data->z);

//	return acc_data;

}

void mpu6050_example_read()
{
	uint8_t data[2];
	int16_t x_acc;

	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS <<1)+1, REG_ACC_DATA, 1, data, 2, 100);
	x_acc = ((int16_t)data[0] << 8) + data[1];
	printf("x axis acceleration: %d \n", x_acc);
}

void mpu6050_acc_angles(ReadVec *acc_data, float *pitch, float* roll) {
	*pitch = atan2((float)acc_data->y, (float)acc_data->z) * RAD2DEG;
	*roll = atan2((float)acc_data->x, (float)acc_data->z) * RAD2DEG;
	printf("acc pitch is: %f \n", *pitch);
	printf("acc roll is: %f \n\n", *roll);
}

void mpu6050_gyro_angles(ReadVec *gyro_data, float *pitch, float *roll) {
	static float total_pitch = 0;
	static float total_roll = 0;

	total_pitch += (float) gyro_data->x * (G);
	total_roll += (float) gyro_data->y * (G);

	*pitch = total_pitch;
	*roll = total_roll;

	printf("gyro pitch is: %f \n", *pitch);
	printf("gyro roll is: %f \n\n", *roll);
}

void mpu6050_complementary_filter(ReadVec *gyro_data, float *acc_pitch, float *acc_roll, float *filtered_pitch, float*filtered_roll) {
	static bool first_run = true;
	static float total_pitch = 0;
	static float total_roll = 0;

	total_pitch += (float) gyro_data->x * (G);
	total_roll += (float) gyro_data->y * (G);

	if (first_run)
	{
		total_pitch = *acc_pitch;
		total_roll = *acc_roll;

		first_run = false;
	}
	else
	{
		total_pitch = total_pitch * Tau + *acc_pitch * (1-Tau);
		total_roll = total_roll * Tau + *acc_roll * (1-Tau);
	}

	*filtered_pitch = total_pitch;
	*filtered_roll = total_roll;

	printf("filtered pitch is: %f \n", *filtered_pitch);
	printf("filtered roll is: %f \n\n", *filtered_roll);

}

