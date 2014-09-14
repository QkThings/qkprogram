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

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

///******************************************************************************
//   ENUMS
// ******************************************************************************/
//typedef enum hal_timer_id
//{
//  HAL_TIMER_ID_1, // Delays, timeouts
//  HAL_TIMER_ID_2  // Periodic sampling
//} hal_timer_id_t;
//typedef enum hal_timer_scale
//{
//  HAL_TIMER_SCALE_SEC,
//  HAL_TIMER_SCALE_MSEC,
//  HAL_TIMER_SCALE_USEC
//} hal_timer_scale_t;
//
///******************************************************************************
//   STRUCTS
// ******************************************************************************/
//typedef volatile struct hal_timer_callbacks
//{
//  void (*timeout)(void);
//} hal_timer_callbacks_t;
//
//typedef volatile struct hal_timer_flags
//{
//  volatile unsigned timeout : 1;
//  volatile unsigned overlap : 1;
//} hal_timer_flags_t;
//
//typedef struct hal_timer
//{
//  uint32_t _value;
//  uint32_t period;
//  hal_timer_scale_t scale;
//  hal_timer_callbacks_t callbacks;
//  hal_timer_flags_t flags;
//} hal_timer_t;
//
///******************************************************************************
//   DEFINES
// ******************************************************************************/
//#ifdef QK_IS_DEVICE
//#define HAL_USE_TIMER2 //TODO count timers
//#endif
//
//#define _hal_timer_1  (&(_qk_hal_timer[HAL_TIMER_ID_2]))
//#define _hal_timer_2  (&(_qk_hal_timer[HAL_TIMER_ID_2]))
//
///******************************************************************************
//   GLOBAL VARIABLES
// ******************************************************************************/
//extern hal_timer_t _qk_hal_timer[2];
//
///******************************************************************************
//   PROTOTYPES
// ******************************************************************************/
//void _hal_timer_init();
///******************************************************************************/
//void hal_timer_init();
//void hal_timer_reinit();
//void hal_timer_start(hal_timer_id_t tmr);
//void hal_timer_stop(hal_timer_id_t tmr);
//void hal_timer_reset(hal_timer_id_t tmr);
//void hal_timer_setPeriod(hal_timer_id_t tmr, uint32_t per, hal_timer_scale_t s);
//void hal_timer_setFrequency(hal_timer_id_t tmr, uint32_t freq);
//void hal_timer_setCallback(hal_timer_id_t tmr, void (*c)(void));
//void delay_setBusyWaiting(bool enable);
//void delay_us(uint16_t value);
//void delay_ms(uint16_t value);
//void delay_s(uint16_t value);
///******************************************************************************/

#ifdef __cplusplus
}
#endif


#endif /* HAL_TIMER_H */
