/*!
 * \file hal_uart.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"


hal_uart_t _hal_uart[HAL_UART_COUNT];



void hal_uart_init()
{

}

void hal_uart_reinit()
{

}

void hal_uart_enable(hal_uart_id_t uart)
{

}
void hal_uart_disable(hal_uart_id_t uart)
{

}
void hal_uart_setBaudRate(hal_uart_id_t uart, uint32_t baud)
{

}
void hal_uart_writeByte(hal_uart_id_t uart, uint8_t b)
{

}
bool hal_uart_cts(hal_uart_id_t uart)
{

}
void hal_uart_rts(hal_uart_id_t uart, bool rts)
{

}

static void handleRxInterrupt(hal_uart_t *uartHAL, uint8_t rxData)
{
  uint32_t i_wr;
  hal_uart_cb_t *rxBuf = (hal_uart_cb_t*)&(uartHAL->buffers.rx);

  i_wr = rxBuf->i_wr;
  rxBuf->data[i_wr] = rxData;
  rxBuf->i_wr = (i_wr + 1) % _HAL_UART_RXBUF_SIZE;
  rxBuf->count++;

  if(rxBuf->count > _HAL_UART_RXBUF_SIZE)
  {
    rxBuf->overflow = true;
  }
}



