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

#if defined(QK_IS_DEVICE)
qk_cb _pendingEvents;
qk_event peBuf[_QK_EVENT_MAX_PENDING];

#define _data(idx)    (_qk_device->buffers.data[idx])
#define _event(idx)   (_qk_device->buffers.event[idx])
#define _action(idx)  (_qk_device->buffers.action[idx])

void _qk_device_init()
{
  memset(_qk_device, 0, sizeof(qk_device));
  _qk_device->info.dataType = QK_DATA_TYPE_INT;
  qk_cb_init(&_pendingEvents, (void *)peBuf, _QK_EVENT_MAX_PENDING, sizeof(qk_event), false);
}

void _qk_device_setup()
{
  uint32_t i;
  for(i = 0; i < _qk_device->info._nevt; i++)
  {
    _event(i)._id = i;
    _event(i).value.argc = 0;
    _event(i).value.text = 0;
  }
}

#ifndef QK_PROGRAM_RAW
void qk_start_sampling()
{
  _qk_request_state_change(QK_CORE_STATE_START);
}

void qk_stop_sampling()
{
  _qk_request_state_change(QK_CORE_STATE_STOP);
}
#endif

qk_cb* qk_pendingEvents()
{
  return &_pendingEvents;
}

void qk_event_set_args(uint8_t idx, float *args, uint8_t count)
{
  uint8_t i;
  if(count > _QK_EVENT_MAX_ARGS)
    count = _QK_EVENT_MAX_ARGS;
  _event(idx).value.argc = count;
  for(i = 0; i < count; i++)
    _event(idx).value.argv[i] = args[i];
}

bool qk_event_generate(uint8_t idx, char *message)
{
  _event(idx).value.text = message;

  if(!qk_cb_isFull(&_pendingEvents))
  {
    qk_cb_write(&_pendingEvents, (const void*) &(_event(idx)));
    return true;
  }
  else
    return false;
}

void qk_data_set_buffer(qk_data *buf, uint32_t count)
{
  _qk_device->buffers.data = buf;
  _qk_device->info._ndat = count;
}
void qk_data_set_count(uint32_t count)
{
  _qk_device->info._ndat = count;
}
void qk_data_set_label(uint8_t idx, char *label)
{
  strcpy((char*)(_data(idx).properties.label), label);
}
void qk_data_set_type(qk_data_type type)
{
  _qk_device->info.dataType = type;
}
void qk_data_set_value_i(uint8_t idx, int32_t value)
{
  _data(idx).value.i = value;
}
void qk_data_set_value_f(uint8_t idx, float value)
{
  _data(idx).value.f = value;
}
void qk_event_set_buffer(qk_event *buf, uint32_t count)
{
  _qk_device->buffers.event = buf;
  _qk_device->info._nevt = count;
}
void qk_event_set_label(uint8_t idx, const char *label)
{
  strcpy((char*)(_qk_device->buffers.event[idx].properties.label), label);
}

void qk_action_set_buffer(qk_action *buf, unsigned int size)
{
  _qk_device->buffers.action = buf;
  _qk_device->info._nact = size;
}

void qk_action_set_label(qk_action_id id, const char *label)
{
  strcpy((char*)(_action(id).properties.label), label);
}

void qk_action_set_type(qk_action_id id, qk_action_type type)
{
  _action(id).type = type;
}

void qk_action_set_value_i(qk_action_id id, int32_t value)
{
  _action(id).value.i = value;
}
void qk_action_set_value_b(qk_action_id id, bool value)
{
  _action(id).value.b = value;
}
qk_action_type qk_action_get_type(qk_action_id id)
{
  return _action(id).type;
}
int32_t qk_action_get_value_i(qk_action_id id)
{
  return _action(id).value.i;
}
bool qk_action_get_value_b(qk_action_id id)
{
  return _action(id).value.b;
}

void qk_action_set_callback(void (*fnc)(qk_action_id))
{
  _qk_device->callbacks.action = fnc;
}

void qk_sampling_set_callback(qk_sampling_callback id, void (*fnc)(void))
{
  switch(id)
  {
  case QK_SAMPLING_CALLBACK_SAMPLE: _qk_device->callbacks.sample = fnc; break;
  case QK_SAMPLING_CALLBACK_START:  _qk_device->callbacks.start = fnc; break;
  case QK_SAMPLING_CALLBACK_STOP:   _qk_device->callbacks.stop = fnc; break;
  default: ;
  }
}


#endif
