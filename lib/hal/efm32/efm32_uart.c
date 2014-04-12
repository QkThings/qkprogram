/*!
 * \file hal_uart.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include "em_gpio.h"
#include "em_usart.h"

hal_uart_t _hal_uart[HAL_UART_COUNT];

/******************************************************************************/
#ifdef HAL_USE_UART1
#define u1rxBuf                 _hal_uart[HAL_UART_ID_1].buffers.rx
#define QK_UART1                USART1
#define QK_UART1_RX_IRQn        USART1_RX_IRQn
#define QK_UART1_RX_IRQHandler  USART1_RX_IRQHandler
#endif
#ifdef HAL_USE_UART2
#define u2rxBuf                 _hal_uart[HAL_UART_ID_2].buffers.rxUART
#define QK_UART2                USART0
#define QK_UART2_RX_IRQn        USART0_RX_IRQn
#define QK_UART2_RX_IRQHandler  USART0_RX_IRQHandler
#endif
/******************************************************************************/
#define OVS_MASK  USART_CTRL_OVS_X4
#define OVS_VALUE 4
/******************************************************************************/
static void init(USART_TypeDef *uart, uint8_t location);
static void setBaudRate(USART_TypeDef *uart, uint32_t baud);
static void enable(USART_TypeDef *uart);
static void disable(USART_TypeDef *uart);
static void writeByte(USART_TypeDef *uart, uint8_t b);
static uint32_t calculateBaudRateClkDiv(uint32_t baud, uint32_t ovs);
static void handleRxInterrupt(hal_uart_t *uartHAL, uint8_t rxData);
/******************************************************************************/
USART_TypeDef* getUARTTypeDef(hal_uart_id_t uart)
{
  switch(uart)
  {
  case HAL_UART_ID_1: return QK_UART1;
#ifdef HAL_USE_UART2
  case HAL_UART_ID_2: return QK_UART2;
#endif
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
  GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);

  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_USART1;

  init(QK_UART1, 1);
  setBaudRate(QK_UART1, _HAL_UART_BAUD_DEFAULT_LOW);
  enable(QK_UART1);

  NVIC_ClearPendingIRQ(QK_UART1_RX_IRQn);
  NVIC_EnableIRQ(QK_UART1_RX_IRQn);
  QK_UART1->IEN |= USART_IF_RXDATAV;

#ifdef HAL_USE_UART2
  //TODO implement QK_UART2
  /*u2rxBuf.i_rd = u2rxBuf.i_wr = 0;
  u2rxBuf.count = 0;
  u2rxBuf.overflow = false;

  GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);

  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_USART1;

  uart_init(QK_UART2, 1);
  uart_setBaudRate(QK_UART2, HAL_UART_BAUD_LOW);
  uart_enable(QK_UART2);

  NVIC_ClearPendingIRQ(QK_UART2_RX_IRQn);
  NVIC_EnableIRQ(QK_UART2_RX_IRQn);
  QK_UART2->IEN |= USART_IF_RXDATAV;*/
#endif /*HAL_USE_UART2*/
}

void hal_uart_reinit()
{
  setBaudRate(QK_UART1, _hal_uart[HAL_UART_ID_1].baudRate);
#ifdef HAL_USE_UART2
  setBaudRate(QK_UART2, _hal_uart[HAL_UART_ID_2].baudRate);
#endif
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

  while((QK_UART1->STATUS & USART_STATUS_RXDATAV) && max--)
  {
    rxData = QK_UART1->RXDATA;
    //writeByte(QK_UART1, rxData);
    handleRxInterrupt(&_hal_uart[HAL_UART_ID_1],rxData); //FIXME uncomment
  }
  QK_UART1->IFC = USART_IF_RXDATAV;
}
#endif /*HAL_USE_UART1*/
#ifdef HAL_USE_UART2
void QK_UART2_RX_IRQHandler()
{
  uint8_t max = 2;
  uint32_t rxData;

  while((QK_UART2->STATUS & USART_STATUS_RXDATAV) && max--)
  {
    rxData = QK_UART2->RXDATA;
    //writeByte(QK_UART1, rxData);
    handleRxInterrupt(&_hal_uart[HAL_UART_ID_2],rxData); //FIXME uncomment
    _toggleLED();
  }
  QK_UART2->IFC = UART_IF_RXDATAV;
}
#endif /*HAL_USE_UART2*/
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


