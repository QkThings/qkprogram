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
#include "qk_system.h"

void fragment_init(qk_fragment *frag, qk_packet *packet, qk_protocol *protocol)
{
  packet->flags.ctrl = 0;
  packet_calc_header_length(packet);

  frag->packet = packet;
  frag->protocol = protocol;
  frag->i_data = 0;
}

bool fragment_send(qk_fragment *frag, uint8_t *data, uint16_t size)
{
  uint8_t count, j = 0;
  uint8_t *dest_ptr, *src_ptr;
  bool fragmented = false;
  qk_packet *packet = frag->packet;
  qk_callback_arg cb_arg;

#ifdef _QK_FEAT_FRAGMENTATION
  int16_t ovf;
  ovf = frag->i_data+size-_QK_PACKET_PAYLOAD_SIZE;
  if(ovf > 0)
    fragmented = true;
  while(ovf > 0)
  {
    dest_ptr = (uint8_t*)(packet->payload + frag->i_data);
    src_ptr = (uint8_t*)(data + j);
    count = size - j - ovf;
    memcpy(dest_ptr, src_ptr, count);

    packet->payload_lenght = _QK_PACKET_PAYLOAD_SIZE;
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_FRAG;

    QK_CALLBACK_ARG_SET_APTR(&cb_arg, 0, (void*)(frag->protocol));
    QK_CALLBACK_ARG_SET_APTR(&cb_arg, 1, (void*) packet);
    frag->protocol->callback[QK_PROTOCOL_CALLBACK_SENDPACKET](&cb_arg);

    j += count;
    frag->i_data = 0;
    ovf = frag->i_data + size - j - _QK_PACKET_PAYLOAD_SIZE;
  }
#endif
  dest_ptr = (uint8_t*)(packet->payload + frag->i_data);
  src_ptr = (uint8_t*)(data + j);
  count = size - j;
  memcpy(dest_ptr, src_ptr, count*sizeof(uint8_t));

  frag->i_data += count;
  packet->payload_lenght = frag->i_data;

  return fragmented;
}

void fragment_fill_value(uint64_t value, uint8_t byteCount, qk_fragment *frag)
{
  uint64_t buf = value;
  fragment_send(frag, (uint8_t*)&buf, byteCount);
}

void fragment_fill_array(const uint8_t *data, uint16_t size, qk_fragment *frag)
{
  fragment_send(frag, (uint8_t*)data, size);
}

void fragment_fill_string(const char* str, qk_fragment *frag)
{
  uint8_t n = strlen(str);
  uint8_t buf[n+1];
  strcpy((char*)buf, str);
  fragment_send(frag, buf, n+1);
}

