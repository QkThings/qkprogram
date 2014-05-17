/*!
 * \file hal_uart.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"
#include "pal.h"
#include "pal_uart.h"

hal_uart_t _hal_uart[HAL_UART_COUNT];



void hal_uart_init()
{

}

void hal_uart_reinit()
{

}

void hal_uart_poll(hal_uart_id_t uart)
{
  uint8_t buf[_HAL_UART_RXBUF_SIZE];
  uint8_t *p_buf = buf;
  uint8_t bytes_read;
  hal_uart_cb_t *rx_cbuf;
  uint8_t i;

  if(uart == HAL_UART_ID_1)
  {
    bytes_read = pal_sio_rx(SIO_2, buf, _HAL_UART_RXBUF_SIZE);

//    if(bytes_read > 0)
//    {
//      buf[bytes_read] = '\0';
//      char echo_str[_HAL_UART_RXBUF_SIZE];
//      sprintf(echo_str, "echo: %s\n", buf);
//      hal_uart_writeString(HAL_UART_ID_1, echo_str);
//    }

    rx_cbuf = (hal_uart_cb_t*)&(_hal_uart[HAL_UART_ID_1].buffers.rx);

    while(bytes_read--)
    {
      rx_cbuf->data[rx_cbuf->i_wr] = *p_buf++;
      rx_cbuf->i_wr = (rx_cbuf->i_wr + 1) % _HAL_UART_RXBUF_SIZE;

      if(rx_cbuf->count >= _HAL_UART_RXBUF_SIZE)
      {
        rx_cbuf->overflow = true;
        rx_cbuf->count = _HAL_UART_RXBUF_SIZE;
      }
      else
        rx_cbuf->count++;
    }
  }
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
  if(uart == HAL_UART_ID_1)
    pal_sio_tx(SIO_2, &b, 1);
}
bool hal_uart_cts(hal_uart_id_t uart)
{

}
void hal_uart_rts(hal_uart_id_t uart, bool rts)
{

}

