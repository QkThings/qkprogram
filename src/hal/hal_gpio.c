/*
 * hal_gpio.c
 *
 *  Created on: 5 de Dez de 2013
 *      Author: mribeiro
 */

#include "../sys/qk_system.h"

hal_gpio_t _hal_gpio;

void _hal_gpio_init()
{
  memset(&_hal_gpio, 0, sizeof(hal_gpio_t));
}
