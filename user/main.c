#include "main.h"
#include "stm32f4xx.h"
#include "buzzer.h"
#include "delay.h"
#include "led.h"
#include "rc.h"
#include "pwm.h"
#include "rng.h"
#include "sys.h"
#include "timer.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "remote_control.h"
#include "start_task.h"
#include "can.h"
#include "relay.h"
void BSP_init(void);

int main(void)
{
    BSP_init();
    delay_ms(200);
    startTast();
    vTaskStartScheduler();

    while (1)
    {

    }
}

void BSP_init(void)
{
    //�ж��� 4
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    //��ʼ���δ�ʱ��
    delay_init(configTICK_RATE_HZ);
    //LED��ʼ��
    led_configuration();
	  //ң������ʼ��
    remote_control_init();
		//��̨�����ʼ��
		Gimbal_PWM_Init();
		//CAN�ӿڳ�ʼ��
    CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
    CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
	  //GPIO��ʼ��
		gpio_relays_init();
}
