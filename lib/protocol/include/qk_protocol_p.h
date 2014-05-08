/*!
 * \file qk_protocol.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_PROTOCOL_P_H
#define QK_PROTOCOL_P_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_device.h" // qk_event
#include "qk_packet.h"

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define QK_PROTOCOL_FLAGMASK_TX           0x0001
#define QK_PROTOCOL_FLAGMASK_RX           0x0002
#define QK_PROTOCOL_FLAGMASK_SEQ          0x0004
#define QK_PROTOCOL_FLAGMASK_DLE          0x0008
#define QK_PROTOCOL_FLAGMASK_VALID        0x0010
#define QK_PROTOCOL_FLAGMASK_NEWPACKET    0x0020
#define QK_PROTOCOL_FLAGMASK_REXMIT       0x0040
#define QK_PROTOCOL_FLAGMASK_ACKREXMIT    0x0080

#define QK_PROTOCOL_CTRL_FLAG             0x55    // Flag
#define QK_PROTOCOL_CTRL_DLE              0xDD    // Data Link Escape

#define QK_PROTOCOL_NACK                  0x00

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_protocol_init(qk_protocol *protocol);

void qk_protocol_build_packet(qk_packet *packet, qk_packet_descriptor *desc, qk_protocol *protocol);
void _qk_protocol_send_code(int code, qk_protocol *protocol);
void _qk_protocol_send_string(const char *str, qk_protocol *protocol);

#if defined( QK_IS_DEVICE )
void _qk_protocol_send_event(qk_event *e, qk_protocol *protocol);
#endif
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* QK_PROTOCOL_H */
