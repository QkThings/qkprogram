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
