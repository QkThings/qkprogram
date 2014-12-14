/*
 * QkThings LICENSE
 * The open source framework and modular platform for smart devices.
 * Copyright (C) 2014 <http://qkthings.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "qk_system.h"
#include "qk_debug.h"
#include <stdio.h>
#include <stdarg.h>

qk_log_mask _log_mask;
qk_protocol *_log_protocol = qk_protocol_board;

char _qk_log_str[_QK_LOG_BUFSIZE];

void qk_log_set_protocol(qk_protocol *protocol)
{
  _log_protocol = protocol;
}

void qk_log_set_levels(uint8_t mask)
{
  _log_mask = mask;
}

void _qk_log_send()
{
  _qk_protocol_send_string(_qk_log_str, _log_protocol);
}
