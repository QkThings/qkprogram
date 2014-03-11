/*!
 * \file qk_comm.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"

qk_protocol _qk_protocol[QK_PROTOCOL_STRUCT_COUNT];

/******************************************************************************/
typedef struct qk_packet_frag
{
  qk_packet *packet;
  qk_protocol *protocol;
  uint16_t i_data;
} qk_packet_frag;
/******************************************************************************/
#define SIZE_FLAGS_CTRL     2
#define SIZE_ID             1
#define SIZE_CODE           1
#define SIZE_ADDR16         2
#define SIZE_ADDR64         8
/******************************************************************************/
#define CODE_OFFSET(packet)   (packet->hdrLen - SIZE_CODE)
#define ID_OFFSET(packet)     (CODE_OFFSET(packet) - SIZE_ID)
#define ADDR16_OFFSET(packet) (ID_OFFSET(packet) - SIZE_ADDR16)
#define ADDR64_OFFSET(packet) (ID_OFFSET(packet) - SIZE_ADDR64)
/******************************************************************************/
#define packet_fill8bit(value,frag)  packet_fillValue((uint64_t)(value),1,frag)
#define packet_fill16bit(value,frag) packet_fillValue((uint64_t)(value),2,frag)
#define packet_fill32bit(value,frag) packet_fillValue((uint64_t)(value),4,frag)
#define packet_fill64bit(value,frag) packet_fillValue((uint64_t)(value),8,frag)
/******************************************************************************/
static uint8_t getSequenceArgumentCode(uint8_t packetCode);
static bool board_processPacket(qk_packet *packet, qk_protocol *protocol);
static bool device_processPacket(qk_packet *packet, qk_protocol *protocol);
/******************************************************************************/
static void packet_fillValue(uint64_t value, uint8_t byteCount, qk_packet_frag *frag);
static void packet_fillArray(const uint8_t *data, uint16_t size, qk_packet_frag *frag);
static void packet_fillString(const char* str, qk_packet_frag *frag);
static void packet_calcHeaderLength(qk_packet *packet);
static qk_board_type packet_getSrc(qk_packet *packet);
static qk_board_type packet_getDest(qk_packet *packet);
static uint64_t packet_getValue(uint8_t byteCount, uint16_t *idx, qk_packet *packet);
static void packet_getString(char *buf, uint16_t count, uint16_t *idx, qk_packet *packet);
static void frag_init(qk_packet_frag *frag, qk_packet *packet, qk_protocol *protocol);
static bool frag_send(qk_packet_frag *frag, uint8_t *data, uint16_t size);
/******************************************************************************/

#define send_FLAG(s)  sendCtrlByte(QK_COMM_CTRL_FLAG,s)
#define send_DLE(s)   sendCtrlByte(QK_COMM_CTRL_DLE,s)


inline void qk_ack_set_OK(qk_ack *ack)
{
  ack->type = QK_ACK_TYPE_OK;
}

void qk_ack_set_ERROR(qk_err err, int32_t arg, qk_ack *ack)
{
  ack->type = QK_ACK_TYPE_ERROR;
  ack->err = err;
  ack->arg = arg;
}


void qk_protocol_init(qk_protocol *protocol)
{
  memset((void*)protocol, 0, sizeof(qk_protocol));
}

