#ifndef CHASSIS_TASK_SIMPLE_H
#define CHASSIS_TASK_SIMPLE_H

#include "main.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "CAN_Receive.h"
#include "chassis_behaviour.h"


//ģʽ��ֵ����
extern uint8_t MODE_CHASSIS;
extern void chassis_task(void *pvParameters);
#endif
