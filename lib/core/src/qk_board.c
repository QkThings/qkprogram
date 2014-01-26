/*!
 * \file qk_board.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"

qk_board board;
QK_DEFINE_BOARD(board);

static void qk_board_comm_sendBytes(uint8_t *buf, uint8_t count);
static void qk_board_comm_processBytes();
static void qk_board_comm_sendPacket(qk_packet_t *packet);
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
    qk_protocol_processByte(hal_uart_readByte(HAL_UART_ID_1), _protocol_board);
  }
}

static void qk_board_comm_sendPacket(qk_packet_t *packet)
{
  qk_protocol_sendPacket(packet, _protocol_board);
}

static void qk_board_comm_processPacket()
{
  qk_protocol_processPacket(_protocol_board);
}
