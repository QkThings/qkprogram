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

qk_core _qk_core;
/******************************************************************************/
static void handle_board_detection();
/******************************************************************************/

inline void
qk_core_register_callback(qk_core_callback_id id,
                          qk_callback cb)
{
  _QK_CALLBACK_REGISTER(_qk_core.callback[id], cb);
}

void qk_core_init()
{
  memset((void*)&_qk_core, 0, sizeof(qk_core));
  
  _qk_core.info.baudRate = _HAL_UART_BAUD_DEFAULT_LOW;

  handle_board_detection();
}

void qk_run()
{
  uint32_t i, count;
  qk_callback_arg cb_arg;

  if(_qk_core.callback[QK_CORE_CALLBACK_APP] != 0)
  {
    _qk_core.callback[QK_CORE_CALLBACK_APP](&cb_arg);
  }

  for(i = 0; i < QK_PROTOCOL_STRUCT_COUNT; i++)
  {
    if(_qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_READ] != 0)
    {
      _qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_READ](&cb_arg);
    }

    if(_qk_protocol[i].flags.status & QK_PROTOCOL_FLAGMASK_NEWPACKET)
    {
      QK_CALLBACK_ARG_SET_APTR(&cb_arg, 0, (void*) (&_qk_protocol[i]));
      if(_qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_PROCESSPACKET] != 0)
        _qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_PROCESSPACKET](&cb_arg);

      _qk_protocol[i].flags.status &= ~QK_PROTOCOL_FLAGMASK_NEWPACKET;
    }
  }

#if defined( QK_IS_DEVICE )
  count = _QK_EVENT_MAX_FIRED;
  qk_cb *pendingEvents = qk_pendingEvents();
  while(qk_cb_available(pendingEvents) > 0 && count--)
  {
    qk_event firedEvent;
    qk_cb_read(pendingEvents, (void*) &firedEvent);
    //qk_event *firedEvent = (qk_event*)qk_cb_pick(pendingEvents);
    _qk_protocol_send_event(&firedEvent, qk_protocol_board);
  }
#endif



}

void qk_loop()
{
  while(1) { qk_run(); }
}

void _qk_request_state_change(qk_state state)
{
  _qk_core.change_to_state = state;
  _qk_core.flags.intern |= QK_CORE_FLAG_INTERN_RQSTATECHANGE;
}
void _qk_handle_state_change()
{
  if(_qk_core.flags.intern & QK_CORE_FLAG_INTERN_RQSTATECHANGE)
  {
    _qk_core.currentState = _qk_core.change_to_state;
    _qk_core.flags.intern &= ~QK_CORE_FLAG_INTERN_RQSTATECHANGE;
  }
}



static void handle_board_detection()
{
#ifdef _QK_PROGRAM_DEV_BOARDALWAYSDET
  _qk_core.flags.status |= QK_CORE_FLAG_STATUS_BOARDDET;
#else
  bool detected = !qk_gpio_get_pin(_QK_HAL_DET); // DET pin is pulled-up
  qk_callback_arg cb_arg;

  if(flag(_qk_core.flags.reg_status, QK_FLAGMASK_STATUS_DET) == detected) 
  {
    return;
  }

  if(detected)
  {
    qk_board_led_blink(2, 50);
    _qk_core.flags.reg_status |= QK_FLAGMASK_STATUS_DET;

    //qk_uart_set_baudrate(_QK_PROGRAM_UART, _qk_core.info.baudRate);

    if(_qk_core.callback[QK_CORE_CALLBACK_BOARDATTACHED] != 0) {
      _qk_core.callback[QK_CORE_CALLBACK_BOARDATTACHED](&cb_arg);
    }

#if defined( QK_IS_DEVICE )
//start
#endif
  }
  else
  {
    qk_board_led_blink(1, 50);
    _qk_core.flags.reg_status &= ~QK_FLAGMASK_STATUS_DET;

    //qk_uart_set_baudrate(_QK_PROGRAM_UART, _HAL_UART_BAUD_DEFAULT_LOW);

    if(_qk_core.callback[QK_CORE_CALLBACK_BOARDREMOVED] != 0)
      _qk_core.callback[QK_CORE_CALLBACK_BOARDREMOVED](&cb_arg);

#if defined( QK_IS_DEVICE )
//stop
#endif
  }
#endif // _QK_PROGRAM_DEV_BOARDALWAYSDET
}

