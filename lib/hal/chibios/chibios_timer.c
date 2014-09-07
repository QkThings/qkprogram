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

hal_timer_t _hal_timer[2];


/******************************************************************************/
bool _delay_bw = false;


void hal_timer_init()
{
  memset(&_hal_timer[HAL_TIMER_ID_1], 0, sizeof(hal_timer_t));
  memset(&_hal_timer[HAL_TIMER_ID_2], 0, sizeof(hal_timer_t));
}

void hal_timer_reinit()
{

}
/******************************************************************************/
void hal_timer_start(hal_timer_id_t tmr)
{

}
void hal_timer_stop(hal_timer_id_t tmr)
{

}
void hal_timer_reset(hal_timer_id_t tmr)
{

}
void hal_timer_setPeriod(hal_timer_id_t tmr, uint32_t per, hal_timer_scale_t s)
{

}
void hal_timer_setFrequency(hal_timer_id_t tmr, uint32_t freq)
{

}
void hal_timer_setCallback(hal_timer_id_t tmr, void (*c)(void))
{

}


void delay_setBusyWaiting(bool en)
{
  _delay_bw = en;
}

void delay_ms(uint16_t value)
{
    chThdSleepMilliseconds(value);
}

void delay_us(uint16_t value)
{

}

