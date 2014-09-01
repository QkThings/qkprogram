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
 *  Created on: 5 de Dez de 2013
 *      Author: mribeiro
 */

#include "../sys/qk_system.h"

hal_timer_t _hal_timer[2];

void _hal_timer_init()
{
  memset(&_hal_timer[HAL_TIMER_ID_1], 0, sizeof(hal_timer_t));
  memset(&_hal_timer[HAL_TIMER_ID_2], 0, sizeof(hal_timer_t));
}
