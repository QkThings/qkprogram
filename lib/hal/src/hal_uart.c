/*!
 * \file hal_uart.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"

/******************************************************************************/
static uint8_t readByte(hal_uart_cb_t *cb, bool wait);
static uint16_t readBytes(hal_uart_cb_t *cb, uint8_t *buf, uint16_t count, bool wait);
/******************************************************************************/

hal_uart_t _hal_uart[HAL_UART_COUNT];

bool uart1_readWait = false;
bool uart2_readWait = false;

void _hal_uart_init()
{
//  memset(&_hal_uart[HAL_UART_ID_1], 0, sizeof(hal_uart_t));
//#ifdef HAL_USE_UART2
//  memset(&_hal_uart[HAL_UART_ID_2], 0, sizeof(hal_uart_t));
//#endif
}

static bool getReadWait(hal_uart_id_t uart)
{
  switch(uart)
  {
  case HAL_UART_ID_1: return uart1_readWait;
#ifdef HAL_USE_UART2
  case HAL_UART_ID_2: return uart2_readWait;
#endif
  }
  return false;
}

void hal_uart_setReadWait(hal_uart_id_t uart, bool en)
{
  switch(uart)
  {
  case HAL_UART_ID_1: uart1_readWait = en; break;
#ifdef HAL_USE_UART2
  case HAL_UART_ID_2: uart2_readWait = en; break;
#endif
  }
}

uint16_t hal_uart_bytesAvailable(hal_uart_id_t uart)
{
  return _hal_uart[uart].buffers.rx.count;
}

void hal_uart_writeBytes(hal_uart_id_t uart, uint8_t *buf, uint32_t count)
{
  while(count--) {
    hal_uart_writeByte(uart, *buf++);
  }
}
void hal_uart_writeString(hal_uart_id_t uart, uint8_t *str)
{
  while(*str != '\0') {
    hal_uart_writeByte(uart, *str++);
  }
}

uint8_t hal_uart_readByte(hal_uart_id_t uart)
{
  hal_uart_cb_t *rxBuf = (hal_uart_cb_t*)&(_hal_uart[uart].buffers.rx);
  return readByte(rxBuf, getReadWait(uart));
}

uint16_t hal_uart_readBytes(hal_uart_id_t uart, uint8_t *buf, uint32_t count)
{
  hal_uart_cb_t *rxBuf = (hal_uart_cb_t*)&(_hal_uart[uart].buffers.rx);
  return readBytes(rxBuf, buf, count, getReadWait(uart));
}
/******************************************************************************/
static uint8_t readByte(hal_uart_cb_t *cb, bool wait)
{
  bool read = false;
  uint8_t b;
  uint16_t i_rd;

  if (wait && cb->count < 1) {
    while (cb->count < 1) {
      hal_power_EM1();
    }
  }
  if(cb->count >= 1) {
    read = true;
  }
  else {
    if(wait) {
      while (cb->count < 1) {
        hal_power_EM1();
      }
      read = true;
    }
  }

  if(read) {
    i_rd = cb->i_rd;
    b = cb->data[i_rd];
    cb->i_rd = (i_rd + 1) % HAL_UART_RXBUF_SIZE;
    cb->count--;
  }

  return b;
}

static uint16_t readBytes(hal_uart_cb_t *cb, uint8_t *buf, uint16_t count, bool wait)
{
  uint16_t i;
  uint16_t i_rd;

  if(wait) {
    if (cb->count < count) {
      while (cb->count < count)
        hal_power_EM1();
    }
  }

  if (count == 0) {
    count = (cb->count);
  }

  for(i = 0; i < count; i++) {
    i_rd = cb->i_rd;
    buf[i] = cb->data[i_rd];
    cb->i_rd = (i_rd + 1) % HAL_UART_RXBUF_SIZE;
  }
  cb->count -= count;

  return i;
}
