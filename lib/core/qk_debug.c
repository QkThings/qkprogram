
#include "qk_debug.h"
#include "qk_system.h"
#include "stdarg.h"

//char _qk_debug_str[_QK_DEBUGSTR_BUFSIZE];

void QK_DEBUG(char *text, ...)
{
  va_list args;
  va_start(args, text);
  _qk_protocol_sendString(text, _protocol_board);
  va_end(args);
}

//void _qk_debug_sendString()
//{
//#ifdef QK_IS_MODULE
//  //_qk_comm_sendString(_qk_debug_str, _qk_module_comm);
//#endif
//#ifdef QK_IS_DEVICE
//  _qk_protocol_sendString(_qk_debug_str, _protocol_board);
//#endif
//}

