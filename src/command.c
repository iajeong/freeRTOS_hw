#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <event_groups.h>

#include <console.h>
#include <command.h>
#include <error.h>

// error_t help_command( void )
// {

// }

// error_t time_command( void )
// {

// }

// error_t task_command( void )
// {

// }

// void start_view_task( void *pvParameters )
// {

// }

// void echo_task( void * pvParameters )
// {

// }

// void event_task1( void *pvParameters )
// {

// }

// void event_task2( void *pvParameters )
// {

// }

// void event_task3( void *pvParameters )
// {

// }

// void event_task4( void *pvParameters )
// {

// }

// void event_create_task( void *pvParameters )
// {

// }

// error_t event_command( void )
// {

// }

// error_t start_view_command( void )
// {

// }

// void execute_command( const char *command )
// {
//     error_t error;

//     if ( strcmp( command, "help") == 0 )
//     {
//         error = help_command();
//         console_prompt();
//     }
//     else if ( strcmp( command, "time") == 0 )
//     {
//         error = time_command();
//         console_prompt();
//     }
//     else if ( strcmp( command, "task") == 0 )
//     {
//         error = task_command();
//         console_prompt();
//     }
//     else if ( strcmp( command, "event") == 0 )
//     {
//         error = event_command();
//         console_prompt();
//     }
//     else if ( strcmp( command, "echo") == 0 )
//     {
//         error = echo_command();
//         console_prompt();
//     }
//     else if ( strcmp( command, "start_view") == 0 )
//     {
//         error = start_view_command();
//         console_prompt();
//     }
//     else
//     {
//         console_printf( "ERROR : 등록된 커맨드가 아닙니다.\r\n" );
//         console_prompt();
//     }

//     if ( error != OK )
//     {
//         console_printf( "ERROR CODE : %d\r\n", error );
//         console_prompt();
//     }
// }