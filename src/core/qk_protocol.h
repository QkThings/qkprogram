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

enum
{
  _QK_PROTOCOL_BOARD,  // Qk board
  _QK_PROTOCOL_PERIPH  // Communication module/peripheral
};

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct qk_protocol_ack
{
  uint32_t arg;
  uint8_t code;
  uint8_t err;
} qk_protocol_ack;

typedef struct qk_protocol_callback
{
  void (*sendBytes)(uint8_t *buf, uint8_t count); // required for UART based comm
  void (*sendPacket)(qk_packet_t *packet);
  void (*processBytes)(void);
  void (*processPacket)(void);
} qk_protocol_callback;

typedef volatile struct qk_comm_flags
{
  uint16_t reg;
} qk_protocol_flags;

typedef volatile struct qk_comm_ctrl
{
  qk_protocol_ack ack;    // Acknowledge
  uint16_t count;       // Count received bytes
} qk_protocol_ctrl;

typedef struct qk_comm_t
{
  qk_packet_t           packet;     // RX packet
  qk_protocol_ctrl      ctrl;
  qk_protocol_flags     flags;
  qk_protocol_callback  callback;  // Callbacks
} qk_protocol;

/******************************************************************************
   DEFINES
 ******************************************************************************/
#if defined( QK_IS_DEVICE )
#define QK_PROTOCOL_STRUCT_COUNT 1
#define _protocol_board   (&_qk_protocol[_QK_PROTOCOL_BOARD])
#else
#define QK_PROTOCOL_STRUCT_COUNT 2
#define _protocol_board   (&_qk_protocol[_QK_PROTOCOL_BOARD])
#define _protocol_periph  (&_qk_protocol[_QK_PROTOCOL_PERIPH])
#endif
/******************************************************************************/
#define QK_COMM_FLAGMASK_TX           0x0001
#define QK_COMM_FLAGMASK_RX           0x0002
#define QK_COMM_FLAGMASK_SEQ          0x0004
#define QK_COMM_FLAGMASK_DLE          0x0008
#define QK_COMM_FLAGMASK_VALID        0x0010
#define QK_COMM_FLAGMASK_NEWPACKET    0x0020
#define QK_COMM_FLAGMASK_REXMIT       0x0040
#define QK_COMM_FLAGMASK_ACKREXMIT    0x0080
/******************************************************************************/
#define QK_COMM_CTRL_FLAG             0x55    // Flag
#define QK_COMM_CTRL_DLE              0xDD    // Data Link Escape
/******************************************************************************/
#define QK_COMM_NACK                  0x00
/******************************************************************************/


/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern qk_protocol _qk_protocol[QK_PROTOCOL_STRUCT_COUNT];

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_protocol_init(qk_protocol *comm);
void qk_protocol_sendPacket(qk_packet_t *packet, qk_protocol *comm);
void qk_protocol_processByte(uint8_t b, qk_protocol *comm);
void qk_protocol_processPacket(qk_protocol *comm);

void qk_protocol_buildPacket(qk_packet_t *packet, qk_packet_descriptor *desc, qk_protocol *comm);
void _qk_protocol_sendCode(int code, qk_protocol *comm);
void _qk_protocol_sendString(const char *str, qk_protocol *comm);

#if defined( QK_IS_DEVICE )
void _qk_protocol_sendEvent(qk_event *e, qk_protocol *comm);
#endif
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* QK_PROTOCOL_H */
