/*
 * mpu6050.h
 *
 *  Created on: Jun 16, 2024
 *      Author: anushmutyala
 */

#include <main.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

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

// Constants
#define A_R 8192.0 // acc scale factor for FS_ACC_4G (FS_SEL=1)
#define G_R 65.5 // gyro scale factor for FS_GYRO_500 (FS_SEL=1)
#define RAD2DEG 57.2957795131
#define Fs 50 // sampling freq (hz)
#define Ts 20 // sampling period (ms)
#define G 0.0003053 // 1/(Fs*G_R)
#define Tau 0.8 // gyro weightage

void mpu6050_init();

//typedef struct ReadVec {
//	float x;
//	float y;
//	float z;
//} ReadVec;

typedef struct ReadVec {
	int16_t x;
	int16_t y;
	int16_t z;
} ReadVec;

void mpu6050_read_gyro();
void mpu6050_read_acc();
void mpu6050_example_read();
void mpu6050_acc_angles(ReadVec *acc_data, float *pitch, float* roll);
void mpu6050_gyro_angles(ReadVec *gyro_data, float *pitch, float *roll);
void mpu6050_complementary_filter(ReadVec *gyro_data, float *acc_pitch, float *acc_roll, float *filtered_pitch, float*filtered_roll);

#endif /* INC_MPU6050_H_ */
