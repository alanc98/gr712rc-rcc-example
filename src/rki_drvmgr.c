/****************************************************************
**  rtems_drvmgr.c
**
**  Author:  Alan Cudmore
**
**  This module is responsible for RTEMS drvmgr initialization.
**  It is based on the rcc 1.3 examples
**
*/

#include "rtems_config.h"

#include <stdlib.h>

/* LEON3 specific system init */
void system_init2(void)
{
   #ifndef RTEMS_DRVMGR_STARTUP
      /* Register GRLIB root bus */
      ambapp_grlib_root_register(&grlib_bus_config);
   #endif
}

void system_init(void)
{
    /* CPU/SYSTEM specific Init */
    system_init2();

    #ifndef RTEMS_DRVMGR_STARTUP
      /* Initializing Driver Manager if not alread performed by BSP */
      printf("RTEMS_DRVMGR_STARTUP is NOT DEFINED - initializing driver manager manually\n");
      if ( drvmgr_init() ) {
          printf("Driver manager Failed to initialize\n");
          exit(-1);
      }
    #endif

    /* Print Driver manager drivers and their assigned devices
    * drvmgr_summary();
    * drvmgr_info_drvs(OPTION_INFO_ALL);
   */
}

/*
** rki_init_drvmgr
*/
int rki_init_drvmgr(void)
{
   printf("rki_init_drvmgr: system_init.\n");
   system_init();

   return(0);
}

