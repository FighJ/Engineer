#ifndef GIMBAL_BEHAVIOUR_SIMPLE_H
#define GIMBAL_BEHAVIOUR_SIMPLE_H

#include "main.h"
#include "math.h"
#include "stm32f4xx.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pid.h"
#include "Remote_Control.h"
#include "CAN_Receive.h"

//PITCH �ٶȻ�PID���� ������ �����޷�
#define PITCH_SPEED_KP 1.4f
#define PITCH_SPEED_KI 2.0f
#define PITCH_SPEED_KD 0.1f
#define PITCH_SPEED_MAX_OUT 30000.0f
#define PITCH_SPEED_MAX_IOUT 4000.0f
//YAW �ٶȻ�PID���� ������ �����޷�
#define YAW_SPEED_KP 1.2f
#define YAW_SPEED_KI 1.0f
#define YAW_SPEED_KD 0.5f
#define YAW_SPEED_MAX_OUT 30000.0f
#define YAW_SPEED_MAX_IOUT 4000.0f
//PITCH �ǶȻ�PID���� ������ �����޷�
#define PITCH_ANGLE_KP 100.0f
#define PITCH_ANGLE_KI 1.1f
#define PITCH_ANGLE_KD 50.0f
#define PITCH_ANGLE_MAX_OUT 10000.0f
#define PITCH_ANGLE_MAX_IOUT 4000.0f
//YAW �ǶȻ�PID���� ������ �����޷�
#define YAW_ANGLE_KP 300.0f
#define YAW_ANGLE_KI 2.1f
#define YAW_ANGLE_KD 50.0f
#define YAW_ANGLE_MAX_OUT 10000.0f
#define YAW_ANGLE_MAX_IOUT 4000.0f

//��̨�ƶ��ٶ�
#define pitch_rc_speed 0.8f
#define yaw_rc_speed 2.0f
//��̨ת�����Ƕ�
#define PITCH_MAX_ANGLE 400
#define YAW_MAX_ANGLE 1000

//�������ֵ����Լ���ֵ
#define Half_ecd_range 4096
#define ecd_range 8191
//����ǶȾ������
#define PITCH_Zero 4096
#define YAW_Zero 4096
//����Ƿ�װ
#define PITCH_IF_TURN 0
#define YAW_IF_TURN 0
//��װ
#define TURN 1
//δ��װ
#define UN_TURN 0

//��̨У׼ʱ��
#define Check_Max_Time 1000

typedef enum
{
  GIMBAL_ZERO_FORCE = 0, //��̨����
  GIMBAL_RC_CONTROL,  //ң������PC����
	GIMBAL_RC_AUTO,
} Gimbal_Mode;

typedef enum
{
  UP = 0,
  MID,  
	DOWN,
} Gimbal_check_Mode;//�󲦸���̨ģʽ

extern const RC_ctrl_t *GIMBAL_RC;
extern const RC_ctrl_t *LAST_GIMBAL_RC;
//��̨���CAN���͵���ֵ
extern int16_t current_pitch;
extern int16_t current_yaw;

//��̨У׼��ʼ��
extern int16_t Check_Time;

//ƫ����̨ƫ���Ǽ���
extern fp32 INS_YAW;
extern fp32 GIMBAL_INS_YAW;
extern fp32 GIMBAL_INS_YAW_TEMP;

extern const motor_measure_t *moto_yaw;

//��̨PID��ʼ��
extern void gimbal_init(void);
//�жϵ���Ƿ�װ
extern void gimbal_turn_check(int16_t PITCH, int16_t YAW);
//��ͬģʽ����̨��Ϊ
extern void gimbal_mode_behaviour(void);
//����󲦸���̨״̬
extern void gimbal_mode_check(void);
extern void mode_change(void);
//��̨�����������
extern void CAN_CMD_GIMBAL_SIMPLE(int16_t PITCH, int16_t YAW);
	
#endif
