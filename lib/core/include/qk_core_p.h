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

#ifndef QK_CORE_P_H
#define QK_CORE_P_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_core.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/
typedef enum qk_state
{
  QK_STATE_SLEEP,
  QK_STATE_IDLE,
  QK_STATE_START,
  QK_STATE_RUNNING,
  QK_STATE_STANDBY,
  QK_STATE_STOP
} qk_state;

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct qk_info
{
  uint32_t baudRate;
} qk_info;

//typedef struct qk_callbacks
//{
//  void (*boardAttached)(void);
//  void (*boardRemoved)(void);
//  void (*inputChanged)(void);
//  void (*clockChanged)(uint32_t new_value);
//} qk_callbacks;



typedef volatile struct qk_flags
{
  union
  {
    uint64_t reg;
    struct
    {
      uint16_t reg_status;
      uint16_t reg_settings;
      uint16_t _rsvd_;
      uint8_t reg_internal;
      uint8_t reg_sleep;
    };
  };
} qk_flags;

typedef struct qk_sampling
{
  //uint16_t  lastPer;            //! Last valid sampling period
  uint32_t  N;
  uint32_t  period;
  uint32_t  frequency;
  uint8_t   mode;
  uint8_t   triggerClock;
  uint8_t   triggerScaler;
} qk_sampling;

typedef struct qk_core
{
  volatile qk_state currentState;
  volatile qk_state change_to_state;
  qk_info       info;
  qk_clock_mode clockMode;
  qk_callback   callback[_QK_CORE_CALLBACK_COUNT];
  qk_flags      flags;
#if defined(QK_IS_DEVICE)
  qk_sampling   sampling;
#endif
} qk_core;

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define QK_FLAGMASK_STATUS_RUNNING          (1<<0)
#define QK_FLAGMASK_STATUS_DET              (1<<1)
#define QK_FLAGMASK_SETTINGS_AUTOSAMP       (1<<0)
#define QK_FLAGMASK_SETTINGS_TIMESTAMP      (1<<1)
#define QK_FLAGMASK_SETTINGS_EVTNOTIF       (1<<2)
#define QK_FLAGMASK_SETTINGS_STATNOTIF      (1<<3)
#define QK_FLAGMASK_INTERNAL_RQSTATECHANGE  (1<<0)
#define QK_FLAGMASK_SLEEP_EVENTS_PENDING    (1<<0)

#define QK_DEFAULT_FLAGS  0

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern qk_core _qk_core;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_core_init();
//bool qk_clock_set_mode(qk_clock_mode mode);

void _qk_request_state_change(qk_state state);
void _qk_handle_state_change();


/******************************************************************************/
static inline
bool _qk_canSleep()
{
  return (_qk_core.flags.reg_sleep == 0);
}

#ifdef __cplusplus
}
#endif

#endif /* QK_CORE_H */
