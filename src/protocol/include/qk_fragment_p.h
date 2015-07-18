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

#ifndef QK_FRAGMENT_H_
#define QK_FRAGMENT_H_

typedef struct qk_packet_frag
{
  qk_packet *packet;
  qk_protocol *protocol;
  uint16_t i_data;
} qk_fragment;

void fragment_fill_value(uint64_t value, uint8_t byteCount, qk_fragment *frag);
void fragment_fill_array(const uint8_t *data, uint16_t size, qk_fragment *frag);
void fragment_fill_string(const char* str, qk_fragment *frag);

void fragment_init(qk_fragment *frag, qk_packet *packet, qk_protocol *protocol);
bool fragment_send(qk_fragment *frag, uint8_t *data, uint16_t size);

#endif /* QK_FRAGMENT_H_ */
