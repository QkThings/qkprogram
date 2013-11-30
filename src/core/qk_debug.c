
#include "../../include/qk_debug.h"
#include "../sys/qk_system.h"
#include "stdarg.h"

char _qk_debug_str[_QK_DEBUGSTR_BUFSIZE];

void _qk_debug(char *text, ...)
{
  va_list args;
  va_start(args, text);
  _qk_comm_sendString(text, _comm_board);
  va_end(args);
}

void _qk_debug_sendString()
{
#ifdef QK_IS_MODULE
  //qk_comm_sendString(_qk_debug_str, _qk_module_comm);
#endif
#ifdef QK_IS_DEVICE
  _qk_comm_sendString(_qk_debug_str, _comm_board);
#endif
}

