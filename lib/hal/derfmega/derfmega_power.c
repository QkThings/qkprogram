/*!
 * \file hal_power.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"

hal_power_t _hal_power;

void hal_power_init()
{
  _hal_power.info.batteryStatus = HAL_BATTERY_STATUS_OK;
}

void hal_power_batteryCheck()
{

}

void hal_power_EM1()
{

}

void hal_power_EM2()
{

}

void hal_power_EM3()
{

}

