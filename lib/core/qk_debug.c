
#include "qk_debug.h"
#include "qk_system.h"
#include <stdio.h>
#include <stdarg.h>

qk_log_mask _log_mask;
char _qk_log_str[_QK_LOG_BUFSIZE];

void qk_log_set_levels(uint8_t mask)
{
  _log_mask = mask;
}


void _qk_log_send()
{
  _qk_protocol_send_string(_qk_log_str, _protocol_board);
}
