/*
** rtems_config.h
**
**  Author:  Alan Cudmore
**
**  This contains the configuration settings for an RTEMS Application
**
*/

/*
** RTEMS Includes
*/
#include <rtems.h>
#include <bsp.h>

#include <rtems/console.h>
#include <bsp/irq-info.h>

#include <drvmgr/drvmgr.h>
#include <grlib/ambapp_bus_grlib.h>
#include <grlib/ambapp_bus.h>
#include <grlib/ambapp_ids.h>

/*
** RTEMS OS Configuration defintions
*/

#define TASK_INTLEVEL 0
#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES	(RTEMS_FLOATING_POINT | RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_ASR | RTEMS_INTERRUPT_LEVEL(TASK_INTLEVEL))
#define CONFIGURE_INIT_TASK_STACK_SIZE	  (8*1024)
#define CONFIGURE_INTERRUPT_STACK_SIZE    (8*1024) 
#define CONFIGURE_MINIMUM_TASK_STACK_SIZE (8*1024) 

#define CONFIGURE_INIT_TASK_PRIORITY	   120

#define CONFIGURE_MAXIMUM_TASKS                        10 
#define CONFIGURE_MAXIMUM_TIMERS                        5
#define CONFIGURE_MAXIMUM_SEMAPHORES                   16 
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES               16 

#define CONFIGURE_EXECUTIVE_RAM_SIZE	( 128 * 1024 )

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_DRIVERS                     32 
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS           200

/* #define CONFIGURE_MICROSECONDS_PER_TICK              10000 */ /* 100 Hz */
/* #define CONFIGURE_MICROSECONDS_PER_TICK              1000  */ /* 1000 Hz */
#define CONFIGURE_MICROSECONDS_PER_TICK              100   /* 10000 Hz */

#define CONFIGURE_STACK_CHECKER_ENABLED

/*
** Init task prototype
*/
rtems_task Init (rtems_task_argument argument);

/* Configure Driver manager */
#define CONFIGURE_DRIVER_AMBAPP_GAISLER_APBUART
#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GPTIMER
#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GRGPIO  /* GRGPIO driver */

/*
** This include file must be AFTER the
** configuration data.
*/

#include <rtems/confdefs.h>
#include <drvmgr/drvmgr.h>
#include <drvmgr/drvmgr_confdefs.h>

