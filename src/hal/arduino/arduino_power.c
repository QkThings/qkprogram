/*!
 * \file hal_power.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

hal_power_t _hal_power;

static void goto_sleep();

void hal_power_init()
{
  _hal_power.info.batteryStatus = HAL_BATTERY_STATUS_OK;


}

void hal_power_batteryCheck()
{

}

void hal_power_EM1()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  goto_sleep();
}

void hal_power_EM2()
{

}

void hal_power_EM3()
{

}

static void goto_sleep()
{
  sleep_enable();
  sei();
  sleep_cpu();
  sleep_disable();
}

