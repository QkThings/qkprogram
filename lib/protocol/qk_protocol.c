/*!
 * \file qk_comm.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"
#include "qk_debug.h"

qk_protocol _qk_protocol[QK_PROTOCOL_STRUCT_COUNT];

/******************************************************************************/
#define CODE_OFFSET(packet)   ((unsigned)(packet->hdrLen - SIZE_CODE))
#define ID_OFFSET(packet)     ((unsigned)(CODE_OFFSET(packet) - SIZE_ID))
#define ADDR16_OFFSET(packet) ((unsigned)(ID_OFFSET(packet) - SIZE_ADDR16))
#define ADDR64_OFFSET(packet) ((unsigned)(ID_OFFSET(packet) - SIZE_ADDR64))
/******************************************************************************/
static bool board_process_packet(qk_packet *packet, qk_protocol *protocol);
static bool device_process_packet(qk_packet *packet, qk_protocol *protocol);
/******************************************************************************/

void qk_ack_set_OK(qk_ack *ack)
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

void qk_protocol_build_packet(qk_packet *packet, qk_packet_descriptor *desc, qk_protocol *protocol)
{
  uint32_t i,j;
  qk_fragment frag;
  qk_config *p_config;

#if defined( QK_IS_DEVICE )
  qk_data *p_data;
  qk_event *p_event;
  qk_action *p_action;
#endif

  fragment_init(&frag, packet, protocol);

  packet->flags.ctrl = (QK_BOARD_TYPE << 4) & QK_PACKET_FLAGMASK_CTRL_SRC;
  packet->address = _qk_board->info.address;
//  if(packet->address != 0)
//  {
//    packet->flags.ctrl |= (QK_PACKET_FLAGMASK_CTRL_ADDRESS);
//  }
  packet->code = desc->code;

  switch(packet->code)
  {
  case QK_PACKET_CODE_ACK:
    fragment_fill_value(protocol->packet.id, 1, &frag);
    fragment_fill_value(protocol->packet.code, 1, &frag);
    fragment_fill_value(protocol->ctrl.ack.type, 1, &frag);
    if(protocol->ctrl.ack.type == QK_ACK_TYPE_ERROR)
    {
      fragment_fill_value(protocol->ctrl.ack.err, 1, &frag);
      fragment_fill_value(protocol->ctrl.ack.arg, 1, &frag);
    }
    break;
  case QK_PACKET_CODE_INFOQK:
    fragment_fill_value(QK_VERSION_MAJOR, 1, &frag);
    fragment_fill_value(QK_VERSION_MINOR, 1, &frag);
    fragment_fill_value(QK_VERSION_PATCH, 1, &frag);
    fragment_fill_value(_qk_core.info.baudRate, 4, &frag);
    fragment_fill_value(_qk_core.flags.reg, 4, &frag);
    break;
  case QK_PACKET_CODE_INFOBOARD:
    fragment_fill_value(_qk_board->info.version, 2, &frag);
    fragment_fill_array((uint8_t*)_qk_board->info.name, _QK_BOARD_NAME_SIZE, &frag);
    break;
  case QK_PACKET_CODE_INFOCONFIG:
    p_config = _qk_board->buffers.config;
    fragment_fill_value(_qk_board->info._ncfg, 1, &frag);
    for(i=0; i < _qk_board->info._ncfg; i++)
    {
      fragment_fill_value(p_config[i].type, 1, &frag);
      fragment_fill_array((uint8_t*)p_config[i].proprieties.label, _QK_LABEL_SIZE, &frag);
      switch(_qk_board->buffers.config[i].type)
      {
      case QK_CONFIG_TYPE_BOOL:
        fragment_fill_value(p_config[i].value.b, 1, &frag);
        break;
      case QK_CONFIG_TYPE_INTHEX:
      case QK_CONFIG_TYPE_INTDEC:
      case QK_CONFIG_TYPE_FLOAT:
        fragment_fill_value(p_config[i].value.i, 4, &frag);
        fragment_fill_value(p_config[i].proprieties.min, 4, &frag);
        fragment_fill_value(p_config[i].proprieties.max, 4, &frag);
        break;
      case QK_CONFIG_TYPE_DATETIME:
        fragment_fill_value(p_config[i].value.dateTime.year, 1, &frag);
        fragment_fill_value(p_config[i].value.dateTime.month, 1, &frag);
        fragment_fill_value(p_config[i].value.dateTime.day, 1, &frag);
      case QK_CONFIG_TYPE_TIME:
        fragment_fill_value(p_config[i].value.dateTime.hours, 1, &frag);
        fragment_fill_value(p_config[i].value.dateTime.minutes, 1, &frag);
        fragment_fill_value(p_config[i].value.dateTime.seconds, 1, &frag);
        break;
      case QK_CONFIG_TYPE_COMBO:
        fragment_fill_value(p_config[i].proprieties.size, 1, &frag);
        for(j=0; j<p_config[i].proprieties.size; j++)
        {
          fragment_fill_string(p_config[i].value.items[j], &frag);
        }
        break;
      }
    }
    break;
#if defined( QK_IS_DEVICE )
  case QK_PACKET_CODE_INFOSAMP:
    fragment_fill_value(_qk_core.sampling.frequency, 4, &frag);
    fragment_fill_value(_qk_core.sampling.mode, 1, &frag);
    fragment_fill_value(_qk_core.sampling.triggerClock, 1, &frag);
    fragment_fill_value(_qk_core.sampling.triggerScaler, 1, &frag);
    fragment_fill_value(_qk_core.sampling.N, 4, &frag);
    break;
  case QK_PACKET_CODE_INFODATA:
    p_data = _qk_device->buffers.data;
    fragment_fill_value(_qk_device->info._ndat, 1, &frag);
    fragment_fill_value(_qk_device->info.dataType, 1, &frag);
    for(i=0; i<_qk_device->info._ndat; i++)
      fragment_fill_array((uint8_t*)p_data[i].properties.label, _QK_LABEL_SIZE, &frag);
    break;
  case QK_PACKET_CODE_DATA:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    p_data = _qk_device->buffers.data;
    fragment_fill_value(_qk_device->info._ndat, 1, &frag);
    fragment_fill_value(_qk_device->info.dataType, 1, &frag);
    for(i=0; i<_qk_device->info._ndat; i++)
      fragment_fill_value(p_data[i].value.i, 4, &frag);
    break;
  case QK_PACKET_CODE_INFOEVENT:
    p_event = _qk_device->buffers.event;
    fragment_fill_value(_qk_device->info._nevt, 1, &frag);
    for(i=0; i<_qk_device->info._nevt; i++)
      fragment_fill_array((uint8_t*)p_event[i].properties.label, _QK_LABEL_SIZE, &frag);
    break;
  case QK_PACKET_CODE_EVENT:
    packet->flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_NOTIF;
    p_event = desc->event_fired;
    fragment_fill_value(p_event->_id, 1, &frag);
    fragment_fill_value(p_event->value.argc, 1, &frag);
    for(i=0; i<p_event->value.argc; i++)
      fragment_fill_value(_bytesFromFloat(p_event->value.argv[i]), 4, &frag);
    fragment_fill_string(p_event->value.text, &frag);
    break;
  case QK_PACKET_CODE_INFOACTION:
    p_action = _qk_device->buffers.action;
    fragment_fill_value(_qk_device->info._nact, 1, &frag);
    for(i=0; i<_qk_device->info._nact; i++)
    {
      fragment_fill_value(p_action[i].type, 1, &frag);
      fragment_fill_array((uint8_t*)p_action[i].properties.label, _QK_LABEL_SIZE, &frag);
      switch(p_action[i].type)
      {
      case QK_ACTION_TYPE_INT:
        fragment_fill_value(p_action[i].properties.min, 4, &frag);
        fragment_fill_value(p_action[i].properties.max, 4, &frag);
        fragment_fill_value(p_action[i].value.i, 4, &frag);
        break;
      case QK_ACTION_TYPE_BOOL:
        fragment_fill_value(p_action[i].value.b, 1, &frag);
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
    fragment_fill_string(desc->string_ptr, &frag);
    break;
  default: ;
  }
}

static void send_raw_byte(uint8_t b, qk_protocol *protocol)
{
  if(protocol->callback.sendBytes != 0)
    protocol->callback.sendBytes(&b, 1);
}

static void send_data_byte(uint8_t b, qk_protocol *protocol)
{
  if(b == QK_PROTOCOL_CTRL_FLAG || b == QK_PROTOCOL_CTRL_DLE)
    send_raw_byte(QK_PROTOCOL_CTRL_DLE, protocol);

  send_raw_byte(b, protocol);
  protocol->packet.checksum += b;
}

static void send_data_array(uint8_t *a, uint8_t size, qk_protocol *protocol)
{
  while(size-- > 0)
    send_data_byte(*a++, protocol);
}

static void send_ctrl_byte(uint8_t b, qk_protocol *protocol)
{
  if(b == QK_PROTOCOL_CTRL_FLAG)
  {
    if(flag(protocol->flags.reg, QK_PROTOCOL_FLAGMASK_TX))
    {
      send_data_byte(protocol->packet.checksum, protocol);
      protocol->packet.checksum = 0;
      protocol->flags.reg &= ~QK_PROTOCOL_FLAGMASK_TX;
    }
    else
      protocol->flags.reg |= QK_PROTOCOL_FLAGMASK_TX;
  }
  send_raw_byte(b, protocol);
}


void qk_protocol_send_packet(qk_packet *packet, qk_protocol *protocol)
{
  packet->checksum = packet->flags.ctrl + packet->code;

  if((protocol->flags.reg & QK_PROTOCOL_FLAGMASK_REXMIT) == 0)
  {
    //qk_packet_setSource(QK_BOARD_TYPE, packet);
  }
  protocol->flags.reg &= ~QK_PROTOCOL_FLAGMASK_REXMIT;

  send_ctrl_byte(QK_PROTOCOL_CTRL_FLAG, protocol);
  send_data_byte(packet->flags.ctrl & 0xFF, protocol);
  send_data_byte(packet->flags.ctrl >> 8, protocol);
  send_data_byte(packet->code, protocol);
  send_data_array((uint8_t*)packet->data, packet->dataLen, protocol);
  send_ctrl_byte(QK_PROTOCOL_CTRL_FLAG, protocol);
}

void _qk_protocol_send_code(int code, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;
  desc.code = code;
  qk_protocol_build_packet(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}

void _qk_protocol_send_string(const char *str, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;

  desc.code = QK_PACKET_CODE_STRING;
  desc.string_ptr = str;
  qk_protocol_build_packet(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}

#if defined( QK_IS_DEVICE )
void _qk_protocol_send_event(qk_event *e, qk_protocol *protocol)
{
  qk_packet packet;
  qk_packet_descriptor desc;
  desc.code = QK_PACKET_CODE_EVENT;
  desc.event_fired = e;
  qk_protocol_build_packet(&packet, &desc, protocol);
  packet.flags.ctrl |= QK_PACKET_FLAGMASK_CTRL_LASTFRAG;
  protocol->callback.sendPacket(&packet);
}
#endif

void qk_protocol_process_byte(uint8_t b, qk_protocol *protocol)
{
  qk_packet *pkt = &(protocol->packet);

  switch(b)
  {
  case QK_PROTOCOL_CTRL_FLAG:
    if(flag(protocol->flags.reg, QK_PROTOCOL_FLAGMASK_DLE) == 0)
    {
      if(flag(protocol->flags.reg, QK_PROTOCOL_FLAGMASK_RX) == 0)
      {
        pkt->address = 0;
        pkt->flags.ctrl = 0;
        pkt->code = 0;
        protocol->ctrl.count = 0;
        protocol->flags.reg |= (QK_PROTOCOL_FLAGMASK_RX | QK_PROTOCOL_FLAGMASK_VALID);
      }
      else
      {
        if(protocol->ctrl.count && flag(protocol->flags.reg, QK_PROTOCOL_FLAGMASK_VALID) == 1)
        {
          pkt->dataLen = protocol->ctrl.count - pkt->hdrLen - 1;
          protocol->flags.reg |= QK_PROTOCOL_FLAGMASK_NEWPACKET;
          protocol->flags.reg &= ~(QK_PROTOCOL_FLAGMASK_RX | QK_PROTOCOL_FLAGMASK_VALID);
        }
      }
      return;
    }
    break;
  case QK_PROTOCOL_CTRL_DLE:
    if(flag(protocol->flags.reg, QK_PROTOCOL_FLAGMASK_VALID) == 1)
    {
      if(flag(protocol->flags.reg, QK_PROTOCOL_FLAGMASK_DLE) == 0)
      {
        protocol->flags.reg |= QK_PROTOCOL_FLAGMASK_DLE;
        return;
      }
    }
    break;
  }

  if(flag(protocol->flags.reg, QK_PROTOCOL_FLAGMASK_VALID))
  {
    if(protocol->ctrl.count == 0)
    {
      pkt->flags.ctrl = b;
      packet_calc_header_length(pkt);
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
      protocol->flags.reg &= ~QK_PROTOCOL_FLAGMASK_VALID;
    }

    protocol->ctrl.count++;
  }

  protocol->flags.reg &= ~QK_PROTOCOL_FLAGMASK_DLE;
}

void qk_protocol_process_packet(qk_protocol *protocol)
{
  qk_packet *packet = &(protocol->packet);
  qk_ack *ack = (qk_ack*) &(protocol->ctrl.ack);
  uint16_t i_data;
  bool handled = false;

#ifdef QK_IS_DEVICE
  handled = device_process_packet(packet, protocol);
#endif
  if(handled) return;

  handled = board_process_packet(packet, protocol);
  if(handled) return;

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
    protocol->ctrl.ack.arg = packet_get_value(1, &i_data, packet);
    break;
  case QK_PACKET_CODE_ERR:
    protocol->ctrl.ack.type = packet->code;
    protocol->ctrl.ack.err = packet_get_value(1, &i_data, packet);
    protocol->ctrl.ack.arg = packet_get_value(4, &i_data, packet);
    break;
  default:
    handled = false;
  }

  if(!handled)
    qk_ack_set_ERROR(QK_ERR_CODE_UNKNOWN, (int32_t)packet->code, ack);

  _qk_protocol_send_code(QK_PACKET_CODE_ACK, protocol);
}

static bool board_process_packet(qk_packet *packet, qk_protocol *protocol)
{
  uint8_t buf[_QK_PACKET_DATBUF_SIZE/2];
  uint8_t count, idx;
  int32_t configValue;
  qk_datetime dateTime;
  uint16_t i_data;
  bool handled = true;

  qk_ack *ack = (qk_ack*) &(protocol->ctrl.ack);
  qk_ack_set_OK(ack);

  i_data = 0;
  switch(packet->code)
  {
  case QK_PACKET_CODE_SETQK:
    break;
  case QK_PACKET_CODE_SETNAME:
    packet_get_string((char*)buf, _QK_BOARD_NAME_SIZE, &i_data, packet);
    qk_board_set_name((char*)buf);
    break;
  case QK_PACKET_CODE_SETCONFIG:
    count = packet_get_value(1, &i_data, packet); // must be 1
    idx = packet_get_value(1, &i_data, packet);
    if(idx < qk_config_count())
    {
      switch(qk_config_get_type(idx))
      {
      case QK_CONFIG_TYPE_BOOL:
        configValue = packet_get_value(1, &i_data, packet);
        qk_config_set_value_b(idx, (bool) configValue);
        break;
      case QK_CONFIG_TYPE_INTDEC:
      case QK_CONFIG_TYPE_INTHEX:
        configValue = packet_get_value(4, &i_data, packet);
        qk_config_set_value_i(idx, configValue);
        break;
      case QK_CONFIG_TYPE_FLOAT:
        configValue = packet_get_value(4, &i_data, packet);
        qk_config_set_value_f(idx, _floatFromBytes(configValue));
        break;
      case QK_CONFIG_TYPE_DATETIME:
        dateTime.year = packet_get_value(1, &i_data, packet);
        dateTime.month = packet_get_value(1, &i_data, packet);
        dateTime.day = packet_get_value(1, &i_data, packet);
      case QK_CONFIG_TYPE_TIME:
        dateTime.hours = packet_get_value(1, &i_data, packet);
        dateTime.minutes = packet_get_value(1, &i_data, packet);
        dateTime.seconds = packet_get_value(1, &i_data, packet);
        qk_config_set_value_dt(idx, dateTime);
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
    _qk_protocol_send_code(QK_PACKET_CODE_ACK, protocol);
  }

  return handled;
}

#ifdef QK_IS_DEVICE
static bool device_process_packet(qk_packet *packet, qk_protocol *protocol)
{
  uint16_t i_data;
  bool handled = true;
  bool call_action;

  qk_action *act;
  qk_action_id act_id = 0;

  qk_ack *ack = (qk_ack*)&(protocol->ctrl.ack);
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
    _qk_protocol_send_code(QK_PACKET_CODE_INFOQK, protocol);
    _qk_protocol_send_code(QK_PACKET_CODE_INFOBOARD, protocol);
    _qk_protocol_send_code(QK_PACKET_CODE_INFOCONFIG, protocol);
    _qk_protocol_send_code(QK_PACKET_CODE_INFOSAMP, protocol);
    _qk_protocol_send_code(QK_PACKET_CODE_INFODATA, protocol);
    _qk_protocol_send_code(QK_PACKET_CODE_INFOEVENT, protocol);
    _qk_protocol_send_code(QK_PACKET_CODE_INFOACTION, protocol);
//    _qk_comm_sendSequenceEnd(seq_arg_code, protocol);
    break;
  case QK_PACKET_CODE_START:
    qk_start_sampling();
    break;
  case QK_PACKET_CODE_STOP:
    qk_stop_sampling();
    break;
  case QK_PACKET_CODE_SETSAMP:
    qk_sampling_set_frequency(packet_get_value(4, &i_data, packet));
    qk_sampling_set_mode((qk_sampling_mode)packet_get_value(1, &i_data, packet));
    qk_sampling_set_trigger(packet_get_value(1, &i_data, packet),
                            packet_get_value(1, &i_data, packet));
    qk_sampling_set_N(packet_get_value(4, &i_data, packet));
    break;
  case QK_PACKET_CODE_ACTUATE:
    act =  _qk_device->buffers.action;
    act_id = (qk_action_id) packet_get_value(1, &i_data, packet);
    packet_get_value(1, &i_data, packet); // action type
    act = &(_qk_device->buffers.action[act_id]);
    switch(act->type)
    {
    case QK_ACTION_TYPE_INT:
      act->value.i = packet_get_value(4, &i_data, packet);
      break;
    case QK_ACTION_TYPE_BOOL:
      act->value.b = packet_get_value(1, &i_data, packet);
      break;
    case QK_ACTION_TYPE_TEXT:
      //TODO
      break;
    }
    call_action = true;
    break;
  default:
    handled = false;
  }

  if(handled)
  {
    _qk_protocol_send_code(QK_PACKET_CODE_ACK, protocol);


    if(call_action && _qk_device->callbacks.action != 0)
      _qk_device->callbacks.action(act_id);
  }

  return handled;
}
#endif





