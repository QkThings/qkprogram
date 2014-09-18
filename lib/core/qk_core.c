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
//static void handle_input_changed();
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
  _qk_core.currentState = QK_STATE_IDLE;
  _qk_core.info.baudRate = _HAL_UART_BAUD_DEFAULT_LOW;
#if defined( QK_IS_DEVICE )
  _qk_core.sampling.frequency = 0; // invalid
  _qk_core.sampling.mode = QK_SAMPLING_MODE_CONTINUOUS;
  _qk_core.sampling.triggerClock = QK_SAMPLING_TRIGGER_CLOCK_10SEC;
  _qk_core.sampling.triggerScaler = 1;
  _qk_core.sampling.N = 10;

  qk_sampling_set_frequency(_QK_DEFAULT_SAMPFREQ);
#endif

  handle_board_detection();
}

void qk_run()
{
  uint32_t i, count;
  qk_callback_arg cb_arg;

  /*************************************
   * PROCESSING
   ************************************/

//  if(qk_gpio_flags())
//  {
//    handle_board_detection();
//  }

#ifdef _QK_FEAT_UART_POLLING
  hal_uart_poll(HAL_UART_ID_1);
#endif

  if(_qk_core.callback[QK_CORE_CALLBACK_APP] != 0)
    _qk_core.callback[QK_CORE_CALLBACK_APP](&cb_arg);

//  qk_gpio_flags_clear();

  //TODO Status notifications
  //TODO Events

  for(i = 0; i < QK_PROTOCOL_STRUCT_COUNT; i++)
  {
    if(_qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_PROCESSBYTES] != 0)
      _qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_PROCESSBYTES](&cb_arg);

    if(_qk_protocol[i].flags.reg & QK_PROTOCOL_FLAGMASK_NEWPACKET)
    {
      if(_qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_PROCESSPACKET] != 0)
        _qk_protocol[i].callback[QK_PROTOCOL_CALLBACK_PROCESSPACKET](&cb_arg);

      _qk_protocol[i].flags.reg &= ~QK_PROTOCOL_FLAGMASK_NEWPACKET;
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

  _qk_handle_state_change();

  /*************************************
   * STATE MACHINE
   ************************************/
  switch(_qk_core.currentState)
  {
  case QK_STATE_SLEEP:
    break;
  case QK_STATE_IDLE:
    qk_timer_stop(_QK_PROGRAM_TIMER1);
    break;
#if defined(QK_IS_DEVICE)
  case QK_STATE_START:
    if(_qk_device->callbacks.start != 0)
      _qk_device->callbacks.start();
    qk_timer_restart(_QK_PROGRAM_TIMER1);
    _qk_core.currentState = QK_STATE_RUNNING;
    break;
  case QK_STATE_RUNNING:
    if(qk_timer_flags(_QK_PROGRAM_TIMER1) & QK_TIMER_FLAG_TIMEOUT)
    {
      if(_qk_device->callbacks.sample != 0)
        _qk_device->callbacks.sample();
      _qk_protocol_send_code(QK_PACKET_CODE_DATA, qk_protocol_board);
      qk_timer_flags_clear(_QK_PROGRAM_TIMER1, QK_TIMER_FLAG_TIMEOUT);
    }
    break;
  case QK_STATE_STANDBY:
    break;
  case QK_STATE_STOP:
    if(_qk_device->callbacks.stop != 0)
      _qk_device->callbacks.stop();
    _qk_core.currentState = QK_STATE_IDLE;
    break;
#endif
  default:
    _qk_core.currentState = QK_STATE_IDLE;
  }

  /*************************************
   * POWER MANAGEMENT
   ************************************/
#ifdef _QK_FEAT_POWER_MANAGEMENT
//  qk_power_EM1();
#endif
}

void qk_loop()
{
  while(1) { qk_run(); }
}

void _qk_request_state_change(qk_state state)
{
  _qk_core.change_to_state = state;
  _qk_core.flags.reg_internal |= QK_FLAGMASK_INTERNAL_RQSTATECHANGE;
}
void _qk_handle_state_change()
{
  if(_qk_core.flags.reg_internal & QK_FLAGMASK_INTERNAL_RQSTATECHANGE)
  {
    _qk_core.currentState = _qk_core.change_to_state;
    _qk_core.flags.reg_internal &= ~QK_FLAGMASK_INTERNAL_RQSTATECHANGE;
  }
}

#ifdef QK_IS_DEVICE
void qk_sampling_set_mode(qk_sampling_mode mode)
{
  _qk_core.sampling.mode = mode;
}

void qk_sampling_set_trigger(qk_sampling_trigger_clock clock, uint8_t scaler)
{
  _qk_core.sampling.triggerClock = clock;
  _qk_core.sampling.triggerScaler = scaler;
}

void qk_sampling_set_N(uint32_t N)
{
  _qk_core.sampling.N = N;
}

void qk_sampling_set_frequency(uint32_t sampFreq)
{
  if(sampFreq == 0 || sampFreq == _qk_core.sampling.frequency)
  {
    return;
  }
  qk_timer_set_frequency(_QK_PROGRAM_TIMER1, sampFreq);
  _qk_core.sampling.frequency = sampFreq;
  _qk_core.sampling.period = (uint32_t)(1000000.0/(float)sampFreq); // usec
}
void qk_sampling_set_period(uint32_t usec)
{
  uint32_t msec;
  if(usec >= 1000) {
    msec = usec*1000;
    qk_timer_set_period(_QK_PROGRAM_TIMER1, msec, QK_TIMER_SCALE_MSEC);
  }
  else {
    qk_timer_set_period(_QK_PROGRAM_TIMER1, usec, QK_TIMER_SCALE_USEC);
  }
  _qk_core.sampling.period = usec;
  _qk_core.sampling.frequency = (uint32_t)(1000000.0/(float)usec);

}

#endif /*QK_IS_DEVICE*/


static void handle_board_detection()
{
  bool detected = !qk_gpio_get_pin(_QK_HAL_DET); // DET pin is pulled-up
  qk_callback_arg cb_arg;

  if(flag(_qk_core.flags.reg_status, QK_FLAGMASK_STATUS_DET) == detected) {
    return;
  }

  if(detected)
  {
    qk_board_led_blink(2, 50);
    _qk_core.flags.reg_status |= QK_FLAGMASK_STATUS_DET;

    qk_uart_set_baudrate(_QK_PROGRAM_UART, _qk_core.info.baudRate);

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

    qk_uart_set_baudrate(_QK_PROGRAM_UART, _HAL_UART_BAUD_DEFAULT_LOW);

    if(_qk_core.callback[QK_CORE_CALLBACK_BOARDREMOVED] != 0)
      _qk_core.callback[QK_CORE_CALLBACK_BOARDREMOVED](&cb_arg);

#if defined( QK_IS_DEVICE )
//stop
#endif
  }
}

//static void handle_input_changed()
//{
//  handle_board_detection();
//
//
////  if(_qk_core.callbacks.inputChanged != 0) {
////    _qk_core.callbacks.inputChanged();
////  }
//}
