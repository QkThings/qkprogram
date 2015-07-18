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
