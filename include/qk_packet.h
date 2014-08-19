#ifndef QK_PACKET_H_
#define QK_PACKET_H_

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
