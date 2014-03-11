/*
 * hal_gpio.c
 *
 *  Created on: 5 de Dez de 2013
 *      Author: mribeiro
 */

#include "qk_system.h"

hal_gpio_t _hal_gpio;

void _hal_gpio_init()
{
  memset(&_hal_gpio, 0, sizeof(hal_gpio_t));
}

void hal_blinkLED(uint8_t n, uint16_t msec)
{
  do {
    hal_setLED(true);
    delay_ms(msec);
    hal_setLED(false);
    delay_ms(msec);
  }
  while(--n > 0);
}
