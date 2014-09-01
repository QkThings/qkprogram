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

#ifndef QK_DEVICE_P_H
#define QK_DEVICE_P_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_device.h"
#include "qk_settings.h"

#if defined(QK_IS_DEVICE)


/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef volatile struct qk_device_info
{
  uint32_t _ndat;
  uint32_t _nact;
  uint32_t _nevt;
  uint32_t _ncfg;
  qk_data_type dataType;
} qk_device_info;

typedef volatile struct qk_device_buffers
{
  qk_data *data;
  qk_event *event;
  qk_action *action;
} qk_device_buffers;

typedef volatile struct qk_device_callbacks
{
  void (*sample)(void);
  void (*start)(void);
  void (*stop)(void);
  void (*action)(qk_action_id);
} qk_device_callbacks;

typedef struct qk_device
{
  qk_device_info      info;
  qk_device_buffers   buffers;
  qk_device_callbacks callbacks;
} qk_device;

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define QK_DEFINE_DEVICE(name)     qk_device *_qk_device = &(name)

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern qk_cb _pendingEvents;
extern qk_device *_qk_device;


/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

void _qk_device_init();
void _qk_device_setup();
void qk_start_sampling();
void qk_stop_sampling();
qk_cb* qk_pendingEvents();

#endif // QK_IS_DEVICE

#ifdef __cplusplus
}
#endif

#endif /* QK_DEVICE_H */
