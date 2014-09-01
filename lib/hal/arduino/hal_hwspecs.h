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

#ifndef HAL_HWSPECS_H_
#define HAL_HWSPECS_H_

/* Available clock frequencies (obtained through external crystals/oscilators or internal
 * oscilators */
typedef enum hal_clock_freq {
  HAL_CLOCK_FREQ_16MHz = 0, // Default
} hal_clock_freq_t;

#define HAL_CLOCK_FREQ_FASTER   HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_FAST     HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_NORMAL   HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_SLOW     HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_SLOWER   HAL_CLOCK_FREQ_16MHz

#endif /* HAL_HWSPECS_H_ */
