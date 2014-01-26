/*!
 * \file qk_utils.c
 *
 * \author root
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include <string.h>

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
 *  Lightweight STDIO functions
 *****************************************************************************/
// Source: http://www.menie.org/georges/embedded/

static void printchar(char **str, int c)
{
  if (str) {
    **str = c;
    ++(*str);
  }
  else {
    hal_uart_writeByte(HAL_UART_ID_1, c);
    if(c == '\n')
      hal_uart_writeByte(HAL_UART_ID_1, '\r');
  }
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
  register int pc = 0, padchar = ' ';

  if (width > 0) {
    register int len = 0;
    register const char *ptr;
    for (ptr = string; *ptr; ++ptr) ++len;
    if (len >= width) width = 0;
    else width -= len;
    if (pad & PAD_ZERO) padchar = '0';
  }
  if (!(pad & PAD_RIGHT)) {
    for ( ; width > 0; --width) {
      printchar (out, padchar);
      ++pc;
    }
  }
  for ( ; *string ; ++string) {
    printchar (out, *string);
    ++pc;
  }
  for ( ; width > 0; --width) {
    printchar (out, padchar);
    ++pc;
  }

  return pc;
}

// the following should be enough for 32 bit int
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
  char print_buf[PRINT_BUF_LEN];
  register char *s;
  register int t, neg = 0, pc = 0;
  register unsigned int u = i;

  if (i == 0) {
    print_buf[0] = '0';
    print_buf[1] = '\0';
    return prints (out, print_buf, width, pad);
  }

  if (sg && b == 10 && i < 0) {
    neg = 1;
    u = -i;
  }

  s = print_buf + PRINT_BUF_LEN-1;
  *s = '\0';

  while (u) {
    t = u % b;
    if( t >= 10 )
      t += letbase - '0' - 10;
    *--s = t + '0';
    u /= b;
  }

  if (neg) {
    if( width && (pad & PAD_ZERO) ) {
      printchar (out, '-');
      ++pc;
      --width;
    }
    else {
      *--s = '-';
    }
  }

  return pc + prints (out, s, width, pad);
}

static int print(char **out, int *varg)
{
  register int width, pad;
  register int pc = 0;
  register char *format = (char *)(*varg++);
  char scr[2];

  for (; *format != 0; ++format) {
    if (*format == '%') {
      ++format;
      width = pad = 0;
      if (*format == '\0') break;
      if (*format == '%') goto out;
      if (*format == '-') {
        ++format;
        pad = PAD_RIGHT;
      }
      while (*format == '0') {
        ++format;
        pad |= PAD_ZERO;
      }
      for ( ; *format >= '0' && *format <= '9'; ++format) {
        width *= 10;
        width += *format - '0';
      }
      if( *format == 's' ) {
        register char *s = *((char **)varg++);
        pc += prints (out, s?s:"(null)", width, pad);
        continue;
      }
      if( *format == 'd' ) {
        pc += printi (out, *varg++, 10, 1, width, pad, 'a');
        continue;
      }
      if( *format == 'x' ) {
        pc += printi (out, *varg++, 16, 0, width, pad, 'a');
        continue;
      }
      if( *format == 'X' ) {
        pc += printi (out, *varg++, 16, 0, width, pad, 'A');
        continue;
      }
      if( *format == 'u' ) {
        pc += printi (out, *varg++, 10, 0, width, pad, 'a');
        continue;
      }
      if( *format == 'c' ) {
        // char are converted to int then pushed on the stack
        scr[0] = *varg++;
        scr[1] = '\0';
        pc += prints (out, scr, width, pad);
        continue;
      }
    }
    else {
    out:
      printchar (out, *format);
      ++pc;
    }
  }
  if (out) **out = '\0';
  return pc;
}

// assuming sizeof(void *) == sizeof(int)

int printf(const char *format, ...)
{
  register int *varg = (int *)(&format);
  return print(0, varg);
}

int sprintf(char *out, const char *format, ...)
{
  register int *varg = (int *)(&format);
  return print(&out, varg);
}

/*****************************************************************************
 *  Others
 *****************************************************************************/
void _blinkLED(uint8_t n, uint16_t msec)
{
  do {
    _setLED(true);
    delay_ms(msec);
    _setLED(false);
    delay_ms(msec);
  }
  while(--n > 0);
}

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