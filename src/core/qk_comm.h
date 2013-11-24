/*!
 * \file qk_comm.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_COMM_H
#define QK_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_device.h" // qk_event_t

/******************************************************************************
   ENUMS
 ******************************************************************************/
typedef enum qk_error
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
} qk_error_t;

enum {
  _QK_COMM_BOARD,  // Qk board
  _QK_COMM_PERIPH  // Communication module/peripheral
};

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct qk_comm_ack
{
  uint32_t arg;
  uint8_t code;
  uint8_t err;
} qk_comm_ack_t;

typedef struct qk_comm_callbacks
{
  void (*sendBytes)(uint8_t *buf, uint8_t count); // required for UART based comm
  void (*sendPacket)(qk_packet_t *packet);
  void (*processBytes)(void);
  void (*processPacket)(void);
} qk_comm_callbacks_t;

typedef volatile struct qk_comm_flags
{
  uint16_t reg;
} qk_comm_flags_t;

typedef volatile struct qk_comm_ctrl
{
  qk_comm_ack_t ack;    // Acknowledge
  uint16_t count;       // Count received bytes
} qk_comm_ctrl_t;

typedef struct qk_comm_t
{
  qk_packet_t    packet;     // RX packet
  qk_comm_ctrl_t      ctrl;
  qk_comm_flags_t     flags;
  qk_comm_callbacks_t callbacks;  // Callbacks
} qk_comm_t;

/******************************************************************************
   OTHER DATA TYPES
 ******************************************************************************/
typedef int8_t err_t;

/******************************************************************************
   DEFINES
 ******************************************************************************/
#if defined( QK_IS_DEVICE )
#define QK_COMM_STRUCT_COUNT 1
#define _comm_board   (&_qk_comm[_QK_COMM_BOARD])
#else
#define QK_COMM_STRUCT_COUNT 2
#define _comm_board   (&_qk_comm[_QK_COMM_BOARD])
#define _comm_periph  (&_qk_comm[_QK_COMM_PERIPH])
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
extern qk_comm_t _qk_comm[QK_COMM_STRUCT_COUNT];

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_comm_init(qk_comm_t *comm);
void qk_comm_sendPacket(qk_packet_t *packet, qk_comm_t *comm);
void qk_comm_processByte(uint8_t b, qk_comm_t *comm);
void qk_comm_processPacket(qk_comm_t *comm);

void qk_comm_buildPacket(qk_packet_t *packet, qk_packet_descriptor_t *desc, qk_comm_t *comm);
void _qk_comm_sendCode(int code, qk_comm_t *comm);
void _qk_comm_sendString(const char *str, qk_comm_t *comm);

#if defined( QK_IS_DEVICE )
void _qk_comm_sendEvent(qk_event_t *e, qk_comm_t *comm);
#endif
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* QK_COMM_H */
