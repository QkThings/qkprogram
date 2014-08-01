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

  hal_uart_enable(HAL_UART_ID_1);

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


static void board_callback_send_bytes(qk_callback_arg *arg)
{
  uint8_t *buf = (uint8_t*) QK_BUF_PTR( QK_CALLBACK_ARG_BUF(arg) );
  uint16_t count = (uint16_t) QK_BUF_COUNT( QK_CALLBACK_ARG_BUF(arg) );
  hal_uart_writeBytes(HAL_UART_ID_1, buf, count);
}

static void board_callback_process_bytes(qk_callback_arg *arg)
{
  uint16_t count = hal_uart_bytesAvailable(HAL_UART_ID_1);
  while(count--)
  {
    qk_protocol_process_byte(hal_uart_readByte(HAL_UART_ID_1), qk_protocol_board);
  }
}

static void board_callback_send_packet(qk_callback_arg *arg)
{
  qk_packet *packet = (qk_packet*) QK_CALLBACK_ARG_PTR(arg);
  qk_protocol_send_packet(packet, qk_protocol_board);
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

