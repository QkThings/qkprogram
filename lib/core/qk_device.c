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
  qk_cb_init(&_pendingEvents, peBuf, _QK_MAX_PENDING_EVENTS, sizeof(qk_event), false);
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

void qk_startSampling()
{
  _qk_requestStateChange(QK_STATE_START);
}

void qk_stopSampling()
{
  _qk_requestStateChange(QK_STATE_STOP);
}

qk_cb* qk_pendingEvents()
{
  return &_pendingEvents;
}

bool qk_fireEvent(uint8_t idx, float *values, uint8_t count, char *message)
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
    qk_cb_write(&_pendingEvents, e);
  else
    return false;
  return true;
}

void qk_setDataBuffer(qk_data *buf, uint32_t count)
{
  _qk_device->buffers.data = buf;
  _qk_device->info._ndat = count;
}
void qk_setDataCount(uint32_t count)
{
  _qk_device->info._ndat = count;
}
void qk_setDataLabel(uint8_t idx, char *label)
{
  strcpy((char*)(_qk_device->buffers.data[idx].properties.label), label);
}
void qk_setDataType(qk_data_type type)
{
  _qk_device->info.dataType = type;
}
void qk_setDataValueI(uint8_t idx, int32_t value)
{
  _qk_device->buffers.data[idx].value.i = value;
}
void qk_setDataValueF(uint8_t idx, float value)
{
  _qk_device->buffers.data[idx].value.f = value;
}
void qk_setEventBuffer(qk_event *buf, uint32_t count)
{
  _qk_device->buffers.event = buf;
  _qk_device->info._nevt = count;
}
void qk_setEventLabel(uint8_t idx, const char *label)
{
  strcpy((char*)(_qk_device->buffers.event[idx].properties.label), label);
}

void qk_setActionBuffer(qk_action *buf, unsigned int size)
{
  _qk_device->buffers.action = buf;
  _qk_device->info._nact = size;
}

void qk_setActionLabel(qk_action_id id, const char *label)
{
  strcpy((char*)(_qk_device->buffers.action[id].properties.label), label);
}

void qk_setActionType(qk_action_id id, qk_action_type type)
{
  _qk_device->buffers.action[id].type = type;
}

void qk_setActionValueI(qk_action_id id, int32_t value)
{
  _qk_device->buffers.action[id].value.i = value;
}
void qk_setActionValueB(qk_action_id id, bool value)
{
  _qk_device->buffers.action[id].value.b = value;
}
qk_action_type qk_actionType(qk_action_id id)
{
  return _qk_device->buffers.action[id].type;
}
int32_t qk_actionValueI(qk_action_id id)
{
  return _qk_device->buffers.action[id].value.i;
}
bool qk_actionValueB(qk_action_id id)
{
  return _qk_device->buffers.action[id].value.b;
}

void qk_setSampleCallback(void (*fnc)(void))
{
  _qk_device->callbacks.sample = fnc;
}
void qk_setStartCallback(void (*fnc)(void))
{
  _qk_device->callbacks.start = fnc;
}
void qk_setStopCallback(void (*fnc)(void))
{
  _qk_device->callbacks.stop = fnc;
}
void qk_setActionCallback(void (*fnc)(qk_action_id))
{
  _qk_device->callbacks.action = fnc;
}
#endif
