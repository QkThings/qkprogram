/*!
 * \file qk_comm.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"

qk_comm_t _qk_comm[QK_COMM_STRUCT_COUNT];

/******************************************************************************/
typedef struct qk_packet_frag_t
{
  qk_packet_t *packet;
  qk_comm_t *comm;
  uint16_t i_data;
} qk_packet_frag_t;
/******************************************************************************/
#define SIZE_FLAGS_CTRL     2
#define SIZE_FLAGS_NETWORK  1
#define SIZE_ID             1
#define SIZE_CODE           1
#define SIZE_ADDR16         2
#define SIZE_ADDR64         8
/******************************************************************************/
#define CODE_OFFSET(packet)   (packet->hdrLen - SIZE_CODE)
#define ADDR16_OFFSET(packet) (CODE_OFFSET(packet) - SIZE_ADDR16)
#define ADDR64_OFFSET(packet) (CODE_OFFSET(packet) - SIZE_ADDR64)
/******************************************************************************/
#define packet_fill8bit(value,frag)  packet_fillValue((uint64_t)(value),1,frag)
#define packet_fill16bit(value,frag) packet_fillValue((uint64_t)(value),2,frag)
#define packet_fill32bit(value,frag) packet_fillValue((uint64_t)(value),4,frag)
#define packet_fill64bit(value,frag) packet_fillValue((uint64_t)(value),8,frag)
/******************************************************************************/
static uint8_t getSequenceArgumentCode(uint8_t packetCode);
static bool board_processPacket(qk_packet_t *packet, qk_comm_t *comm);
static bool device_processPacket(qk_packet_t *packet, qk_comm_t *comm);
/******************************************************************************/
static void packet_fillValue(uint64_t value, uint8_t byteCount, qk_packet_frag_t *frag);
static void packet_fillArray(const uint8_t *data, uint16_t size, qk_packet_frag_t *frag);
static void packet_fillString(const char* str, qk_packet_frag_t *frag);
static void packet_calcHeaderLength(qk_packet_t *packet);
static qk_board_type_t packet_getSrc(qk_packet_t *packet);
static qk_board_type_t packet_getDest(qk_packet_t *packet);
static uint64_t packet_getValue(uint8_t byteCount, uint16_t *idx, qk_packet_t *packet);
static void packet_getString(char *buf, uint16_t count, uint16_t *idx, qk_packet_t *packet);
static void frag_init(qk_packet_frag_t *frag, qk_packet_t *packet, qk_comm_t *comm);
static bool frag_send(qk_packet_frag_t *frag, uint8_t *data, uint16_t size);
/******************************************************************************/

#define send_FLAG(s)  sendCtrlByte(QK_COMM_CTRL_FLAG,s)
#define send_DLE(s)   sendCtrlByte(QK_COMM_CTRL_DLE,s)

void qk_comm_init(qk_comm_t *comm)
{
  memset((void*)comm, 0, sizeof(qk_comm_t));
}

