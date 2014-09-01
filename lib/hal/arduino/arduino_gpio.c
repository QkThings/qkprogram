/*
 * QkThings LICENSE
 * The open source framework and modular platform for smart devices.
 * Copyright (C) 2014 <http://qkthings.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
