/*******************************************************************************
** File: gpio_demo.c
**
*******************************************************************************/

/*
**   Include Files:
*/
#include <stdio.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include "hw_gpio.h"

/*
** Local Functions
*/
void gpio_isr(int irq, void *arg);
void gpio_isr_init(void *handle, void *func, void *arg, int irq_level, int irq_polarity);

/*
** External data
*/
extern void  *pps_in;
extern bool   gpio_ready;

/*
** global data
*/
struct gpiolib_config port_config;
uint32_t gpio_isr_counter = 0;

/*
** gpio_isr_init - used by multiple apps
*/
void gpio_isr_init(void *handle, void *func, void *arg, int irq_level, int irq_polarity)
{
   if(gpiolib_irq_register(handle, func, arg)){
      printf("Failed to register ISR\n");
   }
   port_config.mask = 1;
   port_config.irq_level = irq_level;
   port_config.irq_polarity = irq_polarity;

   if(gpiolib_set_config(handle, &port_config)){
      printf("Failed to configure port interrupt\n");
   }
   if(gpiolib_irq_enable(handle))
      printf("Failed to enable interrupt\n");

}  // end gpio_isr_init

/*
** GPIO ISR
*/
void gpio_isr(int irq, void *arg)
{
  uint32_t level;
  level = sparc_disable_interrupts();

  gpio_isr_counter++;
  if ((gpio_isr_counter % 100) == 0)
     printk("------------------------ GPIO ISR: count = %d ---------------------\n",gpio_isr_counter);

  sparc_enable_interrupts(level);
}

void init_gpio_isr( void )
{
    int status;

    printf("Calling gpio_init\n");
    status = gpio_init();
    if (status != 0)
	printf("gpio_init failed!\n");

    if (gpio_ready == TRUE)
    {
       gpio_isr_init(pps_in, gpio_isr, (void *)pps_in, GPIOLIB_IRQ_EDGE, GPIOLIB_IRQ_POL_HIGH);
    }
    else
    {
       printf("GPIO Library was not initialized, cannot init GPIO ISR\n");
    }
}