void qk_comm_buildPacket(qk_packet_t *packet, qk_packet_descriptor_t *desc, qk_comm_t *comm)
{
  uint32_t i,j;
  qk_packet_frag_t frag;
  qk_config_t *p_config;

#if defined( QK_IS_DEVICE )
  qk_data_t *p_data;
  qk_event_t *p_event;
  qk_action_t *p_action;
#endif

  frag_init(&frag, packet, comm);

  packet->flags.ctrl = (QK_BOARD_TYPE << 4) & QK_PACKET_FLAGMASK_CTRL_SRC;
  packet->address = _qk_board->info.address;
  if(packet->address != 0)
  {
    packet->flags.ctrl |= (QK_PACKET_FLAGMASK_CTRL_ADDRESS);
  }
  packet->code = desc->code;

  switch(packet->code)
  {
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
    packet_fillValue(_qk.info.baudRate, 4, &frag);
    packet_fillValue(_qk.flags.reg, 4, &frag);
    break;
  case QK_PACKET_CODE_INFOBOARD:
    packet_fillValue(_qk_board->info.version, 2, &frag);
    packet_fillArray((uint8_t*)_qk_board->info.name, QK_BOARD_NAME_SIZE, &frag);
    break;
  case QK_PACKET_CODE_INFOCONFIG:
    p_config = _qk_board->buffers.config;
    packet_fillValue(_qk_board->info._ncfg, 1, &frag);
    for(i=0; i < _qk_board->info._ncfg; i++)
    {
      packet_fillValue(p_config[i].type, 1, &frag);
      packet_fillArray((uint8_t*)p_config[i].proprieties.label, QK_LABEL_SIZE, &frag);
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
    packet_fillValue(_qk.sampling.frequency, 4, &frag);
    packet_fillValue(_qk.sampling.mode, 1, &frag);
    packet_fillValue(_qk.sampling.triggerClock, 1, &frag);
    packet_fillValue(_qk.sampling.triggerScaler, 1, &frag);
    packet_fillValue(_qk.sampling.N, 4, &frag);
    break;
  case QK_PACKET_CODE_INFODATA:
    p_data = _qk_device->buffers.data;
    packet_fillValue(_qk_device->info._ndat, 1, &frag);
    packet_fillValue(_qk_device->info.dataType, 1, &frag);
    for(i=0; i<_qk_device->info._ndat; i++)
    {
      packet_fillArray((uint8_t*)p_data[i].proprieties.label, QK_LABEL_SIZE, &frag);
    }
    break;
  case QK_PACKET_CODE_DATA:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    p_data = _qk_device->buffers.data;
    packet_fillValue(_qk_device->info._ndat, 1, &frag);
    packet_fillValue(_qk_device->info.dataType, 1, &frag);
    for(i=0; i<_qk_device->info._ndat; i++)
    {
      packet_fillValue(p_data[i].value.i, 4, &frag);
    }
    break;
  case QK_PACKET_CODE_INFOEVENT:
    p_event = _qk_device->buffers.event;
    packet_fillValue(_qk_device->info._nevt, 1, &frag);
    for(i=0; i<_qk_device->info._nevt; i++)
    {
      packet_fillArray((uint8_t*)p_event[i].proprieties.label, QK_LABEL_SIZE, &frag);
    }
    break;
  case QK_PACKET_CODE_EVENT:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    p_event = desc->event_fired;
    packet_fillValue(p_event->_id, 1, &frag);
    packet_fillValue(p_event->value.argc, 1, &frag);
    for(i=0; i<p_event->value.argc; i++)
    {
      packet_fillValue(_bytesFromFloat(p_event->value.argv[i]), 4, &frag);
    }
    packet_fillString(p_event->value.text, &frag);
    break;
  case QK_PACKET_CODE_INFOACTION:
    p_action = _qk_device->buffers.action;
    packet_fillValue(_qk_device->info._nact, 1, &frag);
    for(i=0; i<_qk_device->info._nact; i++)
    {
      packet_fillValue(p_action[i].type, 1, &frag);
      packet_fillArray((uint8_t*)p_action[i].proprieties.label, QK_LABEL_SIZE, &frag);
      switch(p_action[i].type)
      {
      case QK_ACTION_TYPE_INT:
        packet_fillValue(p_action[i].proprieties.min, 4, &frag);
        packet_fillValue(p_action[i].proprieties.max, 4, &frag);
        break;
      case QK_ACTION_TYPE_TEXT:
        packet_fillValue(p_action[i].proprieties.max, 4, &frag);
        break;
      }
    }
    break;
  case QK_PACKET_CODE_STRING:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    packet_fillString(desc->string_ptr, &frag);
    break;
#endif
  default: ;
  }
}

static void sendRawByte(uint8_t b, qk_comm_t *comm)
{
  if(comm->callbacks.sendBytes != 0) {
    comm->callbacks.sendBytes(&b, 1);
  }
}

static void sendDataByte(uint8_t b, qk_comm_t *comm)
{
  if(b == QK_COMM_CTRL_FLAG || b == QK_COMM_CTRL_DLE) {
    sendRawByte(QK_COMM_CTRL_DLE, comm);
  }

  sendRawByte(b, comm);
  comm->packet.checksum += b;
}

static void sendDataByteArray(uint8_t *a, uint8_t size, qk_comm_t *comm)
{
  while(size-- > 0)
    sendDataByte(*a++, comm);
}

static void sendCtrlByte(uint8_t b, qk_comm_t *comm)
{
  if(b == QK_COMM_CTRL_FLAG) {
    if(flag(comm->flags.reg, QK_COMM_FLAGMASK_TX)) {
      sendDataByte(comm->packet.checksum, comm);
      comm->packet.checksum = 0;
      comm->flags.reg &= ~QK_COMM_FLAGMASK_TX;
    }
    else
      comm->flags.reg |= QK_COMM_FLAGMASK_TX;
  }
  sendRawByte(b, comm);
}


void qk_comm_sendPacket(qk_packet_t *packet, qk_comm_t *comm)
{
  packet->checksum = packet->flags.ctrl + packet->code;

  if((comm->flags.reg & QK_COMM_FLAGMASK_REXMIT) == 0)
  {
    //qk_packet_setSource(QK_BOARD_TYPE, packet);
  }
  comm->flags.reg &= ~QK_COMM_FLAGMASK_REXMIT;

  send_FLAG(comm);
  sendDataByte(packet->flags.ctrl & 0xFF, comm);
  sendDataByte(packet->flags.ctrl >> 8, comm);

  /*if(flag(packet->flags.ctrl, QK_PACKET_FLAGMASK_CTRL_ADDRESS))
  {
    //sendDataByte(packet->flags.network >> 8, comm);
  }*/
  sendDataByte(packet->code, comm);
  sendDataByteArray((uint8_t*)packet->data, packet->dataLen, comm);
  send_FLAG(comm);
}

void _qk_comm_sendSequenceBegin(uint8_t ref, qk_comm_t *comm)
{
  qk_packet_t packet;
  qk_packet_descriptor_t desc;

  desc.code = QK_PACKET_CODE_SEQBEGIN;
  desc.seq_refCode = ref;
  qk_comm_buildPacket(&packet, &desc, comm);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  comm->callbacks.sendPacket(&packet);
}

void _qk_comm_sendSequenceEnd(uint8_t ref, qk_comm_t *comm)
{
  qk_packet_t packet;
  qk_packet_descriptor_t desc;

  desc.code = QK_PACKET_CODE_SEQEND;
  desc.seq_refCode = ref;
  qk_comm_buildPacket(&packet, &desc, comm);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  comm->callbacks.sendPacket(&packet);
}

void _qk_comm_sendCode(int code, qk_comm_t *comm)
{
  qk_packet_t packet;
  qk_packet_descriptor_t desc;
  desc.code = code;
  qk_comm_buildPacket(&packet, &desc, comm);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  comm->callbacks.sendPacket(&packet);
}

void _qk_comm_sendString(const char *str, qk_comm_t *comm)
{
  qk_packet_t packet;
  qk_packet_descriptor_t desc;

  desc.code = QK_PACKET_CODE_STRING;
  desc.string_ptr = str;
  qk_comm_buildPacket(&packet, &desc, comm);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  comm->callbacks.sendPacket(&packet);
}

#if defined( QK_IS_DEVICE )
void _qk_comm_sendEvent(qk_event_t *e, qk_comm_t *comm)
{
  qk_packet_t packet;
  qk_packet_descriptor_t desc;
  desc.code = QK_PACKET_CODE_EVENT;
  desc.event_fired = e;
  qk_comm_buildPacket(&packet, &desc, comm);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  comm->callbacks.sendPacket(&packet);
}
#endif

void _qk_comm_sendOK(uint8_t ref, qk_comm_t *comm)
{
  qk_packet_t packet;
  qk_packet_descriptor_t desc;

  desc.code = QK_PACKET_CODE_OK;
  desc.ok_refCode = ref;
  qk_comm_buildPacket(&packet, &desc, comm);

  comm->callbacks.sendPacket(&packet);
}

void _qk_comm_sendError(qk_error_t err, uint32_t arg, qk_comm_t *comm)
{
  qk_packet_t packet;
  qk_packet_descriptor_t desc;

  desc.code = QK_PACKET_CODE_ERR;
  desc.err_code = (uint8_t)err;
  desc.err_arg0 = arg;
  qk_comm_buildPacket(&packet, &desc, comm);

  comm->callbacks.sendPacket(&packet);
}

void qk_comm_processByte(uint8_t b, qk_comm_t *comm)
{
  qk_packet_t *packet = &(comm->packet);

  switch(b)
  {
  case QK_COMM_CTRL_FLAG:
    if(flag(comm->flags.reg, QK_COMM_FLAGMASK_DLE) == 0)
    {
      if(flag(comm->flags.reg, QK_COMM_FLAGMASK_RX) == 0)
      {
        packet->address = 0;
        packet->flags.ctrl = 0;
        packet->code = 0;
        comm->ctrl.count = 0;
        comm->flags.reg |= (QK_COMM_FLAGMASK_RX | QK_COMM_FLAGMASK_VALID);
      }
      else
      {
        if(comm->ctrl.count && flag(comm->flags.reg, QK_COMM_FLAGMASK_VALID) == 1)
        {
          packet->dataLen = comm->ctrl.count - packet->hdrLen - 1;
          comm->flags.reg |= QK_COMM_FLAGMASK_NEWPACKET;
          comm->flags.reg &= ~(QK_COMM_FLAGMASK_RX | QK_COMM_FLAGMASK_VALID);
        }
      }
      return;
    }
    break;
  case QK_COMM_CTRL_DLE:
    if(flag(comm->flags.reg, QK_COMM_FLAGMASK_VALID) == 1)
    {
      if(flag(comm->flags.reg, QK_COMM_FLAGMASK_DLE) == 0)
      {
        comm->flags.reg |= QK_COMM_FLAGMASK_DLE;
        return;
      }
    }
    break;
  }

  if(flag(comm->flags.reg, QK_COMM_FLAGMASK_VALID))
  {
    if(comm->ctrl.count == 0)
    {
      packet->flags.ctrl = b;
      packet_calcHeaderLength(packet);
    }
    else if(comm->ctrl.count == 1)
    {
      packet->flags.ctrl |= (b << 8);
    }
    /*else if(flag(packet->flags.ctrl, QK_PACKET_FLAGMASK_CTRL_ADDRESS) == 1 &&
            comm->ctrl.count >= ADDR16_OFFSET(packet) &&
            comm->ctrl.count < CODE_OFFSET(packet))
    {
      if(flag(packet->flags, QK_PACKET_FLAGMASK_16BITADDR) == 1)
      {
        packet->address |= b << (comm->ctrl.count - ADDR16_OFFSET(packet));
      }
      else
      {
        //TODO 64bit addresses not supported yet
      }
    }*/
    else if(comm->ctrl.count == CODE_OFFSET(packet))
    {
      packet->code = b;
    }
    else if(comm->ctrl.count <= QK_PACKET_DATBUF_SIZE)
    {
      packet->data[comm->ctrl.count - packet->hdrLen] = b;
    }
    else
    {
      comm->flags.reg &= ~QK_COMM_FLAGMASK_VALID;
    }

    comm->ctrl.count++;
  }

  comm->flags.reg &= ~QK_COMM_FLAGMASK_DLE;
}

void qk_comm_processPacket(qk_comm_t *comm)
{
  qk_packet_t *packet = &(comm->packet);
  uint16_t i_data;
  bool handled = false;

#ifdef QK_IS_DEVICE
  handled = device_processPacket(packet, comm);
#endif

  if(handled)
    return;

  handled = board_processPacket(packet, comm);
  if(handled)
    return;

  i_data = 0;
  handled = true;
  switch(packet->code)
  {
  case QK_PACKET_CODE_SEQBEGIN:
    comm->flags.reg |= QK_COMM_FLAGMASK_SEQ;
    break;
  case QK_PACKET_CODE_SEQEND:
    comm->flags.reg &= ~QK_COMM_FLAGMASK_SEQ;
    comm->ctrl.ack.code = QK_PACKET_CODE_SEQEND;
    comm->ctrl.ack.arg = packet_getValue(1, &i_data, packet);
    break;
  case QK_PACKET_CODE_SAVE:
#ifdef _QK_FEAT_EEPROM_
    qk_save();
#else
    _qk_comm_sendError(QK_ERR_UNSUPPORTED_OPERATION, packet->code, comm);
#endif
    break;
  case QK_PACKET_CODE_OK:
  case QK_PACKET_CODE_TIMEOUT:
    comm->ctrl.ack.code = packet->code;
    comm->ctrl.ack.arg = packet_getValue(1, &i_data, packet);
    break;
  case QK_PACKET_CODE_ERR:
    comm->ctrl.ack.code = packet->code;
    comm->ctrl.ack.err = packet_getValue(1, &i_data, packet);
    comm->ctrl.ack.arg = packet_getValue(4, &i_data, packet);
    break;
  default:
    handled = false;
  }

  if(!handled)
  {
    comm->ctrl.ack.code = QK_PACKET_CODE_ERR;
    comm->ctrl.ack.err = QK_ERR_CODE_UNKNOWN;
    comm->ctrl.ack.arg = packet->code;
    _qk_comm_sendError(QK_ERR_CODE_UNKNOWN, (uint32_t)packet->code, comm);
  }
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

static bool board_processPacket(qk_packet_t *packet, qk_comm_t *comm)
{
  uint8_t buf[QK_PACKET_DATBUF_SIZE/2];
  uint8_t count, idx;
  int32_t configValue;
  qk_datetime_t dateTime;
  uint16_t i_data;
  bool ok = true, handled = true;

  i_data = 0;
  switch(packet->code)
  {
  case QK_PACKET_CODE_SETQK:
    break;
  case QK_PACKET_CODE_SETNAME:
    packet_getString((char*)buf, QK_BOARD_NAME_SIZE, &i_data, packet);
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
    {
      _qk_comm_sendError(QK_ERR_INVALID_DATA_OR_ARG, idx, comm);
      ok = false;
    }
    break;
  default:
    handled = false;
  }

  if(handled && ok)
    _qk_comm_sendOK(packet->code, comm);

  return handled;
}

#ifdef QK_IS_DEVICE
static bool device_processPacket(qk_packet_t *packet, qk_comm_t *comm)
{
  uint16_t i_data;
  bool sendOk = true, handled = true;
  uint8_t seqArgCode = 0;

  i_data = 0;
  switch(packet->code)
  {
  case QK_PACKET_CODE_SEARCH:
  case QK_PACKET_CODE_GETNODE:
  case QK_PACKET_CODE_GETDEVICE:
    seqArgCode = getSequenceArgumentCode(packet->code);
    _qk_comm_sendSequenceBegin(seqArgCode, comm);
    _qk_comm_sendCode(QK_PACKET_CODE_INFOQK, comm);
    _qk_comm_sendCode(QK_PACKET_CODE_INFOBOARD, comm);
    _qk_comm_sendCode(QK_PACKET_CODE_INFOCONFIG, comm);
    _qk_comm_sendCode(QK_PACKET_CODE_INFOSAMP, comm);
    _qk_comm_sendCode(QK_PACKET_CODE_INFODATA, comm);
    _qk_comm_sendCode(QK_PACKET_CODE_INFOEVENT, comm);
    _qk_comm_sendCode(QK_PACKET_CODE_INFOACTION, comm);
    _qk_comm_sendSequenceEnd(seqArgCode, comm);
    sendOk = false;
    break;
  case QK_PACKET_CODE_START:
    qk_startSampling();
    break;
  case QK_PACKET_CODE_STOP:
    qk_stopSampling();
    break;
  case QK_PACKET_CODE_SETSAMP:
    qk_setSamplingFrequency(packet_getValue(4, &i_data, packet));
    qk_setSamplingMode((qk_samp_mode_t)packet_getValue(1, &i_data, packet));
    qk_setTriggerClock(packet_getValue(1, &i_data, packet));
    qk_setTriggerScaler(packet_getValue(1, &i_data, packet));
    qk_setNumberOfSamples(packet_getValue(4, &i_data, packet));
    break;
  default:
    handled = false;
  }

  if(handled && sendOk)
    _qk_comm_sendOK(packet->code, comm);

  return handled;
}
#endif

static void packet_calcHeaderLength(qk_packet_t *packet)
{
  packet->hdrLen = SIZE_FLAGS_CTRL + SIZE_CODE;
}

static void frag_init(qk_packet_frag_t *frag, qk_packet_t *packet, qk_comm_t *comm)
{
  packet->hdrLen = SIZE_FLAGS_CTRL + SIZE_CODE;
  packet->flags.ctrl = 0;
  frag->packet = packet;
  frag->comm = comm;
  frag->i_data = 0;
}

static bool frag_send(qk_packet_frag_t *frag, uint8_t *data, uint16_t size)
{
  uint8_t count, j = 0;
  uint8_t *dest_ptr, *src_ptr;
  bool fragmented = false;
  qk_packet_t *packet = frag->packet;

#ifdef _QK_FEAT_FRAGMENTATION_
  int16_t ovf;
  ovf = frag->i_data+size-QK_PACKET_DATBUF_SIZE;
  if(ovf > 0)
    fragmented = true;
  while(ovf > 0) {
    dest_ptr = (uint8_t*)(packet->data + frag->i_data);
    src_ptr = (uint8_t*)(data + j);
    count = size - j - ovf;
    memcpy(dest_ptr, src_ptr, count);
    packet->dataLen = QK_PACKET_DATBUF_SIZE;
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_FRAG;
    frag->comm->callbacks.sendPacket(packet);
    j += count;
    frag->i_data = 0;
    ovf = frag->i_data + size - j - QK_PACKET_DATBUF_SIZE;
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

static void packet_fillValue(uint64_t value, uint8_t byteCount, qk_packet_frag_t *frag)
{
  uint64_t buf = value;
  frag_send(frag, (uint8_t*)&buf, byteCount);
}

static void packet_fillArray(const uint8_t *data, uint16_t size, qk_packet_frag_t *frag)
{
  frag_send(frag, (uint8_t*)data, size);
}

static void packet_fillString(const char* str, qk_packet_frag_t *frag)
{
  uint8_t n = strlen(str);
  uint8_t buf[n+1];
  strcpy((char*)buf, str);
  frag_send(frag, buf, n+1);
}

static qk_board_type_t packet_getSrc(qk_packet_t *packet)
{
  return (packet->flags.ctrl & QK_PACKET_FLAGMASK_CTRL_SRC) >> 4;
}

static qk_board_type_t packet_getDest(qk_packet_t *packet)
{
  return (packet->flags.ctrl & QK_PACKET_FLAGMASK_CTRL_DEST) >> 8;
}


static uint64_t packet_getValue(uint8_t byteCount, uint16_t *idx, qk_packet_t *packet)
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

static void packet_getString(char *buf, uint16_t count, uint16_t *idx, qk_packet_t *packet)
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