void qk_protocol_buildPacket(qk_packet *packet, qk_packet_descriptor *desc, qk_protocol *protocol)
{
  uint32_t i,j;
  qk_packet_frag frag;
  qk_config *p_config;

#if defined( QK_IS_DEVICE )
  qk_data *p_data;
  qk_event *p_event;
  qk_action *p_action;
#endif

  frag_init(&frag, packet, protocol);

  packet->flags.ctrl = (QK_BOARD_TYPE << 4) & QK_PACKET_FLAGMASK_CTRL_SRC;
  packet->address = _qk_board->info.address;
  if(packet->address != 0)
  {
    packet->flags.ctrl |= (QK_PACKET_FLAGMASK_CTRL_ADDRESS);
  }
  packet->code = desc->code;

  switch(packet->code)
  {
  case QK_PACKET_CODE_ACK:
    packet_fillValue(protocol->packet.id, 1, &frag);
    packet_fillValue(protocol->packet.code, 1, &frag);
    packet_fillValue(protocol->ctrl.ack.type, 1, &frag);
    if(protocol->ctrl.ack.type == QK_ACK_TYPE_ERROR)
    {
      packet_fillValue(protocol->ctrl.ack.err, 1, &frag);
      packet_fillValue(protocol->ctrl.ack.arg, 1, &frag);
    }
    break;
  case QK_PACKET_CODE_SEQBEGIN:
  case QK_PACKET_CODE_SEQEND:
    packet_fillValue(desc->seq_refCode, 1, &frag);
    break;
  case QK_PACKET_CODE_OK:
    packet_fillValue(desc->ok_refCode, 1, &frag);
    break;
  case QK_PACKET_CODE_ERR:
    packet_fillValue(desc->err_code, 1, &frag);
    packet_fillValue(desc->err_arg0, 4, &frag);
    break;
  case QK_PACKET_CODE_INFOQK:
    packet_fillValue(QK_VERSION_MAJOR, 1, &frag);
    packet_fillValue(QK_VERSION_MINOR, 1, &frag);
    packet_fillValue(QK_VERSION_PATCH, 1, &frag);
    packet_fillValue(_qk_core.info.baudRate, 4, &frag);
    packet_fillValue(_qk_core.flags.reg, 4, &frag);
    break;
  case QK_PACKET_CODE_INFOBOARD:
    packet_fillValue(_qk_board->info.version, 2, &frag);
    packet_fillArray((uint8_t*)_qk_board->info.name, _QK_BOARD_NAME_SIZE, &frag);
    break;
  case QK_PACKET_CODE_INFOCONFIG:
    p_config = _qk_board->buffers.config;
    packet_fillValue(_qk_board->info._ncfg, 1, &frag);
    for(i=0; i < _qk_board->info._ncfg; i++)
    {
      packet_fillValue(p_config[i].type, 1, &frag);
      packet_fillArray((uint8_t*)p_config[i].proprieties.label, _QK_LABEL_SIZE, &frag);
      switch(_qk_board->buffers.config[i].type)
      {
      case QK_CONFIG_TYPE_BOOL:
        packet_fillValue(p_config[i].value.b, 1, &frag);
        break;
      case QK_CONFIG_TYPE_INTDEC:
      case QK_CONFIG_TYPE_INTHEX:
      case QK_CONFIG_TYPE_FLOAT:
        packet_fillValue(p_config[i].value.i, 4, &frag);
        packet_fillValue(p_config[i].proprieties.min, 4, &frag);
        packet_fillValue(p_config[i].proprieties.max, 4, &frag);
        break;
      case QK_CONFIG_TYPE_DATETIME:
        packet_fillValue(p_config[i].value.dateTime.year, 1, &frag);
        packet_fillValue(p_config[i].value.dateTime.month, 1, &frag);
        packet_fillValue(p_config[i].value.dateTime.day, 1, &frag);
      case QK_CONFIG_TYPE_TIME:
        packet_fillValue(p_config[i].value.dateTime.hours, 1, &frag);
        packet_fillValue(p_config[i].value.dateTime.minutes, 1, &frag);
        packet_fillValue(p_config[i].value.dateTime.seconds, 1, &frag);
        break;
      case QK_CONFIG_TYPE_COMBO:
        packet_fillValue(p_config[i].proprieties.size, 1, &frag);
        for(j=0; j<p_config[i].proprieties.size; j++)
        {
          packet_fillString(p_config[i].value.items[j], &frag);
        }
        break;
      }
    }
    break;
#if defined( QK_IS_DEVICE )
  case QK_PACKET_CODE_INFOSAMP:
    packet_fillValue(_qk_core.sampling.frequency, 4, &frag);
    packet_fillValue(_qk_core.sampling.mode, 1, &frag);
    packet_fillValue(_qk_core.sampling.triggerClock, 1, &frag);
    packet_fillValue(_qk_core.sampling.triggerScaler, 1, &frag);
    packet_fillValue(_qk_core.sampling.N, 4, &frag);
    break;
  case QK_PACKET_CODE_INFODATA:
    p_data = _qk_device->buffers.data;
    packet_fillValue(_qk_device->info._ndat, 1, &frag);
    packet_fillValue(_qk_device->info.dataType, 1, &frag);
    for(i=0; i<_qk_device->info._ndat; i++)
      packet_fillArray((uint8_t*)p_data[i].properties.label, _QK_LABEL_SIZE, &frag);
    break;
  case QK_PACKET_CODE_DATA:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    p_data = _qk_device->buffers.data;
    packet_fillValue(_qk_device->info._ndat, 1, &frag);
    packet_fillValue(_qk_device->info.dataType, 1, &frag);
    for(i=0; i<_qk_device->info._ndat; i++)
      packet_fillValue(p_data[i].value.i, 4, &frag);
    break;
  case QK_PACKET_CODE_INFOEVENT:
    p_event = _qk_device->buffers.event;
    packet_fillValue(_qk_device->info._nevt, 1, &frag);
    for(i=0; i<_qk_device->info._nevt; i++)
      packet_fillArray((uint8_t*)p_event[i].properties.label, _QK_LABEL_SIZE, &frag);
    break;
  case QK_PACKET_CODE_EVENT:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    p_event = desc->event_fired;
    packet_fillValue(p_event->_id, 1, &frag);
    packet_fillValue(p_event->value.argc, 1, &frag);
    for(i=0; i<p_event->value.argc; i++)
      packet_fillValue(_bytesFromFloat(p_event->value.argv[i]), 4, &frag);
    packet_fillString(p_event->value.text, &frag);
    break;
  case QK_PACKET_CODE_INFOACTION:
    p_action = _qk_device->buffers.action;
    packet_fillValue(_qk_device->info._nact, 1, &frag);
    for(i=0; i<_qk_device->info._nact; i++)
    {
      packet_fillValue(p_action[i].type, 1, &frag);
      packet_fillArray((uint8_t*)p_action[i].properties.label, _QK_LABEL_SIZE, &frag);
      switch(p_action[i].type)
      {
      case QK_ACTION_TYPE_INT:
        packet_fillValue(p_action[i].properties.min, 4, &frag);
        packet_fillValue(p_action[i].properties.max, 4, &frag);
        packet_fillValue(p_action[i].value.i, 4, &frag);
        break;
      case QK_ACTION_TYPE_BOOL:
        packet_fillValue(p_action[i].value.b, 1, &frag);
        break;
      case QK_ACTION_TYPE_TEXT:
        //TODO qk action text
        break;
      }
    }
    break;
#endif
  case QK_PACKET_CODE_STRING:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    packet_fillString(desc->string_ptr, &frag);
    break;
  default: ;
  }
}

