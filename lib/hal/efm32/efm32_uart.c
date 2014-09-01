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
#include "em_gpio.h"
#include "em_usart.h"

hal_uart_t _hal_uart[HAL_UART_COUNT];


#define u1rxBuf                 _hal_uart[HAL_UART_ID_1].buffers.rx

#define QK_UART1                USART0
#define QK_UART1_RX_IRQn        USART0_RX_IRQn
#define QK_UART1_RX_IRQHandler  USART0_RX_IRQHandler
#define QK_UART1_LOC            0
#define QK_UART1_TX_PORT        gpioPortE
#define QK_UART1_TX_PIN         10
#define QK_UART1_RX_PORT        gpioPortE
#define QK_UART1_RX_PIN         11



#define OVS_MASK  USART_CTRL_OVS_X4
#define OVS_VALUE 4

static void init(USART_TypeDef *uart, uint8_t location);
static void setBaudRate(USART_TypeDef *uart, uint32_t baud);
static void enable(USART_TypeDef *uart);
static void disable(USART_TypeDef *uart);
static void writeByte(USART_TypeDef *uart, uint8_t b);
static uint32_t calculateBaudRateClkDiv(uint32_t baud, uint32_t ovs);
static void handleRxInterrupt(hal_uart_t *uartHAL, uint8_t rxData);


USART_TypeDef* getUARTTypeDef(hal_uart_id_t uart)
{
  switch(uart)
  {
  case HAL_UART_ID_1: return QK_UART1;
  }
  return 0;
}
hal_uart_t* getUARTHAL(hal_uart_id_t uart)
{
  return &(_hal_uart[uart]);
}
/******************************************************************************/
void hal_uart_init()
{
  GPIO_PinModeSet(QK_UART1_TX_PORT, QK_UART1_TX_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(QK_UART1_RX_PORT, QK_UART1_RX_PIN, gpioModeInput, 0);

  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_USART0;

  init(QK_UART1, QK_UART1_LOC);
  setBaudRate(QK_UART1, _HAL_UART_BAUD_DEFAULT_LOW);
  enable(QK_UART1);

  NVIC_ClearPendingIRQ(QK_UART1_RX_IRQn);
  NVIC_EnableIRQ(QK_UART1_RX_IRQn);
  QK_UART1->IEN |= USART_IF_RXDATAV;
}

void hal_uart_reinit()
{
  setBaudRate(QK_UART1, _hal_uart[HAL_UART_ID_1].baudRate);
}

void hal_uart_poll(hal_uart_id_t uart)
{

}

void hal_uart_enable(hal_uart_id_t uart)
{
  enable(getUARTTypeDef(uart));
}
void hal_uart_disable(hal_uart_id_t uart)
{
  disable(getUARTTypeDef(uart));
}
void hal_uart_setBaudRate(hal_uart_id_t uart, uint32_t baud)
{
  setBaudRate(getUARTTypeDef(uart), baud);
  _hal_uart[uart].baudRate = baud;
}
void hal_uart_writeByte(hal_uart_id_t uart, uint8_t b)
{
  writeByte(getUARTTypeDef(uart), b);
}
bool hal_uart_cts(hal_uart_id_t uart)
{
  return true;
}
void hal_uart_rts(hal_uart_id_t uart, bool rts)
{

}

/******************************************************************************/
#ifdef HAL_USE_UART1
void QK_UART1_RX_IRQHandler()
{
  uint8_t max = 2;
  uint32_t rxData;

//  hal_toggleLED();

  while((QK_UART1->STATUS & USART_STATUS_RXDATAV) && max--)
  {
    rxData = QK_UART1->RXDATA;
//    writeByte(QK_UART1, 'A');
    handleRxInterrupt(&_hal_uart[HAL_UART_ID_1],rxData); //FIXME uncomment
  }
  QK_UART1->IFC = USART_IF_RXDATAV;
}
#endif /*HAL_USE_UART1*/

/******************************************************************************/
static void init(USART_TypeDef *uart, uint8_t location)
{
  uart->FRAME &= ~(_USART_FRAME_DATABITS_MASK |
                   _USART_FRAME_PARITY_MASK |
                   _USART_FRAME_STOPBITS_MASK);
  uart->FRAME |= (USART_FRAME_DATABITS_EIGHT |
                  USART_FRAME_PARITY_NONE |
                  USART_FRAME_STOPBITS_ONE);

  uart->CTRL &= ~_USART_CTRL_OVS_MASK;
  uart->CTRL |= OVS_MASK;

  uart->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;

  uart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | (location << 8);
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

static void setBaudRate(USART_TypeDef *uart, uint32_t baud)
{
  uart->CLKDIV = calculateBaudRateClkDiv(baud, OVS_VALUE);
}

static void enable(USART_TypeDef *uart)
{
  uart->CMD = USART_CMD_RXEN | USART_CMD_TXEN;
}

static void disable(USART_TypeDef *uart)
{
  uart->CMD = USART_CMD_RXDIS | USART_CMD_TXDIS;
}

static void writeByte(USART_TypeDef *uart, uint8_t b)
{
  uart->TXDATA = (uint32_t)b;
  while (!(uart->STATUS & USART_STATUS_TXBL));
}

static uint32_t calculateBaudRateClkDiv(uint32_t baud, uint32_t ovs)
{
  uint32_t clkdiv;
  clkdiv  = 4 * _hal_clock.freq_value + (ovs * baud) / 2;
  clkdiv /= (ovs * baud);
  clkdiv -= 4;
  clkdiv *= 64;
  return clkdiv;
}


