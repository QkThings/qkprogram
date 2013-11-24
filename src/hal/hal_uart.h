/*!
 * \file hal_uart.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define HAL_USE_UART1
#if defined( QK_IS_DEVICE )
#define HAL_UART_COUNT  1
#else
#define HAL_UART_COUNT  2
#define HAL_USE_UART1
#define HAL_USE_UART2
#endif

/******************************************************************************
   ENUMS
 ******************************************************************************/
typedef enum hal_uart_id
{
  HAL_UART_ID_1, // QkConnector
#ifdef HAL_USE_UART2
  HAL_UART_ID_2, // Communication modules
#endif
} hal_uart_id_t;

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct hal_uart_cb
{
  uint8_t data[HAL_UART_RXBUF_SIZE];
  uint16_t i_rd;
  uint16_t i_wr;
  uint16_t count;
  bool overflow;
} hal_uart_cb_t;

typedef struct hal_uart_callbacks
{
  void (*rx)(void);
} hal_uart_callbacks_t;
typedef volatile struct hal_uart_buffers
{
  hal_uart_cb_t rx;
} hal_uart_buffers_t;
typedef volatile struct hal_uart_flags
{
  unsigned discard : 1;
} hal_uart_flags_t;
typedef struct hal_uart
{
  uint32_t baudRate;
  hal_uart_callbacks_t callbacks;
  hal_uart_buffers_t buffers;
  hal_uart_flags_t flags;
} hal_uart_t;

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/

extern hal_uart_t _hal_uart[HAL_UART_COUNT];

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void hal_uart_init();
void hal_uart_reinit();

void hal_uart_enable(hal_uart_id_t uart);
void hal_uart_disable(hal_uart_id_t uart);
void hal_uart_setBaudRate(hal_uart_id_t uart, uint32_t baud);
void hal_uart_writeByte(hal_uart_id_t uart, uint8_t b);
bool hal_uart_cts(hal_uart_id_t uart);
void hal_uart_rts(hal_uart_id_t uart, bool rts);
/******************************************************************************/
void hal_uart_setReadWait(hal_uart_id_t uart, bool en);
uint16_t hal_uart_bytesAvailable(hal_uart_id_t uart);
void hal_uart_writeBytes(hal_uart_id_t uart, uint8_t *buf, uint32_t count);
void hal_uart_writeString(hal_uart_id_t uart, uint8_t *str);
uint8_t hal_uart_readByte(hal_uart_id_t uart);
uint16_t hal_uart_readBytes(hal_uart_id_t uart, uint8_t *buf, uint32_t count);
/******************************************************************************/

#ifdef __cplusplus
}
#endif


#endif /* HAL_UART_H */
