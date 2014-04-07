/*!
 * \file qk_board.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"

qk_board board;
QK_DEFINE_BOARD(board);

static void qk_board_comm_sendBytes(uint8_t *buf, uint8_t count);
static void qk_board_comm_processBytes();
static void qk_board_comm_sendPacket(qk_packet *packet);
static void qk_board_comm_processPacket();

void qk_board_init()
{
  memset(&board, 0, sizeof(qk_board));
  qk_protocol_init(_protocol_board);
  _protocol_board->callback.sendBytes = qk_board_comm_sendBytes;
  _protocol_board->callback.processBytes = qk_board_comm_processBytes;
  _protocol_board->callback.sendPacket = qk_board_comm_sendPacket;
  _protocol_board->callback.processPacket = qk_board_comm_processPacket;

  hal_uart_enable(HAL_UART_ID_1);

#if defined( QK_IS_DEVICE )
  _qk_device_init();
#endif

#if !defined( QK_IS_DEVICE )
  qk_protocol_init(_comm_periph);
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

static void qk_board_comm_sendBytes(uint8_t *buf, uint8_t count)
{
  hal_uart_writeBytes(HAL_UART_ID_1, buf, count);
}

static void qk_board_comm_processBytes()
{
  uint16_t count = hal_uart_bytesAvailable(HAL_UART_ID_1);
  while(count--)
  {
    qk_protocol_process_byte(hal_uart_readByte(HAL_UART_ID_1), _protocol_board);
  }
}

static void qk_board_comm_sendPacket(qk_packet *packet)
{
  qk_protocol_send_packet(packet, _protocol_board);
}

static void qk_board_comm_processPacket()
{
  qk_protocol_process_packet(_protocol_board);
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

