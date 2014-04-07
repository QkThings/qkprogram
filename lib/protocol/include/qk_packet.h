#ifndef QK_PACKET_H_
#define QK_PACKET_H_

#include "qk_settings.h"

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define QK_PACKET_CODE_WAKEUP           0xF5
#define QK_PACKET_CODE_ACK              0x03

#define QK_PACKET_CODE_OK               0x01
#define QK_PACKET_CODE_ERR              0xFF
#define QK_PACKET_CODE_TIMEOUT          0xFE
#define QK_PACKET_CODE_SEQBEGIN         0xCB    // Sequence begin
#define QK_PACKET_CODE_SEQEND           0xCE    // Sequence end
#define QK_PACKET_CODE_SAVE             0x04
#define QK_PACKET_CODE_RESTORE          0x05
#define QK_PACKET_CODE_SEARCH           0x06
#define QK_PACKET_CODE_START            0x0A
#define QK_PACKET_CODE_STOP             0x0F
#define QK_PACKET_CODE_GETNODE          0x10
#define QK_PACKET_CODE_GETMODULE        0x11
#define QK_PACKET_CODE_GETDEVICE        0x12
#define QK_PACKET_CODE_GETNETWORK       0x13
#define QK_PACKET_CODE_GETGATEWAY       0x14
#define QK_PACKET_CODE_GETQK            0x15
#define QK_PACKET_CODE_GETSAMP          0x16
#define QK_PACKET_CODE_GETSTATUS        0x17
#define QK_PACKET_CODE_GETDATA          0x18
#define QK_PACKET_CODE_GETCALENDAR      0x19
#define QK_PACKET_CODE_GETINFOACTION    0x2A
#define QK_PACKET_CODE_GETINFODATA      0x2D
#define QK_PACKET_CODE_GETINFOCONFIG    0x2C
#define QK_PACKET_CODE_GETINFOEVENT     0x2E
#define QK_PACKET_CODE_DEVICEFOUND      0x1A
#define QK_PACKET_CODE_MODULEFOUND      0x1B
#define QK_PACKET_CODE_NETWORKFOUND     0x1C
#define QK_PACKET_CODE_GATEWAYFOUND     0x1D
#define QK_PACKET_CODE_SETQK            0x33
#define QK_PACKET_CODE_SETNAME          0x34
#define QK_PACKET_CODE_SETSAMP          0x36
#define QK_PACKET_CODE_SETCALENDAR      0x39
#define QK_PACKET_CODE_SETCONFIG        0x3C
#define QK_PACKET_CODE_ACTUATE          0x3A
#define QK_PACKET_CODE_SETBAUD          0x40
#define QK_PACKET_CODE_SETFREQ          0x41
#define QK_PACKET_CODE_INFOQK           0xB1
#define QK_PACKET_CODE_INFOSAMP         0xB2
#define QK_PACKET_CODE_INFOBOARD        0xB5
#define QK_PACKET_CODE_INFOMODULE       0xB6
#define QK_PACKET_CODE_INFODEVICE       0xB7
#define QK_PACKET_CODE_INFONETWORK      0xB8
#define QK_PACKET_CODE_INFOGATEWAY      0xB9
#define QK_PACKET_CODE_INFOACTION       0xBA
#define QK_PACKET_CODE_INFODATA         0xBD
#define QK_PACKET_CODE_INFOEVENT        0xBE
#define QK_PACKET_CODE_INFOCONFIG       0xBC
#define QK_PACKET_CODE_CALENDAR         0xD1
#define QK_PACKET_CODE_STATUS           0xD5
#define QK_PACKET_CODE_DATA             0xD0
#define QK_PACKET_CODE_EVENT            0xDE
#define QK_PACKET_CODE_ACTION           0xDA
#define QK_PACKET_CODE_STRING           0xDF

#define QK_PACKET_FLAGMASK_CTRL_SRC        0x0070
#define QK_PACKET_FLAGMASK_CTRL_NOTIF      0x0008
#define QK_PACKET_FLAGMASK_CTRL_FRAG       0x0004
#define QK_PACKET_FLAGMASK_CTRL_LASTFRAG   0x0002
#define QK_PACKET_FLAGMASK_CTRL_ADDRESS    0x0001
#define QK_PACKET_FLAGMASK_CTRL_DEST       0x0700

//   rsvd[1] | SRC[3] | NOTIF[1] | FRAG[1] | LASTFRAG[1] | ADDRESS[1]
//   rsvd[5] | DEST[3]

#define SIZE_FLAGS_CTRL     2
#define SIZE_ID             1
#define SIZE_CODE           1
#define SIZE_ADDR16         2
#define SIZE_ADDR64         8

/******************************************************************************
   STRUCTS
 ******************************************************************************/

typedef volatile struct qk_packet_flags
{
  uint16_t ctrl;
  uint8_t network;
} qk_packet_flags;

typedef volatile struct qk_packet
{
  qk_packet_flags flags;
  uint64_t address;
  uint8_t  id;
  uint8_t  code;
  uint8_t  data[_QK_PACKET_DATBUF_SIZE];
  uint8_t  checksum;
  uint8_t  hdrLen;
  uint8_t  dataLen;
} qk_packet;

typedef volatile struct qk_packet_descriptor
{
  uint64_t address;
  uint8_t  code;
  uint8_t  boardType;
  qk_event *event_fired;
  union {
    struct {
      uint8_t seq_refCode;
    };
    struct {
      uint8_t ok_refCode;
    };
    struct {
      uint8_t err_code;
      uint32_t err_arg0;
      uint32_t err_arg1;
    };
    struct {
      const char *string_ptr;
    };
  };
} qk_packet_descriptor;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

//#define packet_fill8bit(value,frag)  packet_fillValue((uint64_t)(value),1,frag)
//#define packet_fill16bit(value,frag) packet_fillValue((uint64_t)(value),2,frag)
//#define packet_fill32bit(value,frag) packet_fillValue((uint64_t)(value),4,frag)
//#define packet_fill64bit(value,frag) packet_fillValue((uint64_t)(value),8,frag)

void packet_calc_header_length(qk_packet *packet);
uint64_t packet_get_value(uint8_t byteCount, uint16_t *idx, qk_packet *packet);
void packet_get_string(char *buf, uint16_t count, uint16_t *idx, qk_packet *packet);

#endif /* QK_PACKET_H_ */
