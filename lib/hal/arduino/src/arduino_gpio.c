/*!
 * \file hal_gpio.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include <avr/io.h>

void hal_gpio_init()
{
  DDRB = (1<<5); // Define data direction on porto b, 5 bit for Led
}
bool _getPB()
{
  return true;
}

bool _getDET()
{
  return true;
}

bool _getLED()
{
  return (PORTB & 0x20 ? true : false);
}

void _setLED(bool on)
{
  if (on == true)
    PORTB |= (1<<5);
  else
    PORTB &= ~(1<<5);
}

bool _toggleLED()
{
  bool tmp = _getLED();
  _setLED(!tmp);

  return !tmp;
}
