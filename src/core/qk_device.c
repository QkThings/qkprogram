/*!
 * \file qk_device.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"

#if defined(QK_IS_DEVICE)
qk_cb _pendingEvents;

qk_event peBuf[QK_MAX_PENDING_EVENTS];

void _qk_device_init()
{
  memset(_qk_device, 0, sizeof(qk_device));
  _qk_device->info.dataType = QK_DATA_TYPE_INT;
  qk_cb_init(&_pendingEvents, peBuf, QK_MAX_PENDING_EVENTS, sizeof(qk_event), false);
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

bool qk_fireEvent(uint8_t idx, float *values, uint8_t count, char *message)
{
  if(idx > _qk_device->info._nevt)
    return false;

  uint8_t i;
  qk_event *e = &(_qk_device->buffers.event[idx]);

  if(count > QK_EVENT_MAX_ARGS)
    count = QK_EVENT_MAX_ARGS;
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
#endif
