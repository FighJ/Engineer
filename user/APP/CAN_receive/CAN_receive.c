#include "CAN_Receive.h"
#include "stm32f4xx.h"

//���̵�����ݶ�ȡ
#define get_motor_measure_init(ptr, rx_message)                                                \
		{                                                                                          \
			(ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);             \
			(ptr)->offset_ecd = (ptr)->ecd;                                                          \
		}

#define get_motor_measure(ptr, rx_message)                                                                   \
    {                                                                                                        \
        (ptr)->last_ecd = (ptr)->ecd;                                                                        \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);                         \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);                   \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]);               \
        (ptr)->temperate = (rx_message)->Data[6];                                                            \
			                                                                                                       \
				/*����Ϊ����ԭʼ���ݵ�����*/																													                 \
																																														                 \
				if((ptr)->ecd - (ptr)->last_ecd > 4096)                                                              \
					(ptr)->round_cnt --;                                                                               \
				else if ((ptr)->ecd - (ptr)->last_ecd < -4096)                                                       \
					(ptr)->round_cnt ++;                                                                               \
				(ptr)->total_ecd = (ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd;                         \
																																															               \
				(ptr)->total_ecd_out_2006 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) / 36.f;             \
				(ptr)->total_ecd_out_3508 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) * 187.f / 3591.f;   \
				(ptr)->total_ecd_out_6020 = (ptr)->total_ecd;           																		 							 \
   }
		
//����0x201-0x204�û��󽮵�����ݶ�ȡ
#define get_user_motor_measure_init(ptr, rx_message)                                           \
		{                                                                                          \
			(ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);             \
			(ptr)->offset_ecd = (ptr)->ecd;                                                          \
		}

#define get_user_motor_measure(ptr, rx_message)                                                              \
    {                                                                                                        \
        (ptr)->last_ecd = (ptr)->ecd;                                                                        \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);                         \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);                   \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]);               \
        (ptr)->temperate = (rx_message)->Data[6];                                                            \
			                                                                                                       \
				/*����Ϊ����ԭʼ���ݵ�����*/																													                 \
																																														                 \
				if((ptr)->ecd - (ptr)->last_ecd > 4096)                                                              \
					(ptr)->round_cnt --;                                                                               \
				else if ((ptr)->ecd - (ptr)->last_ecd < -4096)                                                       \
					(ptr)->round_cnt ++;                                                                               \
				(ptr)->total_ecd = (ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd;                         \
																																															               \
				(ptr)->total_ecd_out_2006 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) / 36.f;             \
				(ptr)->total_ecd_out_3508 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) * 187.f / 3591.f;   \
				(ptr)->total_ecd_out_6020 = (ptr)->total_ecd;           																		 							 \
   }

//��̨������ݶ�ȡ
#define get_gimbal_motor_measure_init(ptr, rx_message)                                         \
		{                                                                                          \
			(ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);             \
			(ptr)->offset_ecd = (ptr)->ecd;                                                          \
		}

#define get_gimbal_motor_measure(ptr, rx_message)                                                            \
    {                                                                                                        \
        (ptr)->last_ecd = (ptr)->ecd;                                                                        \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);                         \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);                   \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]);               \
        (ptr)->temperate = (rx_message)->Data[6];                                                            \
			                                                                                                       \
				/*����Ϊ����ԭʼ���ݵ�����*/																													                 \
																																														                 \
				if((ptr)->ecd - (ptr)->last_ecd > 4096)                                                              \
					(ptr)->round_cnt --;                                                                               \
				else if ((ptr)->ecd - (ptr)->last_ecd < -4096)                                                       \
					(ptr)->round_cnt ++;                                                                               \
				(ptr)->total_ecd = (ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd;                         \
																																															               \
				(ptr)->total_ecd_out_2006 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) / 36.f;             \
				(ptr)->total_ecd_out_3508 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) * 187.f / 3591.f;   \
				(ptr)->total_ecd_out_6020 = (ptr)->total_ecd;           																		 							 \
    }
		
//ͳһ����can���պ���
static void CAN1_hook(CanRxMsg *rx_message);
static void CAN2_hook(CanRxMsg *rx_message);
		
//�����������
motor_measure_t motor_arm_left, motor_arm_right, motor_table_left, motor_table_right, motor_carryforward, motor_carryup_left, motor_carryup_right, motor_chassis[4];

//can1�ж�
CanRxMsg rx1_message;
void CAN1_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx1_message);
        CAN1_hook(&rx1_message);
    }
}

//can2�ж�
CanRxMsg rx2_message;
void CAN2_RX1_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
        CAN_Receive(CAN2, CAN_FIFO1, &rx2_message);
        CAN2_hook(&rx2_message);
    }
}

//����0x201-0x204��е�۵����������
void CAN_CMD_ARM(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_ARM_ALL_ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = User_Motor1 >> 8;
    TxMessage.Data[1] = User_Motor1;
    TxMessage.Data[2] = User_Motor2 >> 8;
    TxMessage.Data[3] = User_Motor2;
	  TxMessage.Data[4] = User_Motor3 >> 8;
    TxMessage.Data[5] = User_Motor3;
    TxMessage.Data[6] = User_Motor4 >> 8;
    TxMessage.Data[7] = User_Motor4;

    CAN_Transmit(CAN1, &TxMessage);
}

