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

#if defined( OLIMEX_STM32_P107_REV_C )
#define LED_PORT  GPIOC
#define LED_PIN   GPIOC_LED_STATUS1
#define PB_PORT   GPIOA
#define PB_PIN    GPIOA_SWITCH_WKUP
#define DET_PORT  GPIOC
#define DET_PIN   GPIOC_SWITCH_TAMPER
#else
#error "ChibiOS board not defined"
#endif

static void handleInputChanged();

void hal_gpio_init()
{
 
}


bool hal_getPB()
{
  return (palReadPad(PB_PORT, PB_PIN) == PAL_LOW) ? false : true;
}

bool hal_getDET()
{
  return (palReadPad(DET_PORT, DET_PIN) == PAL_LOW) ? false : true;
}

bool hal_getLED()
{
  return (palReadPad(LED_PORT, LED_PIN) == PAL_LOW) ? false : true;
}

void hal_setLED(bool on)
{
  if(on)
    palSetPad(LED_PORT, LED_PIN);
  else
    palClearPad(LED_PORT, LED_PIN);
}

bool hal_toggleLED()
{
  if(palReadPad(LED_PORT, LED_PIN) == PAL_LOW)
  {
    palWritePad(LED_PORT, LED_PIN, PAL_HIGH);
    return true;
  }
  else
  {
    palWritePad(LED_PORT, LED_PIN, PAL_LOW);
    return false;
  }
}

static void handleInputChanged()
{
  if(_hal_gpio.callbacks.inputChanged != 0) {
    _hal_gpio.callbacks.inputChanged();
  }
  _hal_gpio.flags.inputChanged = 1;
}
