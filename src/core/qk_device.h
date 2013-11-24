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

#if defined(QK_IS_DEVICE)

/******************************************************************************
   ENUMS
 ******************************************************************************/

typedef enum qk_data_type
{
  QK_DATA_TYPE_INT,
  QK_DATA_TYPE_FLOAT
} qk_data_type_t;

typedef enum qk_action_type
{
  QK_ACTION_TYPE_INT,
  QK_ACTION_TYPE_TEXT
} qk_action_type_t;

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct qk_data_value_t
{
  union
  {
    int32_t i;
    float    f;
  };
} qk_data_value_t;

typedef struct qk_data_prop_t
{
  char label[QK_LABEL_SIZE];
} qk_data_prop_t;

typedef struct qk_data_t
{
  //qk_data_type_t  type;
  qk_data_value_t value;
  qk_data_prop_t  proprieties;
} qk_data_t;

typedef struct qk_event_value_t
{
  uint8_t argc;
  float argv[QK_EVENT_MAX_ARGS];
  char *text;
} qk_event_value_t;

typedef struct qk_event_prop_t
{
  char label[QK_LABEL_SIZE];
} qk_event_prop_t;

typedef struct qk_event_t
{
  uint8_t _id;
  qk_event_value_t value;
  qk_event_prop_t  proprieties;
} qk_event_t;

typedef struct qk_action_value_t
{
  uint8_t argc;
  int32_t *argv;
  char    *text;
} qk_action_value_t;

typedef struct qk_action_prop_t
{
  char label[QK_LABEL_SIZE];
  int32_t min,max;
} qk_action_prop_t;

typedef struct qk_action_callbacks_t
{
  void (*run)(void);
} qk_action_callbacks_t;

typedef struct qk_action_t
{
  qk_action_type_t      type;
  qk_action_value_t     value;
  qk_action_callbacks_t callbacks;
  qk_action_prop_t      proprieties;
} qk_action_t;

typedef struct qk_device_buffers_t
{
  qk_data_t *data;
  qk_event_t *event;
  qk_action_t *action;
} qk_device_buffers_t;

typedef struct qk_device_callbacks_t
{
  void (*sample)(void);
  void (*start)(void);
  void (*stop)(void);
} qk_device_callbacks_t;

typedef struct qk_device_info_t
{
  uint32_t _ndat;
  uint32_t _nact;
  uint32_t _nevt;
  uint32_t _ncfg;
  uint8_t _maxFiredEvents;
  qk_data_type_t dataType;
} qk_device_info_t;

typedef struct qk_device_t
{
  qk_device_info_t      info;
  qk_device_buffers_t   buffers;
  qk_device_callbacks_t callbacks;
} qk_device_t;

/******************************************************************************
   DEFINES
 ******************************************************************************/
#define QK_DEFINE_DEVICE(name)     qk_device_t *_qk_device = &(name)

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern qk_cb_t _pendingEvents;
extern qk_device_t *_qk_device;

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
qk_data_type_t qk_dataType()
{
  return _qk_device->info.dataType;
}

static inline
void qk_setDataCount(uint8_t count)
{
  _qk_device->info._ndat = count;
}
static inline
void qk_setDataBuffer(qk_data_t *buf)
{
  _qk_device->buffers.data = buf;
}

static inline
void qk_setDataLabel(uint8_t idx, char *label)
{
  strcpy(_qk_device->buffers.data[idx].proprieties.label, label);
}

static inline
void qk_setDataType(qk_data_type_t type)
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
void qk_setEventCount(uint8_t count)
{
  _qk_device->info._nevt = count;
}

static inline
void qk_setEventBuffer(qk_event_t *buf)
{
  _qk_device->buffers.event = buf;
}

static inline
void qk_setEventLabel(uint8_t idx, char *label)
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
void qk_setMaxFiredEvents(uint8_t count)
{
  _qk_device->info._maxFiredEvents = count;
}

static inline
uint8_t _qk_maxFiredEvents()
{
  return _qk_device->info._maxFiredEvents;
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
