/* Service GR712RC watchdog */
/*
**   Include Files:
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <rtems.h>

/*
** global data
*/

/*-----------------------------------------------------------------------*/
#define WATCHDOG_MAX (60*1000)
uint32_t WatchdogValue = WATCHDOG_MAX;

#define DLEON_REG_START_ADDRESS             0x80000000
#define DLEON_REG_END_ADDRESS               0x801FFFFF

#define DLEON_READ_REG(offset)        *((volatile uint32_t *)(DLEON_REG_START_ADDRESS + (offset)))
#define DLEON_WRITE_REG(offset,data)  *((volatile uint32_t *)(DLEON_REG_START_ADDRESS + (offset))) = (data)

#define DLEON_TIMER_DBG_HALT_FLD    0x00000040
#define DLEON_TIMER_CHAIN_FLD       0x00000020
#define DLEON_TIMER_INT_PEND_FLD    0x00000010
#define DLEON_TIMER_INT_ENA_FLD     0x00000008
#define DLEON_TIMER_LOAD_FLD        0x00000004
#define DLEON_TIMER_RESTART_FLD     0x00000002
#define DLEON_TIMER_ENABLE_FLD      0x00000001

#define DLEON_TIMER4_VALUE_REGISTER         0x340    /*  Offset from DLEON base address to Timer 4 Value Register */
#define DLEON_TIMER4_RELOAD_REGISTER        0x344    /*  Offset from DLEON base address to Timer 4 Reload Register */
#define DLEON_TIMER4_CONTROL_REGISTER       0x348    /*  Offset from DLEON base address to Timer 4 Control Register */

#define DLEON_GET_TIMER4_RELOAD_REG()       DLEON_READ_REG(DLEON_TIMER4_RELOAD_REGISTER)
#define DLEON_SET_TIMER4_RELOAD_REG(value)  DLEON_WRITE_REG(DLEON_TIMER4_RELOAD_REGISTER, (value))
#define DLEON_GET_TIMER4_CTRL_REG()         DLEON_READ_REG(DLEON_TIMER4_CONTROL_REGISTER)
#define DLEON_SET_TIMER4_CTRL_REG(value)    DLEON_WRITE_REG(DLEON_TIMER4_CONTROL_REGISTER, (value))

void service_watchdog(void)
{
    /* The PSP Watchdog value is in Milliseconds, so multiply the value by 1000 */
    DLEON_SET_TIMER4_RELOAD_REG((WatchdogValue*1000));
    DLEON_SET_TIMER4_CTRL_REG(DLEON_TIMER_ENABLE_FLD | DLEON_TIMER_LOAD_FLD |DLEON_TIMER_INT_ENA_FLD);
}

