#include "chassis_task.h"
#include "user_lib.h"

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t chassis_task_stack;
#endif
int count = 0;
int power = 0;
int pick_count=0;//ȥ�ϰ����צ��־λ
int arm_count =0;//ȡ���צ��־λ
int jig_count=0;//ȡ���צ������־λ

void chassis_task(void *pvParameters)
{
	//����PID��ʼ��
	chassis_init();
    while (1)
    {
			if(rc_ctrl.rc.s[0] == RC_SW_UP)
			{
				//���̼��̲���
				MODE_CHASSIS = CHASSIS_KEY_CONTROL;
			}
			if(rc_ctrl.rc.s[0] == RC_SW_MID)
			{
				//��������ģʽ
				MODE_CHASSIS = CHASSIS_ZERO_FORCE;
			}
			if(rc_ctrl.rc.s[0] == RC_SW_DOWN)
			{
				//����ң��������
				MODE_CHASSIS = CHASSIS_RC_CONTROL;
			}

			chassis_mode_behaviour();
			
			//ϵͳ��ʱ
      vTaskDelay(1);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        chassis_task_stack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
