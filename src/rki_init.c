/*
 * RTEMS configuration/initialization
 *
 * Based on code by W. Eric Norum and others
 *
 * This program may be distributed and used for any purpose.
 * I ask only that you:
 *	1. Leave this author information intact.
 *	2. Document any changes you make.
 *
 * W. Eric Norum
 * Saskatchewan Accelerator Laboratory
 * University of Saskatchewan
 * Saskatoon, Saskatchewan, CANADA
 * eric@skatter.usask.ca
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/console.h>
#include <rtems/version.h>
#include <rtems/untar.h>

#define RKI_CONSOLE_BAUD_RATE B57600

/*
** External functions
*/
int  rki_init_drvmgr(void);
void init_gpio(void);
int  demo_start(void);
void service_watchdog();

/*
** RTEMS Startup Task
*/
rtems_task Init (rtems_task_argument ignored)
{
   int      status;
   int      console_fd;
   struct   termios tty;

   rtems_termios_bufsize(256, 4096, 4096);

   /*
   ** Force the Console baud rate setting to override the DRVMGR
   ** Setting that inherits the console baud rate from GRMON or the Bootloader
   */
   console_fd = open( CONSOLE_DEVICE_NAME, O_WRONLY, 0 );
   if (console_fd >= 0)
   {
      printf("Opened Console FD: %d\n",console_fd);
      if (tcgetattr(console_fd, &tty) < 0)
      {
         printf("Error from tcgetattr: %s\n", strerror(errno));
      }
      else
      {
         cfsetospeed(&tty, (speed_t)RKI_CONSOLE_BAUD_RATE);
         cfsetispeed(&tty, (speed_t)RKI_CONSOLE_BAUD_RATE);
         if (tcsetattr(console_fd, TCSANOW, &tty) != 0)
         {
            printf("RKI: Error from tcsetattr: %s\n", strerror(errno));
         }
      }
      close(console_fd);
   }
   else
   {
      printf("Could not open Console FD: %d\n",console_fd);
   }

   printf("\n\n");
   printf("RTEMS Kernel Image Booting\n");

   printf( "\n\n*** RTEMS Info ***\n" );
   printf("%s\n\n", rtems_version() );
   printf("\n");
   printf(" BSP Ticks Per Second = %d\n",(int)rtems_clock_get_ticks_per_second());
   printf( "*** End RTEMS info ***\n\n" );

   /* Driver Manager */
   status = rki_init_drvmgr();

   /* Uncomment to start 3 demo tasks */
#if 1
   /* Start demo tasks */
   demo_start();
#endif

   /* Setup GPIO */ 
   init_gpio();

   /* Feed local watchdog */
   while (1)
    {
        service_watchdog();
        sleep(1);
    } 
   /*
   ** Delete the init task
   ** -- Note, the shell init does not return so the following is not called  
   */
   printf("Ending the RTEMS Init task.\n");
   status = rtems_task_delete( RTEMS_SELF );    /* should not return */
   printf( "rtems_task_delete returned with status of %d.\n", status );
   exit( 1 );
}
