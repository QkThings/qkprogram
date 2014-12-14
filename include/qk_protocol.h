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

#ifndef QK_PROTOCOL_H
#define QK_PROTOCOL_H

/** \addtogroup QkProtocol
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_device.h" // qk_event
#include "qk_packet.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/

//TODO Move this to a more central file (e.g. core? system?)
/**
 * Error
 */
typedef enum
{
  QK_ERR_COMM_TIMEOUT = 0,      //!< QK_ERR_COMM_TIMEOUT
  QK_ERR_CODE_UNKNOWN = 255,    //!< QK_ERR_CODE_UNKNOWN
  QK_ERR_UNABLE_TO_SEND_MESSAGE,//!< QK_ERR_UNABLE_TO_SEND_MESSAGE
  QK_ERR_UNSUPPORTED_OPERATION, //!< QK_ERR_UNSUPPORTED_OPERATION
  QK_ERR_INVALID_BOARD,         //!< QK_ERR_INVALID_BOARD
  QK_ERR_INVALID_DATA_OR_ARG,   //!< QK_ERR_INVALID_DATA_OR_ARG
  QK_ERR_BOARD_NOT_CONNECTED,   //!< QK_ERR_BOARD_NOT_CONNECTED
  QK_ERR_INVALID_SAMP_FREQ,     //!< QK_ERR_INVALID_SAMP_FREQ
  QK_ERR_SAMP_OVERLAP           //!< QK_ERR_SAMP_OVERLAP
} qk_err;

/**
 * ACK type
 */
typedef enum
{
  QK_ACK_TYPE_NACK = 0,//!< QK_ACK_TYPE_NACK
  QK_ACK_TYPE_OK = 1,  //!< QK_ACK_TYPE_OK
  QK_ACK_TYPE_ERROR = 255//!< QK_ACK_TYPE_ERROR
} qk_ack_type;

/**
 * Protocol callback ID
 */
typedef enum qk_protocol_callback_id
{
  QK_PROTOCOL_CALLBACK_WRITE,
  QK_PROTOCOL_CALLBACK_READ,
  QK_PROTOCOL_CALLBACK_SENDPACKET,
  QK_PROTOCOL_CALLBACK_PROCESSPACKET,
  _QK_PROTOCOL_CALLBACK_COUNT
} qk_protocol_callback_id;


/******************************************************************************
   STRUCTS
 ******************************************************************************/

/**
 * ACK
 */
typedef volatile struct
{
  qk_ack_type type; //!< Type
  qk_err err; //!< Error
  int32_t arg; //!< ARgument
} qk_ack;


//typedef struct qk_protocol_callbacks
//{
////  void (*send_bytes)(qk_callback_arg *arg);
////  //void (*send_bytes)(uint8_t *buf, uint8_t count); // required for UART based comm
////  void (*send_packet)(qk_callback_arg *arg);
////  void (*process_bytes)(qk_callback_arg *arg);
////  void (*process_packet)(qk_callback_arg *arg);
//} qk_protocol_callbacks;


typedef volatile struct
{
  uint16_t status;
} qk_protocol_flags;

typedef volatile struct
{
  qk_ack ack;    // Acknowledge
  uint16_t count;       // Count received bytes
} qk_protocol_ctrl;

/**
 * Protocol
 */
typedef volatile struct
{
  qk_packet           packet;
  qk_protocol_ctrl    ctrl;
  qk_protocol_flags   flags;
  qk_callback         callback[_QK_PROTOCOL_CALLBACK_COUNT];
} qk_protocol;

/******************************************************************************
   DEFINES
 ******************************************************************************/

enum
{
  _QK_PROTOCOL_BOARD,  // Qk board
  _QK_PROTOCOL_COMM  // Communication module/peripheral
};

#if defined( QK_IS_DEVICE )
#define QK_PROTOCOL_STRUCT_COUNT 1
#define qk_protocol_board   (&_qk_protocol[_QK_PROTOCOL_BOARD])
#else
#define QK_PROTOCOL_STRUCT_COUNT 2
#define qk_protocol_board   (&_qk_protocol[_QK_PROTOCOL_BOARD])
#define qk_protocol_comm    (&_qk_protocol[_QK_PROTOCOL_COMM])
#endif

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern qk_protocol _qk_protocol[QK_PROTOCOL_STRUCT_COUNT];

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

/**
 * @brief .
 */
static inline
void
qk_protocol_register_callback(qk_protocol *protocol,
                              qk_protocol_callback_id id,
                              qk_callback cb)
{
  _QK_CALLBACK_REGISTER(protocol->callback[id], cb);
}

/**
 * @brief .
 */
void qk_protocol_send_packet(qk_packet *packet, qk_protocol *protocol);

/**
 * @brief .
 */
void qk_protocol_process_bytes(uint8_t *buf, int count, qk_protocol *protocol);

/**
 * @brief .
 */
void qk_protocol_process_byte(uint8_t b, qk_protocol *protocol); //FIXME should be private

/**
 * @brief .
 */
void qk_protocol_process_packet(qk_protocol *protocol);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* QK_PROTOCOL_H */
