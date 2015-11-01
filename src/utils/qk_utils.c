#include "qk_system.h"
#include <string.h>

/*****************************************************************************
 *  Date and Time
 *****************************************************************************/

void qk_datetime_set_time(qk_datetime *dt, uint8_t hr, uint8_t min, uint8_t sec)
{
  dt->hours = hr;
  dt->minutes = min;
  dt->seconds = sec;
}
void qk_datetime_set_date(qk_datetime *dt, uint8_t year, uint8_t month, uint8_t day)
{
  dt->year = year;
  dt->month = month;
  dt->day = day;
}

/*****************************************************************************
 *  Circular Buffers
 *****************************************************************************/
void qk_cb_init(qk_cb *cb, void *buf, uint32_t capacity, uint32_t itemSize, bool overwrite)
{
  cb->buf = buf;
  cb->bufEnd = buf + (capacity*itemSize);
  cb->capacity = capacity;
  cb->itemSize = itemSize;
  cb->count = 0;
  cb->head = buf;
  cb->tail = buf;
  cb->overwrite = overwrite;
}
void qk_cb_write(qk_cb *cb, const void *item)
{
  if(qk_cb_isFull(cb) && cb->overwrite == false)
    return;
  memcpy(cb->head, item, cb->itemSize);
  cb->head = (char*)cb->head + cb->itemSize;
  if(cb->head == cb->bufEnd)
      cb->head = cb->buf;
  cb->count++;
}

void qk_cb_read(qk_cb *cb, void *item)
{
  memcpy(item, cb->tail, cb->itemSize);
  cb->tail = (char*)cb->tail + cb->itemSize;
  if(cb->tail == cb->bufEnd)
      cb->tail = cb->buf;
  cb->count--;
}

void *qk_cb_pick(qk_cb *cb)
{
  return cb->tail;
}

bool qk_cb_isFull(qk_cb *cb)
{
  return (cb->count == cb->capacity ? true : false);
}
bool qk_cb_isEmpty(qk_cb *cb)
{
  return (cb->count == 0 ? true : false);
}
uint32_t qk_cb_available(qk_cb *cb)
{
  return cb->count;
}

/*****************************************************************************
 *  Others
 *****************************************************************************/

float _floatFromBytes(int32_t value)
{
  _IntFloatConverter converter;
  converter.i_value = value;
  return converter.f_value;
}
int32_t _bytesFromFloat(float value)
{
  _IntFloatConverter converter;
  converter.f_value = value;
  return converter.i_value;
}
