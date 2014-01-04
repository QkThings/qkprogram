/*!
 * \file qk_device.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_DEVICE_H
#define QK_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../sys/qk_settings.h"

#if defined(QK_IS_DEVICE)

/******************************************************************************
   ENUMS
 ******************************************************************************/

typedef enum qk_data_type
{
  QK_DATA_TYPE_INT,
  QK_DATA_TYPE_FLOAT
} qk_data_type;

typedef enum qk_action_type
{
  QK_ACTION_TYPE_INT,
  QK_ACTION_TYPE_TEXT
} qk_action_type;

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef volatile struct qk_data_value
{
  union
  {
    int32_t i;
    float    f;
  };
} qk_data_value;

typedef struct qk_data_prop
{
  char label[QK_LABEL_SIZE];
} qk_data_prop;

typedef volatile struct qk_data
{
  //qk_data_type_t  type;
  qk_data_value value;
  qk_data_prop  proprieties;
} qk_data;

typedef volatile struct qk_event_value
{
  uint8_t argc;
  float argv[QK_EVENT_MAX_ARGS];
  char *text;
} qk_event_value;

typedef struct qk_event_prop
{
  char label[QK_LABEL_SIZE];
} qk_event_prop;

typedef volatile struct qk_event
{
  uint8_t _id;
  qk_event_value value;
  qk_event_prop  proprieties;
} qk_event;

typedef struct qk_action_value
{
  uint8_t argc;
  int32_t *argv;
  char    *text;
} qk_action_value;

typedef struct qk_action_prop
{
  char label[QK_LABEL_SIZE];
  int32_t min,max;
} qk_action_prop;

typedef struct qk_action_callbacks
{
  void (*run)(void);
} qk_action_callbacks;

typedef struct qk_action_t
{
  qk_action_type      type;
  qk_action_value     value;
  qk_action_callbacks callbacks;
  qk_action_prop      proprieties;
} qk_action;

typedef volatile struct qk_device_buffers
{
  qk_data *data;
  qk_event *event;
  qk_action *action;
} qk_device_buffers;

typedef volatile struct qk_device_callbacks
{
  void (*sample)(void);
  void (*start)(void);
  void (*stop)(void);
} qk_device_callbacks;

typedef volatile struct qk_device_info
{
  uint32_t _ndat;
  uint32_t _nact;
  uint32_t _nevt;
  uint32_t _ncfg;
  qk_data_type dataType;
} qk_device_info;

typedef struct qk_device
{
  qk_device_info      info;
  qk_device_buffers   buffers;
  qk_device_callbacks callbacks;
} qk_device;

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define QK_DEFINE_DEVICE(name)     qk_device *_qk_device = &(name)

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern qk_cb _pendingEvents;
extern qk_device *_qk_device;


/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

void _qk_device_init();
void _qk_device_setup();
void qk_startSampling();
void qk_stopSampling();
bool qk_fireEvent(uint8_t idx, float *values, uint8_t count, char *message);

/******************************************************************************/
static inline
qk_data_type qk_dataType()
{
  return _qk_device->info.dataType;
}

static inline
void qk_setDataBuffer(qk_data *buf, uint32_t count)
{
  _qk_device->buffers.data = buf;
  _qk_device->info._ndat = count;
}

static inline
void qk_setDataCount(uint32_t count)
{
  _qk_device->info._ndat = count;
}

static inline
void qk_setDataLabel(uint8_t idx, char *label)
{
  strcpy(_qk_device->buffers.data[idx].proprieties.label, label);
}

static inline
void qk_setDataType(qk_data_type type)
{
  _qk_device->info.dataType = type;
}

static inline
void qk_setDataValueI(uint8_t idx, int32_t value)
{
  _qk_device->buffers.data[idx].value.i = value;
}

static inline
void qk_setDataValueF(uint8_t idx, float value)
{
  _qk_device->buffers.data[idx].value.f = value;
}

static inline
void qk_setEventBuffer(qk_event *buf, uint32_t count)
{
  _qk_device->buffers.event = buf;
  _qk_device->info._nevt = count;
}

static inline
void qk_setEventLabel(uint8_t idx, const char *label)
{
  strcpy(_qk_device->buffers.event[idx].proprieties.label, label);
}

/*static inline
void qk_setEventValues(uint8_t idx, int32_t *values, uint8_t count)
{
  _qk_device->buffers.event[idx].value.argc = count;
  _qk_device->buffers.event[idx].value.argv = values;
}
static inline
void qk_setEventMessage(uint8_t idx, char *message)
{
  _qk_device->buffers.event[idx].value.text = message;
}*/


static inline
qk_cb* qk_pendingEvents()
{
  return &_pendingEvents;
}
static inline
void qk_setSampleCallback(void (*fnc)(void))
{
  _qk_device->callbacks.sample = fnc;
}
static inline
void qk_setStartCallback(void (*fnc)(void))
{
  _qk_device->callbacks.start = fnc;
}
static inline
void qk_setStopCallback(void (*fnc)(void))
{
  _qk_device->callbacks.stop = fnc;
}

#endif // QK_IS_DEVICE

#ifdef __cplusplus
}
#endif

#endif /* QK_DEVICE_H */
