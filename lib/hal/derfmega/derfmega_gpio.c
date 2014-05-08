/*!
 * \file hal_gpio.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"


hal_gpio_t _hal_gpio;

static void handleInputChanged();

void hal_gpio_init()
{

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
  return true;
}

void hal_setLED(bool on)
{

}

bool hal_toggleLED()
{

}

static void handleInputChanged()
{
  if(_hal_gpio.callbacks.inputChanged != 0) {
    _hal_gpio.callbacks.inputChanged();
  }
  _hal_gpio.flags.inputChanged = 1;
}
