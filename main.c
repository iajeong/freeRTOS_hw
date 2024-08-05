

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"


/* Local includes. */
#include "console.h" // posix 환경에서 console을 사용하기 위한 헤더파일. printf 대신 사용

// Simulation of the CPU hardware sleeping mode
// Idle task hook, 지우면 안됨
void vApplicationIdleHook( void )
{
    usleep( 15000 );
}

QueueHandle_t xQueue1, xQueue2;
SemaphoreHandle_t xSemaphore = NULL;
TimerHandle_t xTimer = NULL;

const TickType_t xTicksToWait = pdMS_TO_TICKS( 1000 );

// LED 1 제어 태스크
// 1000ms마다 LED1울 켠다.
// queue1에 LED1의 상태(ON/OFF)를 기록
static void vLED1Task( void *pvParameters )
{
	int32_t lValueToSend;
	BaseType_t xStatus;
	lValueToSend = ( int32_t )pvParameters;

	for( ;; )
	{
		vTaskDelay( xTicksToWait );
		xStatus = xQueueSendToBack( xQueue1, &lValueToSend, xTicksToWait );
		if( xStatus == pdPASS )
		{
			console_print( "LED 1 task - LED 1 task가 수행되었습니다. LED 1을 켭니다.\r\n" );
		}
		else
		{
			console_print( "Could not send to the queue1.\r\n" );
		}
	}
}

// 핸들러 태스크
// 세마포어를 받아 LED2를 켠다.
// queue2에 LED2의 상태(ON/OFF)를 기록
static void vHandlerTask( void *pvParameters )
{
	BaseType_t xSemaphoreStatus, xStatus;
	int32_t lValueToSend;
	lValueToSend = ( int32_t )pvParameters;

	for( ;; )
	{
		xSemaphoreStatus = xSemaphoreTake( xSemaphore, xTicksToWait );
		if( xSemaphoreStatus == pdPASS )
		{
			xStatus = xQueueSendToBack( xQueue2, &lValueToSend, 0 );
			console_print( "Handler Task - 세마포어를 성공적으로 가져왔습니다. LED 2를 켭니다.\r\n" );
		}
		else
		{
			console_print( "Handler Task - 세마포어를 가져오지 못했습니다. LED 2를 켜지 못했습니다.\r\n" );
		}
		
	}
}

// LED State 태스크
// 1000ms를 주기로 LED들의 상태를 출력
// LED의 상태 정보는 queue1,2에서 받음
static void LEDStatusTask( void *pvParameters )
{
	int32_t lReceivedValue;
	UBaseType_t uxQueueLength1, uxQueueLength2;
	BaseType_t xStatus1, xStatus2;

	for( ;; )
	{
		uxQueueLength1 = uxQueueMessagesWaiting( xQueue1 );
		uxQueueLength2 = uxQueueMessagesWaiting( xQueue2 );

		console_print( "queue1에 기록된 데이타 수 : %u\r\n", uxQueueLength1 );
		console_print( "queue2에 기록된 데이타 수 : %u\r\n", uxQueueLength2 );

		xStatus1 = xQueueReceive( xQueue1, &lReceivedValue, 0 );
		xStatus2 = xQueueReceive( xQueue2, &lReceivedValue, 0 );
		if ( xStatus1 == pdPASS )
		{
			console_print( "LED 1 State : ON\r\n" );
		}
		else
		{
			console_print( "LED 1 State : OFF\r\n" );
		}

		if ( xStatus2 == pdPASS )
		{
			console_print( "LED 2 State : ON\r\n" );
		}
		else
		{
			console_print( "LED 2 State : OFF\r\n" );
		}

		vTaskDelay( xTicksToWait );
		
	}
	
}

// 소프트웨어 타이머
// 3000ms를 주기로 실행
// 타이머의 콜백 함수에서 세마포어를 give
void vTimerCallback( TimerHandle_t xTimer )
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );
	console_print( "세마포어가 타이머에 의해 이용 가능한 상태가 되었습니다.\r\n " );
}

int main( void )
{
    console_init(); 

	xQueue1 = xQueueCreate( 5, sizeof( int32_t ) );
	xQueue2 = xQueueCreate( 5, sizeof( int32_t ) );
	xSemaphore = xSemaphoreCreateBinary();

	xTaskCreate( vLED1Task, "Turn on LED 1", configMINIMAL_STACK_SIZE, ( void * ) 1, 2, NULL );
	xTaskCreate( LEDStatusTask, "LED Status", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

    if( xSemaphore != NULL )
	{
		xTaskCreate( vHandlerTask, "Handler", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
		xTimer = xTimerCreate( "Timer", pdMS_TO_TICKS( 3000 ), pdTRUE, NULL, vTimerCallback );
		if ( xTimer != NULL )
		{
			xTimerStart( xTimer, 0 );
		}
	}

	vTaskStartScheduler();
	for( ;; );
}