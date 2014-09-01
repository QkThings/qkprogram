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
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include "hal_hwspecs.h"

hal_clock_t _hal_clock;

void hal_clock_init()
{
  /* HFRCO is chosen by hardware as the clock source for HFCLK when
   * the device starts up (e.g. after reset and after waking up from
   * EM2 and EM3. After reset, the HFRCO frequency is 14 MHz.*/
  _hal_clock.freq = HAL_CLOCK_FREQ_16MHz;
  _hal_clock.freq_value = 16000000UL;
}

bool hal_clock_setFrequency(hal_clock_freq_t freq)
{
  return false;
}

hal_clock_freq_t hal_clock_getFreqEnum(uint32_t fosc)
{
  switch(fosc)
  {
  case 16000000UL: return HAL_CLOCK_FREQ_16MHz;
  default: return 0;
  }
}
uint32_t hal_clock_getFreqValue(hal_clock_freq_t freq)
{
  switch(freq)
  {
  case HAL_CLOCK_FREQ_16MHz: return 16000000UL;
  default: return 0;
  }
}