//����0x205-0x208��е�۵����������
void CAN_CMD_CARRY(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_CARRY_ALL_ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = User_Motor1 >> 8;
    TxMessage.Data[1] = User_Motor1;
    TxMessage.Data[2] = User_Motor2 >> 8;
    TxMessage.Data[3] = User_Motor2;
	  TxMessage.Data[4] = User_Motor3 >> 8;
    TxMessage.Data[5] = User_Motor3;
    TxMessage.Data[6] = User_Motor4 >> 8;
    TxMessage.Data[7] = User_Motor4;

    CAN_Transmit(CAN1, &TxMessage);
}

//CAN ���� 0x700��ID�����ݣ�������M3508�����������IDģʽ
void CAN_CMD_CHASSIS_RESET_ID(void)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = 0x700;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = 0;
    TxMessage.Data[1] = 0;
    TxMessage.Data[2] = 0;
    TxMessage.Data[3] = 0;
    TxMessage.Data[4] = 0;
    TxMessage.Data[5] = 0;
    TxMessage.Data[6] = 0;
    TxMessage.Data[7] = 0;

    CAN_Transmit(CAN2, &TxMessage);
}

//���͵��̵����������
void CAN_CMD_CHASSIS(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_CHASSIS_ALL_ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = motor1 >> 8;
    TxMessage.Data[1] = motor1;
    TxMessage.Data[2] = motor2 >> 8;
    TxMessage.Data[3] = motor2;
    TxMessage.Data[4] = motor3 >> 8;
    TxMessage.Data[5] = motor3;
    TxMessage.Data[6] = motor4 >> 8;
    TxMessage.Data[7] = motor4;

    CAN_Transmit(CHASSIS_CAN, &TxMessage);
}

//���ػ�е������������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Arm_Left_Motor_Measure_Point(void)
{
    return &motor_arm_left;
}
//���ػ�е���ҵ��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Arm_Right_Motor_Measure_Point(void)
{
    return &motor_arm_right;
}
//���ػ�̨����������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Table_Left_Motor_Measure_Point(void)
{
    return &motor_table_left;
}
//���ػ�̨�ҵ��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Table_Right_Motor_Measure_Point(void)
{
    return &motor_table_right;
}
//���ذ����ϰ���ǰ����������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Carryforward_Motor_Measure_Point(void)
{
    return &motor_carryforward;
}
//���ذ����ϰ�����������������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Carryforward_Left_Motor_Measure_Point(void)
{
    return &motor_carryup_left;
}
//���ذ����ϰ��������ҵ��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Carryforward_Right_Motor_Measure_Point(void)
{
    return &motor_carryup_right;
}
//���ص��̵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}

motor_feedback_time_t motor_feedback_time;

//����can1�жϺ��������Ҽ�¼�������ݵ�ʱ�䣬��Ϊ�����ж�����
static void CAN1_hook(CanRxMsg *rx_message)
{
    switch (rx_message->StdId)
    {
			case CAN_ARM_LEFT_ID:
			{
					//���������ݺ꺯��
					get_motor_measure(&motor_arm_left, rx_message);
					break;
			}
			case CAN_ARM_RIGHT_ID:
			{
					//���������ݺ꺯��
					get_motor_measure(&motor_arm_right, rx_message);
					break;
			}
			case CAN_TABLE_LEFT_ID:
			{
					//���������ݺ꺯��
					get_motor_measure(&motor_table_left, rx_message);
					break;
			}
			case CAN_TABLE_RIGHT_ID:
			{
					//���������ݺ꺯��
					get_motor_measure(&motor_table_right, rx_message);
					break;
			}
						case CAN_CARRYUP_LEFT_ID:
			{
					//���������ݺ꺯��
					get_motor_measure(&motor_carryup_left, rx_message);
					break;
			}
						case CAN_CARRYUP_RIGHT_ID:
			{
					//���������ݺ꺯��
					get_motor_measure(&motor_carryup_right, rx_message);
					break;
			}
			case CAN_CARRYFORWARD_ID:
			{
					//���������ݺ꺯��
					get_motor_measure(&motor_carryforward, rx_message);
					break;
			}

			default:
			{
					break;
			}
    }
}

static void CAN2_hook(CanRxMsg *rx_message)
{
	  switch (rx_message->StdId)
    {	
			case CAN_3508_M1_ID:
			case CAN_3508_M2_ID:
			case CAN_3508_M3_ID:
			case CAN_3508_M4_ID:
			{
					static uint8_t i = 0;
					//������ID��
					i = rx_message->StdId - CAN_3508_M1_ID;
					if(motor_feedback_time.motor_chassis[i] == 0)
					{
						get_motor_measure_init(&motor_chassis[i], rx_message);
						motor_feedback_time.motor_chassis[i] = 1;
					}
					//���������ݺ꺯��
					get_motor_measure(&motor_chassis[i], rx_message);
					break;
			}
			
			default:
			{
					break;
			}
    }
	
}
