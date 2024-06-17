/*
 * mpu6050.h
 *
 *  Created on: Jun 16, 2024
 *      Author: anushmutyala
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define DEVICE_ADDRESS 0x68

#define FS_GYRO_250 0
#define FS_GYRO_500 8
#define FS_GYRO_1000 9
#define FS_GYRO_2000 10

#define FS_ACC_2G 0
#define FS_ACC_4G 8
#define FS_ACC_8G 9
#define FS_ACC_16G 10

#define REG_CONFIG_GYRO 27
#define REG_CONFIG_ACC 28
#define REG_USR_CTRL 107
#define REG_ACC_DATA 59
#define REG_GYRO_DATA 67

void mpu6050_init();

typedef struct ReadVec {
	int x;
	int y;
	int z;
} ReadVec;

ReadVec mpu6050_read_gyro();
ReadVec mpu6050_read_acc();

#endif /* INC_MPU6050_H_ */
