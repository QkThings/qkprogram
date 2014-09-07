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



#define u1rxBuf                 _hal_uart[HAL_UART_ID_1].buffers.rx


#if defined( OLIMEX_STM32_P107_REV_C )
#define UART_SD SD3
#else
#error "ChibiOS board not defined"
#endif


/******************************************************************************/
void hal_uart_init()
{
  sdStart(&UART_SD, NULL);
}

void hal_uart_reinit()
{

}

void hal_uart_poll(hal_uart_id_t uart)
{
  hal_uart_cb_t *rxBuf = (hal_uart_cb_t*)&(_hal_uart[HAL_UART_ID_1].buffers.rx);
  uint16_t i_wr;
  uint8_t rxData = 0;
  uint8_t *p_buf, buf[128];

  int bytes_read = sdRead(&UART_SD, buf, 128);
  p_buf = buf;

  while(bytes_read--)
  {
    rxData = *p_buf++;
    i_wr = rxBuf->i_wr;
    rxBuf->data[i_wr] = rxData;
    rxBuf->i_wr = (i_wr + 1) % _HAL_UART_RXBUF_SIZE;

    if(rxBuf->count >= _HAL_UART_RXBUF_SIZE)
    {
      rxBuf->overflow = true;
      rxBuf->count = _HAL_UART_RXBUF_SIZE;
    }
    else
      rxBuf->count++;
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

}
bool hal_uart_cts(hal_uart_id_t uart)
{
  return true;
}
void hal_uart_rts(hal_uart_id_t uart, bool rts)
{

}



