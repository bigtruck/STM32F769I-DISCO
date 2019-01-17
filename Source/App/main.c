#include "stm32f7xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "bsp.h"

#include "../lvgl/lvgl/lvgl.h"


#define task_PRIORITY_LED_BLINK 5
#define task_PRIORITY_GUI       6


static void prvLEDBlink( void *pvParameters );
static void prvGUI( void *pvParameters ) ;

int main( void )
{
  bsp_init() ;
  
  xTaskCreate( prvLEDBlink,                     /* The function that implements the task. */
               "Blink LED",                     /* The text name assigned to the task - for debug only as it is not used by the kernel. */
               configMINIMAL_STACK_SIZE,        /* The size of the stack to allocate to the task. */
               NULL,                            /* The parameter passed to the task - not used in this case. */
               task_PRIORITY_LED_BLINK,         /* The priority assigned to the task. */
               NULL );                          /* The task handle is not required, so NULL is passed. */

  xTaskCreate( prvGUI,
               "LittlevGL",
               4096,
               NULL,
               task_PRIORITY_GUI,
               NULL );
  
  
  
  /* Start the tasks and timer running. */
  vTaskStartScheduler();
  
  while(1);
}


static void prvLEDBlink( void *pvParameters )
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;
  
  __GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init( GPIOJ, &GPIO_InitStruct );

  for( ;; )
  {
    GPIOJ->ODR ^= GPIO_PIN_13;
    vTaskDelay(200);
  }
}

extern void tft_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
extern bool touchpad_read(lv_indev_data_t *data);
extern void tft_copy2fb(void);
static void prvGUI( void *pvParameters )
{
  lv_disp_drv_t disp_drv;
  lv_indev_drv_t indev_drv;
  
  lv_init();

  lv_indev_drv_init(&indev_drv);
  indev_drv.read = touchpad_read;
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);
  
  lv_disp_drv_init(&disp_drv);
  disp_drv.disp_flush = tft_flush;
  lv_disp_drv_register(&disp_drv);
  
  lv_theme_t *th = lv_theme_alien_init(21, NULL);
  lv_test_theme_1(th);
  //lv_test_object_1();
  //sysmon_create();
  //lv_test_group_1();
  for(;;)
  {
    lv_task_handler();
//    tft_copy2fb();
    vTaskDelay(10);
  }
}



void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeHeapSpace;

	/* This is just a trivial example of an idle hook.  It is called on each
	cycle of the idle task.  It must *NOT* attempt to block.  In this case the
	idle task just queries the amount of FreeRTOS heap that remains.  See the
	memory management section on the http://www.FreeRTOS.org web site for memory
	management options.  If there is a lot of heap memory free then the
	configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	RAM. */
	xFreeHeapSpace = xPortGetFreeHeapSize();

	/* Remove compiler warning about xFreeHeapSpace being set but never used. */
	( void ) xFreeHeapSpace;
}
/*-----------------------------------------------------------*/

void vAssertCalled( uint32_t ulLine, const char *pcFile )
{
volatile unsigned long ul = 0;

	( void ) pcFile;
	( void ) ulLine;

	taskENTER_CRITICAL();
	{
		/* Set ul to a non-zero value using the debugger to step out of this
		function. */
		while( ul == 0 )
		{
			__NOP();
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
}