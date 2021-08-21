#include "User_Task.h"
#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "led.h"
#include "Remote_Control.h"

#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t UserTaskStack;
#endif

void UserTask(void *pvParameters)
{
    while (1)
    {
			//������ɫLED��500ms
		  	led_green_on();
        vTaskDelay(500);
			//������ɫLED��500ms
        led_green_off();
        vTaskDelay(500);
			
			//����һ��ʱ��
      vTaskDelay(1);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        UserTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
