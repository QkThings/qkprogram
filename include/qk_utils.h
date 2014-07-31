/*!
 * \file qk_utils.h
 *
 * \author root
 *  
 * This file is part of QkProgram
 */

#ifndef QK_UTILS_H
#define QK_UTILS_H

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************
 *  Defines and some useful macros
 *****************************************************************************/
#define round(x)  ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define abs(x)    (x>0?x:(-x))

#define flag(flag,mask) (flag & mask ? 1 : 0)
#define flag_set(flag, mask) (flag |= mask)
#define flag_clr(flag, mask) (flag &= ~mask)

/*****************************************************************************
 *  Date and Time
 *****************************************************************************/
typedef struct qk_datetime
{
  unsigned int hours : 5;
  unsigned int minutes : 6;
  unsigned int seconds : 6;
  unsigned int year : 6;
  unsigned int month : 4;
  unsigned int day : 5;
} qk_datetime;

typedef qk_datetime qk_time;

void qk_datetime_set_time(qk_datetime *dt, uint8_t hr, uint8_t min, uint8_t sec);
void qk_datetime_set_date(qk_datetime *dt, uint8_t year, uint8_t month, uint8_t day);

/*****************************************************************************
 *  Buffers
 *****************************************************************************/
typedef struct qk_buf
{
  void *ptr;
  int count;
} qk_buf;

#define QK_BUF_SET_PTR(buf, p)      ((buf)->ptr = (p))
#define QK_BUF_SET_COUNT(buf, c)  ((buf)->count = (c))

#define QK_BUF_PTR(buf)   ((buf)->ptr)
#define QK_BUF_COUNT(buf) ((buf)->count)

/*****************************************************************************
 *  Circular Buffers
 *****************************************************************************/
typedef volatile struct qk_cb
{
  void     *buf;
  void     *bufEnd;
  uint32_t capacity;
  uint32_t itemSize;
  uint32_t count;
  void *head;
  void *tail;
  bool overwrite;
} qk_cb;

void qk_cb_init(qk_cb *cb, void *buf, uint32_t bufSize, uint32_t itemSize, bool overwrite);
void qk_cb_write(qk_cb *cb, const void *item);
void qk_cb_read(qk_cb *cb, void *item);
void *qk_cb_pick(qk_cb *cb);
bool qk_cb_isFull(qk_cb *cb);
bool qk_cb_isEmpty(qk_cb *cb);
uint32_t qk_cb_available(qk_cb *cb);

/*****************************************************************************
 *  Callbacks
 *****************************************************************************/

typedef struct qk_callback_arg
{
  void *ptr;
  int val_i;
  qk_buf *buf;
} qk_callback_arg;

typedef void (*qk_callback)(qk_callback_arg *arg);

#define QK_CALLBACK_ARG_SET_PTR(arg, p) ((arg)->ptr = (p))
#define QK_CALLBACK_ARG_SET_INT(arg, i) ((arg)->val_i = (i))
#define QK_CALLBACK_ARG_SET_BUF(arg, b) ((arg)->buf = (b))

#define QK_CALLBACK_ARG_PTR(arg) ((arg)->ptr)
#define QK_CALLBACK_ARG_INT(arg) ((arg)->val_i)
#define QK_CALLBACK_ARG_BUF(arg) ((arg)->buf)

#define _QK_CALLBACK_REGISTER(cb, c) ((cb) = (c))

/*****************************************************************************
 *  Lightweight STDIO functions
 *****************************************************************************/
int sprintf(char *str, const char *format, ...);
int printf(const char *format, ...);

/*****************************************************************************
 *  Others
 *****************************************************************************/
typedef union _IntFloatConverter
{
  int32_t i_value;
  float f_value;
} _IntFloatConverter;

//void _blinkLED(uint8_t n, uint16_t msec);
float _floatFromBytes(int32_t value);
int32_t _bytesFromFloat(float value);

#endif /* QK_UTILS_H */
