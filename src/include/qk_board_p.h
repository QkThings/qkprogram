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

#ifndef QK_BOARD_P_H
#define QK_BOARD_P_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_board.h"
#include "qk_settings.h"
#include "qk_utils.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/

typedef enum
{
  QK_BOARD_TYPE_HOST,
  QK_BOARD_TYPE_COMM,
  QK_BOARD_TYPE_DEVICE
} qk_board_type;

/******************************************************************************
   STRUCTS
 ******************************************************************************/

typedef struct qk_board_info
{
  uint64_t address;
  uint16_t version;
  char     name[_QK_BOARD_NAME_SIZE];
  uint8_t  _ncfg;
} qk_board_info;

typedef struct qk_board_buffer
{
  qk_config *config;
} qk_board_buffer;

typedef struct qk_board_callbacks
{
  void (*init)(void);
  void (*config)(void);
} qk_board_callbacks;

typedef struct qk_board
{
  qk_board_info       info;
  qk_board_buffer     buffers;
  qk_board_callbacks  callbacks;
#if defined( QK_IS_COMM )
  qk_comm   comm;
#elif defined( QK_IS_DEVICE )
  qk_device device;
#endif
} qk_board;

/******************************************************************************
   DEFINES
 ******************************************************************************/

#if defined( QK_IS_COMM )
#define QK_BOARD_TYPE QK_BOARD_TYPE_COMM
#elif defined( QK_IS_DEVICE )
#define QK_BOARD_TYPE QK_BOARD_TYPE_DEVICE
#endif

#if defined( QK_IS_COMM )
#define QK_DEFINE_BOARD(name)   qk_board *_qk_board = &name; \
                                QK_DEFINE_COMM(name.comm)
#elif defined( QK_IS_DEVICE )
#define QK_DEFINE_BOARD(name)   qk_board *_qk_board = &name; \
                                QK_DEFINE_DEVICE(name.device)
#endif

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/

extern qk_board *_qk_board;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_board_init();
void qk_board_setup();
void qk_board_ready();
bool qk_board_hwfc_in();
void qk_board_hwfc_out(bool state);

#include <qk_debug.h>

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* QK_BOARD_H */
