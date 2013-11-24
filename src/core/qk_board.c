/*!
 * \file qk_board.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"

qk_board_t board;
QK_DEFINE_BOARD(board);

static void qk_board_comm_sendBytes(uint8_t *buf, uint8_t count);
static void qk_board_comm_processBytes();
static void qk_board_comm_sendPacket(qk_packet_t *packet);
static void qk_board_comm_processPacket();

void qk_board_init()
{
  memset(&board, 0, sizeof(qk_board_t));
  qk_comm_init(_comm_board);
  _comm_board->callbacks.sendBytes = qk_board_comm_sendBytes;
  _comm_board->callbacks.processBytes = qk_board_comm_processBytes;
  _comm_board->callbacks.sendPacket = qk_board_comm_sendPacket;
  _comm_board->callbacks.processPacket = qk_board_comm_processPacket;

  hal_uart_enable(HAL_UART_ID_1);

#if defined( QK_IS_DEVICE )
  _qk_device_init();
#endif

#if !defined( QK_IS_DEVICE )
  qk_comm_init(_comm_periph);
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
    qk_comm_processByte(hal_uart_readByte(HAL_UART_ID_1), _comm_board);
  }
}

static void qk_board_comm_sendPacket(qk_packet_t *packet)
{
  qk_comm_sendPacket(packet, _comm_board);
}

static void qk_board_comm_processPacket()
{
  qk_comm_processPacket(_comm_board);
}
