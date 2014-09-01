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

#ifndef HAL_CLOCK_H
#define HAL_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_hwspecs.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct hal_clock_callbacks
{
  void (*changed)(uint64_t new_fosc);
} hal_clock_callbacks_t;

typedef struct hal_clock
{
  hal_clock_freq_t freq;
  uint32_t freq_value;
  hal_clock_callbacks_t callbacks;
} hal_clock_t;

/******************************************************************************
   DEFINES
 ******************************************************************************/

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern hal_clock_t _hal_clock;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void _hal_clock_init();
void hal_clock_init();
bool hal_clock_setFrequency(hal_clock_freq_t freq);
hal_clock_freq_t hal_clock_getFreqEnum(uint32_t value);
uint32_t hal_clock_getFreqValue(hal_clock_freq_t freq);
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* HAL_CLOCK_H */
