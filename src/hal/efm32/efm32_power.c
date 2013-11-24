/*!
 * \file hal_power.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include "em_emu.h"

hal_power_t _hal_power;

void hal_power_init()
{
  _hal_power.info.batteryStatus = HAL_BATTERY_STATUS_OK;


}

void hal_power_batteryCheck()
{
  /*bool changed = false;
  hal_battery_status_t status;

  if(changed) {
    if(_hal_power.callbacks.batteryStatusChanged != 0) {
      _hal_power.callbacks.batteryStatusChanged(status);
    }
  }*/
}

void hal_power_EM1()
{
  EMU_EnterEM1();
}

void hal_power_EM2()
{
  EMU_EnterEM1();
}

void hal_power_EM3()
{
  EMU_EnterEM2(true);
}

