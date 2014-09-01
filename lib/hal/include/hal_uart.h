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

#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define HAL_USE_UART1

#define HAL_UART_COUNT 1


/******************************************************************************
   ENUMS
 ******************************************************************************/
typedef enum hal_uart_id
{
  HAL_UART_ID_1, // QkConnector
} hal_uart_id_t;

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct hal_uart_cb
{
  uint8_t data[_HAL_UART_RXBUF_SIZE];
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
void _hal_uart_init();
/******************************************************************************/
void hal_uart_init();
void hal_uart_reinit();
void hal_uart_enable(hal_uart_id_t uart);
void hal_uart_disable(hal_uart_id_t uart);
void hal_uart_setBaudRate(hal_uart_id_t uart, uint32_t baud);
void hal_uart_writeByte(hal_uart_id_t uart, uint8_t b);
bool hal_uart_cts(hal_uart_id_t uart);
void hal_uart_rts(hal_uart_id_t uart, bool rts);
/******************************************************************************/
#ifdef _QK_HAL_UART_POLLING
void hal_uart_poll(hal_uart_id_t uart);
#endif
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
