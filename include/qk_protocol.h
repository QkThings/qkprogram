/*!
 * \file qk_protocol.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_PROTOCOL_H
#define QK_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_device.h" // qk_event
#include "qk_packet.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/
typedef enum
{
  QK_ERR_COMM_TIMEOUT = 0,
  QK_ERR_CODE_UNKNOWN = 255,
  QK_ERR_UNABLE_TO_SEND_MESSAGE,
  QK_ERR_UNSUPPORTED_OPERATION,
  QK_ERR_INVALID_BOARD,
  QK_ERR_INVALID_DATA_OR_ARG,
  QK_ERR_BOARD_NOT_CONNECTED,
  QK_ERR_INVALID_SAMP_FREQ,
  QK_ERR_SAMP_OVERLAP
} qk_err;

typedef enum
{
  QK_ACK_TYPE_NACK,
  QK_ACK_TYPE_OK,
  QK_ACK_TYPE_ERROR
} qk_ack_type;

typedef enum qk_protocol_callback_id
{
  QK_PROTOCOL_CALLBACK_SENDBYTES,
  QK_PROTOCOL_CALLBACK_SENDPACKET,
  QK_PROTOCOL_CALLBACK_PROCESSBYTES,
  QK_PROTOCOL_CALLBACK_PROCESSPACKET,
  _QK_PROTOCOL_CALLBACK_COUNT
} qk_protocol_callback_id;


/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct qk_ack
{
  qk_ack_type type;
  qk_err err;
  int32_t arg;
} qk_ack;


//typedef struct qk_protocol_callbacks
//{
////  void (*send_bytes)(qk_callback_arg *arg);
////  //void (*send_bytes)(uint8_t *buf, uint8_t count); // required for UART based comm
////  void (*send_packet)(qk_callback_arg *arg);
////  void (*process_bytes)(qk_callback_arg *arg);
////  void (*process_packet)(qk_callback_arg *arg);
//} qk_protocol_callbacks;


typedef volatile struct qk_comm_flags
{
  uint16_t reg;
} qk_protocol_flags;

typedef volatile struct qk_comm_ctrl
{
  qk_ack ack;    // Acknowledge
  uint16_t count;       // Count received bytes
} qk_protocol_ctrl;

typedef struct qk_protocol
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

static inline void
qk_protocol_register_callback(qk_protocol *protocol,
                              qk_protocol_callback_id id,
                              qk_callback cb)
{
  _QK_CALLBACK_REGISTER(protocol->callback[id], cb);
}

void qk_protocol_send_packet(qk_packet *packet, qk_protocol *protocol);
void qk_protocol_process_byte(uint8_t b, qk_protocol *protocol);
void qk_protocol_process_packet(qk_protocol *protocol);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* QK_PROTOCOL_H */
