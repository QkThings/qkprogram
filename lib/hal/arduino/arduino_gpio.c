/*!
 * \file hal_gpio.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"
#include <avr/io.h>

void hal_gpio_init()
{
  DDRB = (1<<5); // Define data direction on porto b, 5 bit for Led
}
bool hal_getPB()
{
  return true;
}

bool hal_getDET()
{
  return true;
}

bool hal_getLED()
{
  return (PORTB & 0x20 ? true : false);
}

void hal_setLED(bool on)
{
  if (on == true)
    PORTB |= (1<<5);
  else
    PORTB &= ~(1<<5);
}

bool hal_toggleLED()
{
  bool tmp = hal_getLED();
  hal_setLED(!tmp);

  return !tmp;
}
