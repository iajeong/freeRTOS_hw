

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Local includes. */
#include "console.h" // posix 환경에서 console을 사용하기 위한 헤더파일. printf 대신 사용

// Simulation of the CPU hardware sleeping mode
// Idle task hook, 지우면 안됨
void vApplicationIdleHook( void )
{
    usleep( 15000 );
}

// HW1_1의 Task 1 정의
// void vTask1_1( void *pvParameters )
// {
// 	const char *pcTaskName = "Task 1: 1 sec period\r\n";

// 	for ( ;; )
// 	{
// 		console_print( pcTaskName );
// 		vTaskDelay( 1000 );
// 	}
// }
// HW1_1의 Task 2 정의
// void vTask1_2( void *pvParameters )
// {
// 	const char *pcTaskName = "Task 2: 2 sec period\r\n";

// 	for ( ;; )
// 	{
// 		console_print( pcTaskName );
// 		vTaskDelay( 2000 );
// 	}
// }

TaskHandle_t xHandle;

// HW1_2의 Task 1 정의
void vTask2_1( void *pvParameters )
{
	const char *pcTaskName = "zzzzzzz\r\n";

	for( ;; )
	{
		console_print( pcTaskName );
        vTaskDelay( 1000 );
	}
}
// HW1_2의 Task 2 정의
void vTask2_2( void *pvParameters )
{
	const char *pcTaskName = "알람! 알람! 알람!\r\n";

	for( ;; )
	{
		console_print( pcTaskName );
		vTaskDelay( 5000 );
        vTaskSuspend( xHandle );
		//vTaskDelay( 1000 );
		vTaskResume( xHandle );
		//vTaskDelay( 1000 );
	}
}
// HW1_2의 Task 3 정의
void vTask2_3( void *pvParameters )
{
	const char *pcTaskName = "5분만..\r\n";

	for ( ;; )
	{
		console_print( pcTaskName );
		vTaskDelay( 5000 );
	}
}

int main( void )
{
    console_init(); 

	/* xTaskCreate(Pointer to the function that implement the task, text name, stack depth, task parameter, task priority, task handle)*/
	// xTaskCreate( vTask1_1, "Task 1_1", configMINIMAL_STACK_SIZE, NULL, 0, NULL );
	// xTaskCreate( vTask1_2, "Task 1_2", configMINIMAL_STACK_SIZE, NULL, 0, NULL );
	xTaskCreate( vTask2_1, "Task 2_1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate( vTask2_2, "Task 2_2", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
	xTaskCreate( vTask2_3, "Task 2_3", configMINIMAL_STACK_SIZE, NULL, 2, &xHandle );
    
	vTaskStartScheduler();
	for( ;; );
}