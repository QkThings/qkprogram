/*
 * qk_frag.h
 *
 *  Created on: Mar 11, 2014
 *      Author: mribeiro
 */

#ifndef QK_FRAGMENT_H_
#define QK_FRAGMENT_H_

typedef struct qk_packet_frag
{
  qk_packet *packet;
  qk_protocol *protocol;
  uint16_t i_data;
} qk_fragment;

void fragment_fillValue(uint64_t value, uint8_t byteCount, qk_fragment *frag);
void fragment_fillArray(const uint8_t *data, uint16_t size, qk_fragment *frag);
void fragment_fillString(const char* str, qk_fragment *frag);

void fragment_init(qk_fragment *frag, qk_packet *packet, qk_protocol *protocol);
bool fragment_send(qk_fragment *frag, uint8_t *data, uint16_t size);

#endif /* QK_FRAGMENT_H_ */
