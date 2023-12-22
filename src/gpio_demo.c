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
#include <rtems/libio.h>
#include <grlib/gpiolib.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <bsp.h> /* for device driver prototypes */

/*
** For this demo, GPIO pin 47 is used for the interrupt.
**  Normally this pin has a 1PPS signal that causes the interrupt.
**  For this demo, a STM32 board is being used to provide a programmable
**  rate interrupt on this pin. 
*/
#define PPS_IN_PIN              47

/*
** Local Functions
*/
int gpio_init(void);
void gpio_isr(int irq, void *arg);
void gpio_isr_init(void *handle, void *func, void *arg, int irq_level, int irq_polarity);
void *discrete_pin_cfg(int gpio_pin_num, int direction, int initial_state);

/*
** global data
*/
struct gpiolib_config port_config;
uint32_t              gpio_isr_counter = 0;
void                 *pps_in;

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


void *discrete_pin_cfg(int gpio_pin_num, int direction, int initial_state)
{
   void *handle;
   struct gpiolib_config port_config;

   gpiolib_show(gpio_pin_num, NULL);
   handle = gpiolib_open(gpio_pin_num);
   if(handle == NULL){
 	printf("GPIO Port Open Error!\n");
	return NULL;
   }

   // Disable interrupt
   port_config.mask = 0;
   port_config.irq_level = GPIOLIB_IRQ_LEVEL;
   port_config.irq_polarity = GPIOLIB_IRQ_POL_LOW;

   if(gpiolib_set_config(handle, &port_config)){
 	printf("GPIO Config Error!\n");
	return NULL;
   }

   gpiolib_set(handle, direction, initial_state);
   return handle;
}

/*
** gpio_isr_init
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
** init_gpio 
*/
void init_gpio( void )
{
    printf("Initialize GPIO\n");

    /* Initialize gpiolib */
    if ( gpiolib_initialize() ) 
       printf("Failed to initialize GPIO library\n");
    else
       printf("GPIO library initialized\n");

    /* Show all GPIO Ports available */
    gpiolib_show(-1, NULL);

    pps_in = discrete_pin_cfg(PPS_IN_PIN, 0, 0);
    if(pps_in == NULL)
    {
       printf("Failed to setup GPIO pin\n");
    }
    else
    {
       printf("Setting up GPIO ISR\n");
       gpio_isr_init(pps_in, gpio_isr, (void *)pps_in, GPIOLIB_IRQ_EDGE, GPIOLIB_IRQ_POL_HIGH);
    }
}