static void sendRawByte(uint8_t b, qk_protocol *protocol)
{
  if(protocol->callback.sendBytes != 0) {
    protocol->callback.sendBytes(&b, 1);
  }
}

static void sendDataByte(uint8_t b, qk_protocol *protocol)
{
  if(b == QK_COMM_CTRL_FLAG || b == QK_COMM_CTRL_DLE) {
    sendRawByte(QK_COMM_CTRL_DLE, protocol);
  }

  sendRawByte(b, protocol);
  protocol->packet.checksum += b;
}

static void sendDataByteArray(uint8_t *a, uint8_t size, qk_protocol *protocol)
{
  while(size-- > 0)
    sendDataByte(*a++, protocol);
}

static void sendCtrlByte(uint8_t b, qk_protocol *protocol)
{
  if(b == QK_COMM_CTRL_FLAG)
  {
    if(flag(protocol->flags.reg, QK_COMM_FLAGMASK_TX))
    {
      sendDataByte(protocol->packet.checksum, protocol);
      protocol->packet.checksum = 0;
      protocol->flags.reg &= ~QK_COMM_FLAGMASK_TX;
    }
    else
      protocol->flags.reg |= QK_COMM_FLAGMASK_TX;
  }
  sendRawByte(b, protocol);
}


void qk_protocol_sendPacket(qk_packet *packet, qk_protocol *protocol)
{
  packet->checksum = packet->flags.ctrl + packet->code;

  if((protocol->flags.reg & QK_COMM_FLAGMASK_REXMIT) == 0)
  {
    //qk_packet_setSource(QK_BOARD_TYPE, packet);
  }
  protocol->flags.reg &= ~QK_COMM_FLAGMASK_REXMIT;

  send_FLAG(protocol);
  sendDataByte(packet->flags.ctrl & 0xFF, protocol);
  sendDataByte(packet->flags.ctrl >> 8, protocol);

  /*if(flag(packet->flags.ctrl, QK_PACKET_FLAGMASK_CTRL_ADDRESS))
  {
    //sendDataByte(packet->flags.network >> 8, comm);
  }*/
  sendDataByte(packet->code, protocol);
  sendDataByteArray((uint8_t*)packet->data, packet->dataLen, protocol);
  send_FLAG(protocol);
}

