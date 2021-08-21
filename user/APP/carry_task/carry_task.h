#ifndef CARRY_TASK_H
#define CARRY_TASK_H

#include "main.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "CAN_Receive.h"

//CARRY�ٶȻ�PID���� ������ �����޷�
#define CARRY_SPEED_KP 2.0f
#define CARRY_SPEED_KI 0.0f
#define CARRY_SPEED_KD 0.0f
#define CARRY_SPEED_MAX_OUT 10000.0f
#define CARRY_SPEED_MAX_IOUT 1000.0f

//CARRY�ǶȻ�PID���� ������ �����޷�
#define CARRY_ANGLE_KP 5.0f
#define CARRY_ANGLE_KI 0.0f
#define CARRY_ANGLE_KD 0.0f
#define CARRY_ANGLE_MAX_OUT 10000.0f
#define CARRY_ANGLE_MAX_IOUT 1000.0f

//ģʽ��ֵ����
extern void carry_task(void *pvParameters);
#endif
