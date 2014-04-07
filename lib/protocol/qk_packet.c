/*
 * qk_packet.c
 *
 *  Created on: Mar 11, 2014
 *      Author: mribeiro
 */
#include "qk_system.h"

void packet_calc_header_length(qk_packet *packet)
{
  packet->hdrLen = SIZE_FLAGS_CTRL + SIZE_CODE + SIZE_ID;
}


uint64_t packet_get_value(uint8_t byteCount, uint16_t *idx, qk_packet *packet)
{
  uint16_t j, i = *idx;
  uint64_t value = 0;

  for(j = 0; j < byteCount; j++)
    value += (uint64_t)(((uint64_t)(packet->data[i++]) & 0xFF) << (8*j)); // LSB first

  *idx = i;

  switch(byteCount) // truncate
  {
  case 1:
      value &= 0xFF;
      break;
  case 2:
      value &= 0xFFFF;
      break;
  case 4:
      value &= 0xFFFFFFFF;
      break;
  }
  return value;
}

void packet_get_string(char *buf, uint16_t count, uint16_t *idx, qk_packet *packet)
{
  uint16_t j, i = *idx;
  char c;

  memset(buf, '\0', count+1);
  for(j = 0; j < count; j++) {
    if(j+1 < packet->dataLen) {
      c = (char)packet->data[i++];
      if(c != '\0' && (c < 32 || c > 126))
        c = ' ';
      buf[j] = c;
    }
    else
      break;
  }
  *idx = i;
}


