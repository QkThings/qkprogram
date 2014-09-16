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

qk_board board;
QK_DEFINE_BOARD(board);

static void board_callback_send_bytes(qk_callback_arg *arg);
static void board_callback_process_bytes(qk_callback_arg *arg);
static void board_callback_send_packet(qk_callback_arg *arg);
static void board_callback_process_packet(qk_callback_arg *arg);


void qk_board_init()
{
  memset(&board, 0, sizeof(qk_board));
  qk_protocol_init(qk_protocol_board);

  qk_protocol_register_callback(qk_protocol_board,
                                QK_PROTOCOL_CALLBACK_SENDBYTES,
                                board_callback_send_bytes);

  qk_protocol_register_callback(qk_protocol_board,
                                QK_PROTOCOL_CALLBACK_PROCESSBYTES,
                                board_callback_process_bytes);

  qk_protocol_register_callback(qk_protocol_board,
                                QK_PROTOCOL_CALLBACK_SENDPACKET,
                                board_callback_send_packet);

  qk_protocol_register_callback(qk_protocol_board,
                                QK_PROTOCOL_CALLBACK_PROCESSPACKET,
                                board_callback_process_packet);


#ifdef QK_IS_DEVICE
  _qk_device_init();
#endif
#ifdef QK_IS_COMM
  _qk_comm_init();
#endif
}

void qk_board_setup()
{
#if defined( QK_IS_DEVICE )
  _qk_device_setup();
#endif

  if(_qk_board->callbacks.init != 0) {
    _qk_board->callbacks.init();
  }
  if(_qk_board->callbacks.config != 0) {
    _qk_board->callbacks.config();
  }
}

void qk_board_ready()
{
  _qk_protocol_send_code(QK_PACKET_CODE_READY, qk_protocol_board);
}

void qk_board_led_set(bool state)
{
  if(state)
    qk_gpio_set_pin(_QK_HAL_LED, false);
  else
    qk_gpio_set_pin(_QK_HAL_LED, true);
}

void qk_board_led_blink(unsigned int n, unsigned int msec)
{
  do
  {
    qk_gpio_set_pin(_QK_HAL_LED, false);
    delay_ms(msec);
    qk_gpio_set_pin(_QK_HAL_LED, true);
    delay_ms(msec);
  }
  while(--n > 0);
}


static void board_callback_send_bytes(qk_callback_arg *arg)
{
  qk_uart_write(_QK_PROGRAM_UART,
                (uint8_t*) QK_BUF_PTR( QK_CALLBACK_ARG_BUF(arg) ),
                (uint16_t) QK_BUF_COUNT( QK_CALLBACK_ARG_BUF(arg) ));
}

static void board_callback_process_bytes(qk_callback_arg *arg)
{
  uint8_t buf[32], *p_buf;
  int count = qk_uart_read(_QK_PROGRAM_UART, buf, 32);

  p_buf = buf;

  while(count--)
  {
    qk_protocol_process_byte(*p_buf++, qk_protocol_board);
  }
}

static void board_callback_send_packet(qk_callback_arg *arg)
{
#ifndef _QK_PROGRAM_DEV_DONTSENDPACKET
  qk_packet *packet = (qk_packet*) QK_CALLBACK_ARG_PTR(arg);
  qk_protocol_send_packet(packet, qk_protocol_board);
#endif
}

static void board_callback_process_packet(qk_callback_arg *arg)
{
  qk_protocol_process_packet(qk_protocol_board);
}

void qk_board_set_name(const char *name)
{
  strcpy((char*)_qk_board->info.name, name);
}

void qk_board_set_version(uint16_t version)
{
  _qk_board->info.version = version;
}

void qk_config_set_buffer(qk_config *buffer, uint8_t count)
{
  _qk_board->buffers.config = buffer;
  _qk_board->info._ncfg = count;
}

void qk_config_set_label(uint8_t idx, const char *label)
{
  strcpy((char*)_qk_board->buffers.config[idx].proprieties.label, label);
}

void qk_config_set_type(uint8_t idx, qk_config_type type)
{
  _qk_board->buffers.config[idx].type = type;
}

void qk_config_set_value_b(uint8_t idx, bool value)
{
  _qk_board->buffers.config[idx].value.b = value;
}

void qk_config_set_value_i(uint8_t idx, int32_t value)
{
  _qk_board->buffers.config[idx].value.i = value;
}

void qk_config_set_value_f(uint8_t idx, float value)
{
  _qk_board->buffers.config[idx].value.f = value;
}

void qk_config_set_value_dt(uint8_t idx, qk_datetime dateTime)
{
  _qk_board->buffers.config[idx].value.dateTime = dateTime;
}

uint8_t qk_config_count()
{
  return _qk_board->info._ncfg;
}

qk_config_type qk_config_get_type(uint8_t idx)
{
  return _qk_board->buffers.config[idx].type;
}


