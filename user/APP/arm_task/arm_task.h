#ifndef ARM_TASK_H
#define ARM_TASK_H

#include "main.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "CAN_Receive.h"
#include "arm_behavior.h"

//ģʽ��ֵ����
extern uint8_t MODE_ARM;
extern void arm_task(void *pvParameters);
#endif
