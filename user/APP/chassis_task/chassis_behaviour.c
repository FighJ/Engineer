#include "chassis_behaviour.h"
#include "user_lib.h"
#include "User_Task.h"
#include "pid.h"
#include "CAN_Receive.h"

//��ǰ�����ֵ��ID����Ϊ1 ����ʱ������Ϊ2��3��4
//��ʼ���ٶ� speed_x=ǰ������ speed_y=��ת��ת speed_turn= ��ƽ����ƽ��
int16_t speed_x=0, speed_y=0, speed_turn=0;

//���������ٶ�
float speed[4];

int16_t SPEED_MAX = 1500;

first_order_filter_type_t chassis_cmd_slow_set_vx;
first_order_filter_type_t chassis_cmd_slow_set_vy;

const static fp32 chassis_x_order_filter[1] = {CHASSIS_ACCEL_X_NUM};
		
const static fp32 chassis_y_order_filter[1] = {CHASSIS_ACCEL_Y_NUM};

//��ʼ��������ֵ
int16_t current[4]={0,0,0,0};

//ģʽ��ֵ����
uint8_t MODE_CHASSIS;
	
//���̵��PID��������
static const fp32 PID_WHEEL_SPEED[] = {WHEEL_SPEED_KP, WHEEL_SPEED_KI, WHEEL_SPEED_KD};

//���̵��PID�ṹ��
PidTypeDef pid_wheel_spd[4];

void chassis_init(void)
{
	for(int k=0; k<4; k++)
	{		
		//PID�㷨��ȷ���ĸ����ӵ��ٶ�һ��
		PID_Init(&pid_wheel_spd[k], PID_POSITION, PID_WHEEL_SPEED, WHEEL_SPEED_MAX_OUT, WHEEL_SPEED_MAX_IOUT);
	}
	//��һ���˲�����б����������
  first_order_filter_init(&chassis_cmd_slow_set_vx, CHASSIS_CONTROL_TIME, chassis_x_order_filter);
  first_order_filter_init(&chassis_cmd_slow_set_vy, CHASSIS_CONTROL_TIME, chassis_y_order_filter);
}


void chassis_mode_behaviour(void)
{
	//����
	if (MODE_CHASSIS == CHASSIS_ZERO_FORCE)
	{
		CAN_CMD_CHASSIS(0, 0, 0, 0);
	}
	//ң���������ƶ�
	else if(MODE_CHASSIS == CHASSIS_RC_CONTROL)
	{
		speed_x = -rc_ctrl.rc.ch[0] * RC_SPEED;
		speed_y =  -rc_ctrl.rc.ch[1] * RC_SPEED;
		speed_turn = rc_ctrl.rc.ch[2] * RC_SPEED;
		
		speed[0] = -speed_x - speed_y + speed_turn;
		speed[1] = speed_x - speed_y + speed_turn;
		speed[2] = speed_x + speed_y + speed_turn;
		speed[3] = -speed_x + speed_y + speed_turn;
		
	  current[0] = PID_Calc(&pid_wheel_spd[0], motor_chassis[0].speed_rpm, speed[0]);
		current[1] = PID_Calc(&pid_wheel_spd[1], motor_chassis[1].speed_rpm, speed[1]);
		current[2] = PID_Calc(&pid_wheel_spd[2], motor_chassis[2].speed_rpm, speed[2]);
		current[3] = PID_Calc(&pid_wheel_spd[3], motor_chassis[3].speed_rpm, speed[3]);
		
		CAN_CMD_CHASSIS(current[0], current[1], current[2], current[3]);
	}
	//���̿���
	if (MODE_CHASSIS == CHASSIS_KEY_CONTROL)
	{
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_W)
		  speed_y=-SPEED_MAX;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_A)
			speed_x=SPEED_MAX;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_S)
			speed_y=SPEED_MAX;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_D)
			speed_x=-SPEED_MAX;
		
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A)
		{
			speed_y=-SPEED_MAX;speed_x=SPEED_MAX;
		}
		if( rc_ctrl.key.v == 9)
		{
			speed_y=-SPEED_MAX;speed_x=-SPEED_MAX;
		}
		if( rc_ctrl.key.v == 6)
		{
			speed_y=SPEED_MAX;speed_x=SPEED_MAX;
		}
		if( rc_ctrl.key.v == 10)
		{
			speed_y=SPEED_MAX;speed_x=-SPEED_MAX;
		}
		if( rc_ctrl.key.v == 0)
		{
			speed_x=0;
			speed_y=0;
		}
		
		speed_turn = rc_ctrl.mouse.x * 48;
		
		if(speed_turn >= 2000)
		{
			speed_turn = 2000;
			
		}
		else if(speed_turn <= -2000)
		{
			speed_turn = -2000 ;
		}
		
		if(rc_ctrl.key.v == KEY_PRESSED_OFFSET_Q)
		{
			speed_turn = -2000;
		}
		if(rc_ctrl.key.v == KEY_PRESSED_OFFSET_E)
		{
			speed_turn = 2000;
		}
		
		if(rc_ctrl.mouse.press_r)
		{
			speed_x = 0.5f * speed_x;
			speed_y = 0.5f * speed_y;
			speed_turn = 0.5f * speed_turn;
		}
		else
		{
			speed_x = 2.0f * speed_x;
			speed_y = 2.0f * speed_y;
			speed_turn = 2.0f * speed_turn;
		}
		
		//һ�׵�ͨ�˲�����б����Ϊ�����ٶ�����
    first_order_filter_cali(&chassis_cmd_slow_set_vx, speed_x);
    first_order_filter_cali(&chassis_cmd_slow_set_vy, speed_y);
		
		speed[0] = -chassis_cmd_slow_set_vx.out - chassis_cmd_slow_set_vy.out + speed_turn;
		speed[1] = chassis_cmd_slow_set_vx.out - chassis_cmd_slow_set_vy.out + speed_turn;
		speed[2] = chassis_cmd_slow_set_vx.out + chassis_cmd_slow_set_vy.out + speed_turn;
		speed[3] = -chassis_cmd_slow_set_vx.out + chassis_cmd_slow_set_vy.out + speed_turn;
		
		current[0] = PID_Calc(&pid_wheel_spd[0], motor_chassis[0].speed_rpm, speed[0]);
		current[1] = PID_Calc(&pid_wheel_spd[1], motor_chassis[1].speed_rpm, speed[1]);
		current[2] = PID_Calc(&pid_wheel_spd[2], motor_chassis[2].speed_rpm, speed[2]);
		current[3] = PID_Calc(&pid_wheel_spd[3], motor_chassis[3].speed_rpm, speed[3]);
	}
	CAN_CMD_CHASSIS(current[0], current[1], current[2], current[3]);
}
