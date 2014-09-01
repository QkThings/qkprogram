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

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
   ENUMS
 ******************************************************************************/

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct hal_gpio_callbacks
{
  void (*inputChanged)(void);
} hal_gpio_callbacks_t;

typedef volatile struct hal_gpio_flags
{
  unsigned inputChanged : 1;
} hal_gpio_flags_t;

typedef struct hal_gpio
{
  hal_gpio_callbacks_t callbacks;
  hal_gpio_flags_t flags;
} hal_gpio_t;

/******************************************************************************
   DEFINES
 ******************************************************************************/

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern hal_gpio_t _hal_gpio;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void _hal_gpio_init();
void hal_gpio_init();

bool hal_getPB();
bool hal_getDET();
//bool _getLED();
//void _setLED(bool on);
//bool _toggleLED();

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_H */
