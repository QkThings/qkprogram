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
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define _BAUD _HAL_UART_BAUD_DEFAULT_LOW
#define _UBRR ((F_CPU / (_BAUD * 16UL)) - 1)

//-----------------------------------------------------------------------------
int _write_byte(char data, FILE *stream)
{
  hal_uart_writeByte(HAL_UART_ID_1, data);
  if(data == '\n')
    hal_uart_writeByte(HAL_UART_ID_1, '\r');
  return 0;
}
int _read_byte(FILE *stream)
{
  hal_uart_readByte(HAL_UART_ID_1);
  return (int)hal_uart_readByte(HAL_UART_ID_1);
}
FILE _usart_stream = FDEV_SETUP_STREAM(_write_byte, _read_byte, _FDEV_SETUP_RW);
//-----------------------------------------------------------------------------
void hal_uart_init()
{
  UBRR0H = (uint8_t)(_UBRR>>8); // Set baud rate
  UBRR0L = (uint8_t)_UBRR;
  UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable receiver and transmitter
  UCSR0C = (1<<USBS0)|(3<<UCSZ00); // Set frame format: 8data, 2stop bit
  UCSR0B |= (1 << RXCIE0); // Enable the USART Recieve Complete interrupt (USART_RXC);

  stdin=stdout=&_usart_stream;
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
  if(uart == HAL_UART_ID_1)
  {
    UDR0 = b; // Put data into buffer, sends the data
    while ( !( UCSR0A & (1<<UDRE0)) ); // Wait for empty transmit buffer
  }
}
bool hal_uart_cts(hal_uart_id_t uart)
{
  return true;
}
void hal_uart_rts(hal_uart_id_t uart, bool rts)
{

}
//-----------------------------------------------------------------------------
ISR(USART_RX_vect)
{
  hal_uart_cb_t *rxBuf = (hal_uart_cb_t*)&(_hal_uart[HAL_UART_ID_1].buffers.rx);
  uint16_t i_wr;
  uint8_t rxData = 0;

  rxData = UDR0;
  //hal_uart_writeByte(0, rxData);

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

