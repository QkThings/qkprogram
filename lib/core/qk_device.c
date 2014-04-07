/*!
 * \file qk_device.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"

#if defined(QK_IS_DEVICE)
qk_cb _pendingEvents;
qk_event peBuf[_QK_MAX_PENDING_EVENTS];

void _qk_device_init()
{
  memset(_qk_device, 0, sizeof(qk_device));
  _qk_device->info.dataType = QK_DATA_TYPE_INT;
  qk_cb_init(&_pendingEvents, (void *)peBuf, _QK_MAX_PENDING_EVENTS, sizeof(qk_event), false);
}

void _qk_device_setup()
{
  uint32_t i;
  qk_event *e;
  for(i = 0; i < _qk_device->info._nevt; i++)
  {
    e = &(_qk_device->buffers.event[i]);
    e->_id = i;
    e->value.argc = 0;
    e->value.text = 0;
  }
}

void qk_start_sampling()
{
  _qk_request_state_change(QK_STATE_START);
}

void qk_stop_sampling()
{
  _qk_request_state_change(QK_STATE_STOP);
}

qk_cb* qk_pendingEvents()
{
  return &_pendingEvents;
}

bool qk_event_generate(uint8_t idx, float *values, uint8_t count, char *message)
{
  if(idx > _qk_device->info._nevt)
    return false;

  uint8_t i;
  qk_event *e = &(_qk_device->buffers.event[idx]);

  if(count > _QK_EVENT_MAX_ARGS)
    count = _QK_EVENT_MAX_ARGS;
  e->value.argc = count;

  for(i=0; i<count; i++)
    e->value.argv[i] = values[i];

  e->value.text = message;

  if(!qk_cb_isFull(&_pendingEvents))
    qk_cb_write(&_pendingEvents, (const void*) e);
  else
    return false;
  return true;
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
  strcpy((char*)(_qk_device->buffers.data[idx].properties.label), label);
}
void qk_data_set_type(qk_data_type type)
{
  _qk_device->info.dataType = type;
}
void qk_data_set_value_i(uint8_t idx, int32_t value)
{
  _qk_device->buffers.data[idx].value.i = value;
}
void qk_data_set_value_f(uint8_t idx, float value)
{
  _qk_device->buffers.data[idx].value.f = value;
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
  strcpy((char*)(_qk_device->buffers.action[id].properties.label), label);
}

void qk_action_set_type(qk_action_id id, qk_action_type type)
{
  _qk_device->buffers.action[id].type = type;
}

void qk_action_set_value_i(qk_action_id id, int32_t value)
{
  _qk_device->buffers.action[id].value.i = value;
}
void qk_action_set_value_b(qk_action_id id, bool value)
{
  _qk_device->buffers.action[id].value.b = value;
}
qk_action_type qk_action_get_type(qk_action_id id)
{
  return _qk_device->buffers.action[id].type;
}
int32_t qk_action_get_value_i(qk_action_id id)
{
  return _qk_device->buffers.action[id].value.i;
}
bool qk_action_get_value_b(qk_action_id id)
{
  return _qk_device->buffers.action[id].value.b;
}

void qk_set_sample_callback(void (*fnc)(void))
{
  _qk_device->callbacks.sample = fnc;
}
void qk_set_start_callback(void (*fnc)(void))
{
  _qk_device->callbacks.start = fnc;
}
void qk_set_stop_callback(void (*fnc)(void))
{
  _qk_device->callbacks.stop = fnc;
}
void qk_set_action_callback(void (*fnc)(qk_action_id))
{
  _qk_device->callbacks.action = fnc;
}
#endif
