/*
 * hw_gpio.h
 *
 *  Created on: Jan 15, 2021
 *      Author: tlng
 */

#ifndef HW_GPIO_H_
#define HW_GPIO_H_

#include <rtems.h>
#include <rtems/libio.h>
#include <grlib/gpiolib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <bsp.h> /* for device driver prototypes */

#define PPS_IN_PIN		47  

typedef enum {
	PORT1 = 1,
	PORT2 = 2
}GPIO_PORT_t;

int gpio_init(void);
void *discrete_pin_cfg(int gpio_pin_num, int direction, int initial_state);

#endif /* HW_GPIO_H_ */

