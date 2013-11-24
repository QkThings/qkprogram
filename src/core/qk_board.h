/*!
 * \file qk_board.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_BOARD_H
#define QK_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_utils.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/

typedef enum qk_board_type {
  QK_BOARD_TYPE_HOST,
  QK_BOARD_TYPE_MODULE,
  QK_BOARD_TYPE_DEVICE,
  QK_BOARD_TYPE_NETWORK,
  QK_BOARD_TYPE_GATEWAY
} qk_board_type_t;

typedef enum qk_config_type
{
  QK_CONFIG_TYPE_INTDEC,
  QK_CONFIG_TYPE_INTHEX,
  QK_CONFIG_TYPE_FLOAT,
  QK_CONFIG_TYPE_BOOL,
  QK_CONFIG_TYPE_COMBO,
  QK_CONFIG_TYPE_TIME,
  QK_CONFIG_TYPE_DATETIME,
} qk_config_type_t;

/******************************************************************************
   STRUCTS
 ******************************************************************************/

typedef struct qk_config_prop
{
  char label[QK_LABEL_SIZE];
  int32_t min, max;
  uint8_t size;
} qk_config_prop_t;

typedef struct qk_config_value
{
  union {
    bool    b;
    int32_t i;
    float   f;
    char    **items;
    qk_datetime_t dateTime;
  };
} qk_config_value_t;

typedef volatile struct qk_config_flags
{
  unsigned int changed : 1;
} qk_config_flags_t;


typedef struct qk_config
{
  qk_config_type_t  type;
  qk_config_value_t value;
  qk_config_prop_t  proprieties;
  qk_config_flags_t flags;
} qk_config_t;

/******************************************************************************/
typedef struct qk_board_info {
  uint64_t address;
  uint16_t version;
  char     name[QK_BOARD_NAME_SIZE];
  uint8_t  _ncfg;
} qk_board_info_t;

typedef struct qk_board_buffers {
  qk_config_t *config;
} qk_board_buffers_t;

typedef struct qk_board_callbacks {
  void (*init)(void);
  void (*config)(void);
} qk_board_callbacks_t;

typedef struct qk_board {
  qk_board_info_t      info;
  qk_board_buffers_t   buffers;
  qk_board_callbacks_t callbacks;
#if defined( QK_IS_GATEWAY )
  qk_gateway_t  gateway;
#elif defined( QK_IS_NETWORK )
  qk_network_t  network;
#elif defined( QK_IS_MODULE )
  qk_module_t   module;
#elif defined( QK_IS_DEVICE )
  qk_device_t   device;
#endif
} qk_board_t;

/******************************************************************************
   DEFINES
 ******************************************************************************/

#if defined( QK_IS_GATEWAY )
#define QK_BOARD_TYPE QK_BOARD_TYPE_GATEWAY
#elif defined( QK_IS_NETWORK )
#define QK_BOARD_TYPE QK_BOARD_TYPE_NETWORK
#elif defined( QK_IS_MODULE )
#define QK_BOARD_TYPE QK_BOARD_TYPE_MODULE
#elif defined( QK_IS_DEVICE )
#define QK_BOARD_TYPE QK_BOARD_TYPE_DEVICE
#endif

#if defined( QK_IS_GATEWAY )
#define QK_DEFINE_BOARD(name)   qk_board_t *_qk_board = &name; \
                                QK_DEFINE_GATEWAY(name.gateway)
#elif defined( QK_IS_NETWORK )
#define QK_DEFINE_BOARD(name)   qk_board_t *_qk_board = &name; \
                                QK_DEFINE_NETWORK(name.network)
#elif defined( QK_IS_MODULE )
#define QK_DEFINE_BOARD(name)   qk_board_t *_qk_board = &name; \
                                QK_DEFINE_MODULE(name.module)
#elif defined( QK_IS_DEVICE )
#define QK_DEFINE_BOARD(name)   qk_board_t *_qk_board = &name; \
                                QK_DEFINE_DEVICE(name.device)
#endif

#define static inline static inline

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/

extern qk_board_t *_qk_board;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

void qk_board_init();
void qk_board_setup();
void _qk_board_wakeUp();
void _qk_board_comm_sendPacket(qk_packet_t *packet);
void _qk_board_comm_processByte(uint8_t b);
void _qk_board_comm_processPacket(qk_packet_t *packet);

/******************************************************************************/
static inline
void qk_setBoardVersion(uint16_t version)
{
  _qk_board->info.version = version;
}

static inline
void qk_setBoardName(const char *name)
{
  strcpy(_qk_board->info.name, name);
}

static inline
int qk_boardVersion()
{
  return _qk_board->info.version;
}

static inline
char* qk_boardName()
{
  return _qk_board->info.name;
}


static inline
void qk_setConfigCount(uint8_t count)
{
  _qk_board->info._ncfg = count;
}
static inline
void qk_setConfigLabel(uint8_t idx, char *label)
{
  strcpy(_qk_board->buffers.config[idx].proprieties.label, label);
}
static inline
void qk_setConfigType(uint8_t idx, qk_config_type_t type)
{
  _qk_board->buffers.config[idx].type = type;
}
static inline
void qk_setConfigValueB(uint8_t idx, bool value)
{
  _qk_board->buffers.config[idx].value.b = value;
}
static inline
void qk_setConfigValueI(uint8_t idx, int32_t value)
{
  _qk_board->buffers.config[idx].value.i = value;
}
static inline
void qk_setConfigValueF(uint8_t idx, float value)
{
  _qk_board->buffers.config[idx].value.f = value;
}

static inline
void qk_setConfigValueDT(uint8_t idx, qk_datetime_t dateTime)
{
  _qk_board->buffers.config[idx].value.dateTime = dateTime;
}

static inline
uint8_t qk_configCount()
{
  return _qk_board->info._ncfg;
}

static inline
qk_config_type_t qk_configType(uint8_t idx)
{
  return _qk_board->buffers.config[idx].type;
}

static inline
bool qk_configValueB(uint8_t word)
{
  return _qk_board->buffers.config[word].value.b;
}

static inline
uint32_t qk_configValueI(uint8_t idx)
{
  return _qk_board->buffers.config[idx].value.i;
}

static inline
float qk_configValueF(uint8_t idx)
{
  return _qk_board->buffers.config[idx].value.f;
}

static inline
char** qk_configItems(uint8_t idx)
{
  return _qk_board->buffers.config[idx].value.items;
}

static inline
qk_datetime_t qk_configDateTime(uint8_t idx)
{
  return _qk_board->buffers.config[idx].value.dateTime;
}

static inline
bool qk_isConfigChanged(uint8_t idx)
{
  return _qk_board->buffers.config[idx].flags.changed;
}

static inline
void qk_setConfigHandled(uint8_t idx)
{
  _qk_board->buffers.config[idx].flags.changed = false;
}

static inline
void qk_setInitCallback(void (*fnc)(void))
{
  _qk_board->callbacks.init = fnc;
}

static inline
void qk_setConfigCallback(void (*fnc)(void))
{
  _qk_board->callbacks.config = fnc;
}

static inline
void qk_setConfigBuffer(qk_config_t *buffer)
{
  _qk_board->buffers.config = buffer;
}

#ifdef __cplusplus
}
#endif

#endif /* QK_BOARD_H */