void _qk_comm_sendSequenceBegin(uint8_t ref, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;

  desc.code = QK_PACKET_CODE_SEQBEGIN;
  desc.seq_refCode = ref;
  qk_protocol_buildPacket(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}

void _qk_comm_sendSequenceEnd(uint8_t ref, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;

  desc.code = QK_PACKET_CODE_SEQEND;
  desc.seq_refCode = ref;
  qk_protocol_buildPacket(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}

void _qk_protocol_sendCode(int code, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;
  desc.code = code;
  qk_protocol_buildPacket(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}

void _qk_protocol_sendString(const char *str, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;

  desc.code = QK_PACKET_CODE_STRING;
  desc.string_ptr = str;
  qk_protocol_buildPacket(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}

#if defined( QK_IS_DEVICE )
void _qk_comm_sendEvent(qk_event *e, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;
  desc.code = QK_PACKET_CODE_EVENT;
  desc.event_fired = e;
  qk_protocol_buildPacket(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}
#endif

void qk_protocol_processByte(uint8_t b, qk_protocol *protocol)
{
  qk_packet *pkt = &(protocol->packet);

  switch(b)
  {
  case QK_COMM_CTRL_FLAG:
    if(flag(protocol->flags.reg, QK_COMM_FLAGMASK_DLE) == 0)
    {
      if(flag(protocol->flags.reg, QK_COMM_FLAGMASK_RX) == 0)
      {
        pkt->address = 0;
        pkt->flags.ctrl = 0;
        pkt->code = 0;
        protocol->ctrl.count = 0;
        protocol->flags.reg |= (QK_COMM_FLAGMASK_RX | QK_COMM_FLAGMASK_VALID);
      }
      else
      {
        if(protocol->ctrl.count && flag(protocol->flags.reg, QK_COMM_FLAGMASK_VALID) == 1)
        {
          pkt->dataLen = protocol->ctrl.count - pkt->hdrLen - 1;
          protocol->flags.reg |= QK_COMM_FLAGMASK_NEWPACKET;
          protocol->flags.reg &= ~(QK_COMM_FLAGMASK_RX | QK_COMM_FLAGMASK_VALID);
        }
      }
      return;
    }
    break;
  case QK_COMM_CTRL_DLE:
    if(flag(protocol->flags.reg, QK_COMM_FLAGMASK_VALID) == 1)
    {
      if(flag(protocol->flags.reg, QK_COMM_FLAGMASK_DLE) == 0)
      {
        protocol->flags.reg |= QK_COMM_FLAGMASK_DLE;
        return;
      }
    }
    break;
  }

  if(flag(protocol->flags.reg, QK_COMM_FLAGMASK_VALID))
  {
    if(protocol->ctrl.count == 0)
    {
      pkt->flags.ctrl = b;
      packet_calcHeaderLength(pkt);
    }
    else if(protocol->ctrl.count == 1)
    {
      pkt->flags.ctrl |= (b << 8);
    }
    else if(protocol->ctrl.count == ID_OFFSET(pkt))
    {
      pkt->id = b;
    }
    else if(protocol->ctrl.count == CODE_OFFSET(pkt))
    {
      pkt->code = b;
    }
    else if(protocol->ctrl.count <= _QK_PACKET_DATBUF_SIZE)
    {
      pkt->data[protocol->ctrl.count - pkt->hdrLen] = b;
    }
    else
    {
      protocol->flags.reg &= ~QK_COMM_FLAGMASK_VALID;
    }

    protocol->ctrl.count++;
  }

  protocol->flags.reg &= ~QK_COMM_FLAGMASK_DLE;
}

void qk_protocol_processPacket(qk_protocol *protocol)
{
  qk_packet *packet = &(protocol->packet);
  qk_ack *ack = &(protocol->ctrl.ack);
  uint16_t i_data;
  bool handled = false;

#ifdef QK_IS_DEVICE
  handled = device_processPacket(packet, protocol);
#endif
  if(handled) return;

  handled = board_processPacket(packet, protocol);
  if(handled) return;


  //TODO send ACK (ok/err) using QK_

  i_data = 0;
  handled = true;
  switch(packet->code)
  {
  case QK_PACKET_CODE_SAVE:
#ifdef _QK_FEAT_EEPROM_
    qk_save();
#else
    qk_ack_set_ERROR(QK_ERR_UNSUPPORTED_OPERATION, (int32_t)packet->code, ack);
#endif
    break;
    //    case QK_PACKET_CODE_TIMEOUT:
  case QK_PACKET_CODE_OK:
    protocol->ctrl.ack.type = packet->code;
    protocol->ctrl.ack.arg = packet_getValue(1, &i_data, packet);
    break;
  case QK_PACKET_CODE_ERR:
    protocol->ctrl.ack.type = packet->code;
    protocol->ctrl.ack.err = packet_getValue(1, &i_data, packet);
    protocol->ctrl.ack.arg = packet_getValue(4, &i_data, packet);
    break;
  default:
    handled = false;
  }


  if(!handled)
  {
    protocol->ctrl.ack.type = QK_PACKET_CODE_ERR;
    protocol->ctrl.ack.err = QK_ERR_CODE_UNKNOWN;
    protocol->ctrl.ack.arg = packet->code;
    qk_ack_set_ERROR(QK_ERR_CODE_UNKNOWN, (int32_t)packet->code, ack);
  }

  // For each packet processed, send ACK
  _qk_protocol_sendCode(QK_PACKET_CODE_ACK, protocol);
}

static uint8_t getSequenceArgumentCode(uint8_t packetCode)
{
  uint8_t seqArgCode = 0;
  switch(packetCode)
  {
  case QK_PACKET_CODE_SEARCH:
    seqArgCode = QK_PACKET_CODE_DEVICEFOUND;
    break;
  case QK_PACKET_CODE_GETNODE:
  case QK_PACKET_CODE_GETDEVICE:
    seqArgCode = QK_PACKET_CODE_GETDEVICE;
    break;
  default: ;
  }
  return seqArgCode;
}

static bool board_processPacket(qk_packet *packet, qk_protocol *protocol)
{
  uint8_t buf[_QK_PACKET_DATBUF_SIZE/2];
  uint8_t count, idx;
  int32_t configValue;
  qk_datetime dateTime;
  uint16_t i_data;
  bool handled = true;

  qk_ack *ack = &(protocol->ctrl.ack);
  qk_ack_set_OK(ack);

  i_data = 0;
  switch(packet->code)
  {
  case QK_PACKET_CODE_SETQK:
    break;
  case QK_PACKET_CODE_SETNAME:
    packet_getString((char*)buf, _QK_BOARD_NAME_SIZE, &i_data, packet);
    qk_setBoardName((char*)buf);
    break;
  case QK_PACKET_CODE_SETCONFIG:
    count = packet_getValue(1, &i_data, packet); // must be 1
    idx = packet_getValue(1, &i_data, packet);
    if(idx < qk_configCount())
    {
      switch(qk_configType(idx))
      {
      case QK_CONFIG_TYPE_BOOL:
        configValue = packet_getValue(1, &i_data, packet);
        qk_setConfigValueB(idx, (bool) configValue);
        break;
      case QK_CONFIG_TYPE_INTDEC:
      case QK_CONFIG_TYPE_INTHEX:
        configValue = packet_getValue(4, &i_data, packet);
        qk_setConfigValueI(idx, configValue);
        break;
      case QK_CONFIG_TYPE_FLOAT:
        configValue = packet_getValue(4, &i_data, packet);
        qk_setConfigValueF(idx, _floatFromBytes(configValue));
        break;
      case QK_CONFIG_TYPE_DATETIME:
        dateTime.year = packet_getValue(1, &i_data, packet);
        dateTime.month = packet_getValue(1, &i_data, packet);
        dateTime.day = packet_getValue(1, &i_data, packet);
      case QK_CONFIG_TYPE_TIME:
        dateTime.hours = packet_getValue(1, &i_data, packet);
        dateTime.minutes = packet_getValue(1, &i_data, packet);
        dateTime.seconds = packet_getValue(1, &i_data, packet);
        qk_setConfigValueDT(idx, dateTime);
      case QK_CONFIG_TYPE_COMBO:
        break;
      default: ;
      }
      if(_qk_board->callbacks.config != 0)
        _qk_board->callbacks.config();
    }
    else
      qk_ack_set_ERROR(QK_ERR_INVALID_DATA_OR_ARG, idx, ack);
    break;
  default:
    handled = false;
  }

  if(handled)
  {
    _qk_protocol_sendCode(QK_PACKET_CODE_ACK, protocol);
  }

  return handled;
}

#ifdef QK_IS_DEVICE
static bool device_processPacket(qk_packet *packet, qk_protocol *protocol)
{
  uint16_t i_data;
  bool handled = true;
  bool call_action;

  qk_action *act;
  qk_action_id act_id;

  qk_ack *ack = &(protocol->ctrl.ack);
  qk_ack_set_OK(ack);

  i_data = 0;
  switch(packet->code)
  {
  case QK_PACKET_CODE_SEARCH:
  case QK_PACKET_CODE_GETNODE:
  case QK_PACKET_CODE_GETDEVICE:
    //TODO sending info should be optional (the ACK already signals the presence of the device)
    //TODO seq is obsolete (wait for ACK)
//    seq_arg_code = getSequenceArgumentCode(packet->code);
//    _qk_comm_sendSequenceBegin(seq_arg_code, protocol);
    _qk_protocol_sendCode(QK_PACKET_CODE_INFOQK, protocol);
    _qk_protocol_sendCode(QK_PACKET_CODE_INFOBOARD, protocol);
    _qk_protocol_sendCode(QK_PACKET_CODE_INFOCONFIG, protocol);
    _qk_protocol_sendCode(QK_PACKET_CODE_INFOSAMP, protocol);
    _qk_protocol_sendCode(QK_PACKET_CODE_INFODATA, protocol);
    _qk_protocol_sendCode(QK_PACKET_CODE_INFOEVENT, protocol);
    _qk_protocol_sendCode(QK_PACKET_CODE_INFOACTION, protocol);
//    _qk_comm_sendSequenceEnd(seq_arg_code, protocol);
    break;
  case QK_PACKET_CODE_START:
    qk_startSampling();
    break;
  case QK_PACKET_CODE_STOP:
    qk_stopSampling();
    break;
  case QK_PACKET_CODE_SETSAMP:
    qk_setSamplingFrequency(packet_getValue(4, &i_data, packet));
    qk_setSamplingMode((qk_samp_mode)packet_getValue(1, &i_data, packet));
    qk_setTriggerClock(packet_getValue(1, &i_data, packet));
    qk_setTriggerScaler(packet_getValue(1, &i_data, packet));
    qk_setNumberOfSamples(packet_getValue(4, &i_data, packet));
    break;
  case QK_PACKET_CODE_ACTUATE:
    act =  _qk_device->buffers.action;
    act_id = (qk_action_id) packet_getValue(1, &i_data, packet);
    packet_getValue(1, &i_data, packet); // action type
    act = &(_qk_device->buffers.action[act_id]);
    switch(act->type)
    {
    case QK_ACTION_TYPE_INT:
      act->value.i = packet_getValue(4, &i_data, packet);
      break;
    case QK_ACTION_TYPE_BOOL:
      act->value.b = packet_getValue(1, &i_data, packet);
      break;
    }
    call_action = true;
    break;
  default:
    handled = false;
  }

  if(handled)
  {
    _qk_protocol_sendCode(QK_PACKET_CODE_ACK, protocol);

    if(call_action && _qk_device->callbacks.action != 0)
      _qk_device->callbacks.action(act_id);
  }

  return handled;
}
#endif

static void packet_calcHeaderLength(qk_packet *packet)
{
  packet->hdrLen = SIZE_FLAGS_CTRL + SIZE_CODE + SIZE_ID;
}

static void frag_init(qk_packet_frag *frag, qk_packet *packet, qk_protocol *protocol)
{
  packet->flags.ctrl = 0;
  packet_calcHeaderLength(packet);

  frag->packet = packet;
  frag->protocol = protocol;
  frag->i_data = 0;
}

static bool frag_send(qk_packet_frag *frag, uint8_t *data, uint16_t size)
{
  uint8_t count, j = 0;
  uint8_t *dest_ptr, *src_ptr;
  bool fragmented = false;
  qk_packet *packet = frag->packet;

#ifdef _QK_FEAT_FRAGMENTATION
  int16_t ovf;
  ovf = frag->i_data+size-_QK_PACKET_DATBUF_SIZE;
  if(ovf > 0)
    fragmented = true;
  while(ovf > 0) {
    dest_ptr = (uint8_t*)(packet->data + frag->i_data);
    src_ptr = (uint8_t*)(data + j);
    count = size - j - ovf;
    memcpy(dest_ptr, src_ptr, count);
    packet->dataLen = _QK_PACKET_DATBUF_SIZE;
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_FRAG;
    frag->protocol->callback.sendPacket(packet);
    j += count;
    frag->i_data = 0;
    ovf = frag->i_data + size - j - _QK_PACKET_DATBUF_SIZE;
  }
#endif
  dest_ptr = (uint8_t*)(packet->data + frag->i_data);
  src_ptr = (uint8_t*)(data + j);
  count = size - j;
  memcpy(dest_ptr, src_ptr, count*sizeof(uint8_t));
  frag->i_data += count;
  packet->dataLen = frag->i_data;

  return fragmented;
}

static void packet_fillValue(uint64_t value, uint8_t byteCount, qk_packet_frag *frag)
{
  uint64_t buf = value;
  frag_send(frag, (uint8_t*)&buf, byteCount);
}

static void packet_fillArray(const uint8_t *data, uint16_t size, qk_packet_frag *frag)
{
  frag_send(frag, (uint8_t*)data, size);
}

static void packet_fillString(const char* str, qk_packet_frag *frag)
{
  uint8_t n = strlen(str);
  uint8_t buf[n+1];
  strcpy((char*)buf, str);
  frag_send(frag, buf, n+1);
}

static qk_board_type packet_getSrc(qk_packet *packet)
{
  return (packet->flags.ctrl & QK_PACKET_FLAGMASK_CTRL_SRC) >> 4;
}

static qk_board_type packet_getDest(qk_packet *packet)
{
  return (packet->flags.ctrl & QK_PACKET_FLAGMASK_CTRL_DEST) >> 8;
}


static uint64_t packet_getValue(uint8_t byteCount, uint16_t *idx, qk_packet *packet)
{
  uint16_t j, i = *idx;
  uint64_t value = 0;

  for(j = 0; j < byteCount; j++) {
    value += (packet->data[i++] & 0xFF) << (8*j); // little endian
  }

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
  *idx = i;
  return value;
}

static void packet_getString(char *buf, uint16_t count, uint16_t *idx, qk_packet *packet)
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



