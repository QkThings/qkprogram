
#include "qk_debug.h"
#include "qk_system.h"
#include <stdio.h>
#include <stdarg.h>

#define _LOGSTR_BUFSIZE  128
static uint8_t log_mask = 0;

void qk_setLogMask(uint8_t mask)
{
  log_mask = mask;
}

#ifndef QK_LOG_NO_OUTPUT
void _QK_LOG(QK_LOG_LEVEL level, char *text, ...)
{
  if((level & log_mask) == 0) return;

  char buf[_LOGSTR_BUFSIZE];
  va_list args;
  va_start(args, text);
  vsprintf(buf,text, args);
  _qk_protocol_sendString(buf, _protocol_board);
  va_end(args);
}
#endif
