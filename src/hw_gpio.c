/*
 * hw_gpio.c
 *
 *  Created on: Jan 15, 2021
 *      Author: tlng
 */
#include "hw_gpio.h"

void *pps_in;
unsigned int pps_state, pps_count;
bool gpio_ready = FALSE;
rtems_id gpio_sem;

void *discrete_pin_cfg(int gpio_pin_num, int direction, int initial_state);

int gpio_init()
{
	// Initialize gpiolib
	if ( gpiolib_initialize() ) {
		printf("Failed to initialize GPIO library\n");
		return -1;
	}

	/* Show all GPIO Ports available */
	gpiolib_show(-1, NULL);

	pps_in = discrete_pin_cfg(PPS_IN_PIN, 0, 0);
	if(pps_in == NULL)
		return -1;

        gpio_ready = TRUE;

	return 0;
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

