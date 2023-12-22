/* Create 3 demo tasks that compete for a semaphore */

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <rtems.h>
#include <bsp.h>

#define TASK_ATTRIBUTES RTEMS_FLOATING_POINT | RTEMS_LOCAL

rtems_id   DEMO_Task1_id;
rtems_name DEMO_Task1_name;

rtems_id   DEMO_Task2_id;
rtems_name DEMO_Task2_name;

rtems_id   DEMO_Task3_id;
rtems_name DEMO_Task3_name;

rtems_name DEMO_sem_name;
rtems_id   DEMO_sem_id;

rtems_task demo_task1(
  rtems_task_argument unused
)
{
  rtems_status_code status;
  int i = 0; 
  printf("*** DEMO TASK 1 STARTING ***\n");
  
  while(1)
  {
     i++;
     status = rtems_semaphore_obtain(DEMO_sem_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
     if ( status != RTEMS_SUCCESSFUL )
             printf("Demo Task 1 Failed to obtain semaphore: %d\n",status);
      // if ((i % 2000) == 0) printf("DEMO Task 1 here!\n");
      //status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() / 10000 );
      status = rtems_task_wake_after( 10 );
      status = rtems_semaphore_release(DEMO_sem_id);
  }
  printf( "*** DEMO TASK 1 FINISHED ***\n");
  rtems_task_delete( RTEMS_SELF );    /* should not return */
}

rtems_task demo_task2(
  rtems_task_argument unused
)
{
  rtems_status_code status;
  int i = 0; 
  printf("*** DEMO TASK 2 STARTING ***\n");
  while(1)
  {
     i++;
     status = rtems_semaphore_obtain(DEMO_sem_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
     if ( status != RTEMS_SUCCESSFUL )
             printf("Demo Task 2 Failed to obtain semaphore: %d\n",status);
      // if ((i % 2500) == 0) printf("DEMO Task 2 here!\n");
      // status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() / 5000 );
      status = rtems_task_wake_after( 10 );
      status = rtems_semaphore_release(DEMO_sem_id);
  }
  printf( "*** DEMO TASK 2 FINISHED ***\n");
  rtems_task_delete( RTEMS_SELF );    /* should not return */
}

rtems_task demo_task3(
  rtems_task_argument unused
)
{
  rtems_status_code status;
  int i = 0; 
  printf("*** DEMO TASK 3 STARTING ***\n");
  while(1)
  {
     i++;
     status = rtems_semaphore_obtain(DEMO_sem_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
     if ( status != RTEMS_SUCCESSFUL )
             printf("Demo Task 3 Failed to obtain semaphore: %d\n",status);
      // if ((i % 5000) == 0) printf("DEMO Task 3 here!\n");
      // status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() / 9000 );
      status = rtems_task_wake_after( 10 );
      status = rtems_semaphore_release(DEMO_sem_id);
  }
  printf( "*** DEMO TASK 3 FINISHED ***\n");
  rtems_task_delete( RTEMS_SELF );    /* should not return */
}

/*
** start_demo_tasks
*/
int demo_start(void)
{
   rtems_status_code status;

   DEMO_Task1_name = rtems_build_name( 'D', 'M', 'T', '1' );
   DEMO_Task2_name = rtems_build_name( 'D', 'M', 'T', '2' );
   DEMO_Task3_name = rtems_build_name( 'D', 'M', 'T', '3' );
   DEMO_sem_name = rtems_build_name( 'D', 'M', 'S', 'M' );

   status = rtems_semaphore_create(DEMO_sem_name,0, RTEMS_LOCAL,RTEMS_PRIORITY, &DEMO_sem_id);
   if ( status != RTEMS_SUCCESSFUL ) {
        printf("Failed to create semaphore, status: %d\n", status);
        exit(-1);
   }

  status = rtems_task_create(
    DEMO_Task1_name, 200, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_NO_ASR,
    TASK_ATTRIBUTES, &DEMO_Task1_id
  );
  if ( status != RTEMS_SUCCESSFUL )
     printf("Error creating DEMO Task 1\n");
  else
     printf("DEMO Task 1 created!\n");

  status = rtems_task_create(
    DEMO_Task2_name, 220, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_NO_ASR,
    TASK_ATTRIBUTES, &DEMO_Task2_id
  );
  if ( status != RTEMS_SUCCESSFUL )
     printf("Error creating DEMO Task 2\n");
  else
     printf("DEMO Task 2 created!\n");

   status = rtems_task_create(
    DEMO_Task3_name, 230, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_NO_ASR,
    TASK_ATTRIBUTES, &DEMO_Task3_id
  );
  if ( status != RTEMS_SUCCESSFUL )
     printf("Error creating DEMO Task 3\n");
  else
     printf("DEMO Task 3 created!\n");

  status = rtems_task_start(DEMO_Task1_id, demo_task1, 0);
  if ( status != RTEMS_SUCCESSFUL )
  {
     printf("Error Starting DEMO Task 1!\n");
  }

  status = rtems_task_start(DEMO_Task2_id, demo_task2, 0);
  if ( status != RTEMS_SUCCESSFUL )
  {
     printf("Error Starting DEMO Task 2!\n");
  }

  status = rtems_task_start(DEMO_Task3_id, demo_task3, 0);
  if ( status != RTEMS_SUCCESSFUL )
  {
     printf("Error Starting DEMO Task 3!\n");
  }

   status = rtems_semaphore_release(DEMO_sem_id);

   printf("Returning from DEMO startup\n");
   return(0);
}

