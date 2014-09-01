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

#ifndef QK_PACKET_H
#define QK_PACKET_H

/** \addtogroup QkPacket
 *  @{
 */

#include "qk_settings.h"

/******************************************************************************
   DEFINES
 ******************************************************************************/


/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef volatile struct qk_packet_flags
{
  uint16_t ctrl;
  uint8_t network;
} qk_packet_flags;

/**
 * Packet
 */
typedef volatile struct qk_packet
{
  qk_packet_flags flags;
  uint64_t address;
  uint8_t  id;
  uint8_t  code;
  uint8_t  payload[_QK_PACKET_PAYLOAD_SIZE];
  uint8_t  checksum;
  uint8_t  header_lenght;
  uint8_t  payload_lenght;
} qk_packet;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

/** @}*/

#endif /* QK_PACKET_H_ */
