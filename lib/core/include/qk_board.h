/**
 * @file qk_board.h
 * @author qkthings
 *  
 * This file is part of QkProgram
 */

#ifndef QK_BOARD_H
#define QK_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_settings.h"
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
} qk_board_type;

typedef enum qk_config_type
{
  QK_CONFIG_TYPE_INTDEC,
  QK_CONFIG_TYPE_INTHEX,
  QK_CONFIG_TYPE_FLOAT,
  QK_CONFIG_TYPE_BOOL,
  QK_CONFIG_TYPE_COMBO,
  QK_CONFIG_TYPE_TIME,
  QK_CONFIG_TYPE_DATETIME,
} qk_config_type;

/******************************************************************************
   STRUCTS
 ******************************************************************************/

typedef struct qk_config_prop
{
  char label[QK_LABEL_SIZE];
  int32_t min, max;
  uint8_t size;
} qk_config_prop;

typedef struct qk_config_value
{
  union {
    bool    b;
    int32_t i;
    float   f;
    char    **items;
    qk_datetime dateTime;
  };
} qk_config_value;

typedef volatile struct qk_config_flags
{
  unsigned int changed : 1;
} qk_config_flags;


typedef struct qk_config
{
  qk_config_type  type;
  qk_config_value value;
  qk_config_prop  proprieties;
  qk_config_flags flags;
} qk_config;

/******************************************************************************/
typedef struct qk_board_info {
  uint64_t address;
  uint16_t version;
  char     name[QK_BOARD_NAME_SIZE];
  uint8_t  _ncfg;
} qk_board_info;

typedef struct qk_board_buffer {
  qk_config *config;
} qk_board_buffer;

typedef struct qk_board_callbacks {
  void (*init)(void);
  void (*config)(void);
} qk_board_callbacks;

typedef struct qk_board {
  qk_board_info       info;
  qk_board_buffer     buffers;
  qk_board_callbacks  callbacks;
#if defined( QK_IS_GATEWAY )
  qk_gateway_t  gateway;
#elif defined( QK_IS_NETWORK )
  qk_network_t  network;
#elif defined( QK_IS_MODULE )
  qk_module_t   module;
#elif defined( QK_IS_DEVICE )
  qk_device         device;
#endif
} qk_board;

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
#define QK_DEFINE_BOARD(name)   qk_board *_qk_board = &name; \
                                QK_DEFINE_GATEWAY(name.gateway)
#elif defined( QK_IS_NETWORK )
#define QK_DEFINE_BOARD(name)   qk_board *_qk_board = &name; \
                                QK_DEFINE_NETWORK(name.network)
#elif defined( QK_IS_MODULE )
#define QK_DEFINE_BOARD(name)   qk_board *_qk_board = &name; \
                                QK_DEFINE_MODULE(name.module)
#elif defined( QK_IS_DEVICE )
#define QK_DEFINE_BOARD(name)   qk_board *_qk_board = &name; \
                                QK_DEFINE_DEVICE(name.device)
#endif

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/

extern qk_board *_qk_board;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_board_init();
void qk_board_setup();

/******************************************************************************/

/**
 * @brief Sets the firmware version of the board
 * @param version 16bit version number
 */
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
void qk_setConfigType(uint8_t idx, qk_config_type type)
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
void qk_setConfigValueDT(uint8_t idx, qk_datetime dateTime)
{
  _qk_board->buffers.config[idx].value.dateTime = dateTime;
}

static inline
uint8_t qk_configCount()
{
  return _qk_board->info._ncfg;
}

static inline
qk_config_type qk_configType(uint8_t idx)
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
qk_datetime qk_configDateTime(uint8_t idx)
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
void qk_setConfigBuffer(qk_config *buffer)
{
  _qk_board->buffers.config = buffer;
}

#ifdef __cplusplus
}
#endif

#endif /* QK_BOARD_H */
