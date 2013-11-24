
#include "../../include/qk_debug.h"
#include "../sys/qk_system.h"

char _qk_debug_str[_QK_DEBUGSTR_BUFSIZE];

void _qk_debug_sendString()
{
#ifdef QK_IS_MODULE
  //qk_comm_sendString(_qk_debug_str, _qk_module_comm);
#endif
#ifdef QK_IS_DEVICE
  _qk_comm_sendString(_qk_debug_str, _comm_board);
#endif
}

