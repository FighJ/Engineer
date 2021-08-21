#ifndef CANTASK_H
#define CANTASK_H
#include "main.h"

#define CHASSIS_CAN CAN2

//CAN���ͺͽ��յ�ID
typedef enum
{
	  CAN_ARM_ALL_ID = 0x200,
    CAN_ARM_LEFT_ID = 0x201,//���3508���
		CAN_ARM_RIGHT_ID = 0x202,//�ұ�3508���
    CAN_TABLE_LEFT_ID = 0x203,//��̨2006���
	  CAN_TABLE_RIGHT_ID = 0x204,//�һ�̨2006���

    CAN_CARRY_ALL_ID = 0x1FF,
    CAN_CARRYUP_LEFT_ID = 0x205,//�ϰ���������2006���
    CAN_CARRYUP_RIGHT_ID = 0x206,//�ϰ���������2006���
	  CAN_CARRYFORWARD_ID = 0x207,//�ϰ���ǰ��2006���

    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,//��ǰ
    CAN_3508_M2_ID = 0x202,//���
    CAN_3508_M3_ID = 0x203,//��ǰ
    CAN_3508_M4_ID = 0x204,//�Һ�
} can_msg_id_e;

//RM���ͳһ���ݽṹ��
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	
	  //����Ϊ����ԭʼ���ݵ�������
		uint16_t	offset_ecd;
		int32_t		round_cnt;  //���ۿɼ�¼524352Ȧ
		int32_t		total_ecd;  //���ۿɼ�¼4294967295����ֵ
		
		int32_t		total_ecd_out_2006;  //���ۿɼ�¼14565Ȧ
		int32_t		total_ecd_out_3508;  //���ۿɼ�¼27305Ȧ
		int32_t		total_ecd_out_6020;  //���ۿɼ�¼524352Ȧ
	
} motor_measure_t;

typedef struct
{
	uint8_t motor_yaw;
	uint8_t motor_pit;
	uint8_t motor_trigger;
	
	uint8_t motor_user[4];
	uint8_t motor_chassis[4];
	
}motor_feedback_time_t;

extern motor_feedback_time_t motor_feedback_time;

//CAN ���� 0x700��ID�����ݣ�������M3508�����������IDģʽ
extern void CAN_CMD_CHASSIS_RESET_ID(void);
//����0x201-0x204��е�۵����������
extern void CAN_CMD_ARM(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4);
//����0x205-0x208��е�۵����������
extern void CAN_CMD_CARRY(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4);
//���͵��̵����������
extern void CAN_CMD_CHASSIS(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

//���ػ�е������������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Arm_Left_Motor_Measure_Point(void);
//���ػ�е���ҵ��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Arm_Right_Motor_Measure_Point(void);
//���ػ�̨����������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Table_Left_Motor_Measure_Point(void);
//���ػ�̨�ҵ��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Table_Right_Motor_Measure_Point(void);
//���ذ����ϰ���ǰ����������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Carryforward_Motor_Measure_Point(void);
//���ذ����ϰ�����������������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Carryforward_Left_Motor_Measure_Point(void);
//���ذ����ϰ��������ҵ��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Carryforward_Right_Motor_Measure_Point(void);
//����0x201-0x204�����󽮵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_User_Motor_Measure_Point(uint8_t i);
//���ص��̵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i);
extern	motor_measure_t motor_arm_left, motor_arm_right, motor_table_left, motor_table_right, motor_carryforward, motor_carryup_left, motor_carryup_right, motor_chassis[4];

#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
extern void GIMBAL_lose_slove(void);
#endif

#endif
