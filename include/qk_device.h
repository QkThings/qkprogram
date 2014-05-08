/**
 * @file qk_device.h
 *  
 * This file is part of qkprogram
 */

#ifndef QK_DEVICE_H
#define QK_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_settings.h"

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
  QK_ACTION_TYPE_BOOL,
  QK_ACTION_TYPE_INT,
  QK_ACTION_TYPE_TEXT
} qk_action_type;

/******************************************************************************
   TYPES
 ******************************************************************************/
typedef int32_t qk_action_id;

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
  char label[_QK_LABEL_SIZE];
} qk_data_prop;

typedef volatile struct qk_data
{
  //qk_data_type_t  type;
  qk_data_value value;
  qk_data_prop  properties;
} qk_data;

typedef volatile struct qk_event_value
{
  uint8_t argc;
  int32_t argv[_QK_EVENT_MAX_ARGS];
  char *text;
} qk_event_value;

typedef struct qk_event_prop
{
  char label[_QK_LABEL_SIZE];
} qk_event_prop;

typedef volatile struct qk_event
{
  uint8_t _id;
  qk_event_value value;
  qk_event_prop  properties;
} qk_event;

typedef struct qk_action_value
{
//  uint8_t argc;
//  int32_t argv[_QK_ACTION_MAX_ARGS];
  int32_t i;
  bool    b;
  char    *t;
} qk_action_value;

typedef struct qk_action_prop
{
  char label[_QK_LABEL_SIZE];
  int32_t min,max;
} qk_action_prop;

typedef struct qk_action_callbacks
{
  void (*run)(void);
} qk_action_callbacks;

typedef struct qk_action
{
  qk_action_type      type;
  qk_action_value     value;
  qk_action_callbacks callbacks;
  qk_action_prop      properties;
} qk_action;

//TODO move all sampling related stuff to qk_device
typedef enum qk_sampling_callback_id
{
  QK_SAMPLING_CALLBACK_SAMPLE,
  QK_SAMPLING_CALLBACK_START,
  QK_SAMPLING_CALLBACK_STOP
} qk_sampling_callback;


/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

void qk_data_set_buffer(qk_data *buf, uint32_t count);
void qk_data_set_count(uint32_t count);
void qk_data_set_label(uint8_t idx, char *label);
void qk_data_set_type(qk_data_type type);
void qk_data_set_value_i(uint8_t idx, int32_t value);
void qk_data_set_value_f(uint8_t idx, float value);

void qk_event_set_buffer(qk_event *buf, uint32_t count);
void qk_event_set_label(uint8_t idx, const char *label);
void qk_event_set_args(uint8_t idx, float *args, uint8_t count);
bool qk_event_generate(uint8_t idx, char *message);

void qk_action_set_buffer(qk_action *buf, unsigned int size);
void qk_action_set_label(qk_action_id id, const char *label);
void qk_action_set_type(qk_action_id id, qk_action_type type);
void qk_action_set_value_i(qk_action_id id, int32_t value);
void qk_action_set_value_b(qk_action_id id, bool value);

qk_action_type qk_action_get_type(qk_action_id id);
int32_t qk_action_get_value_i(qk_action_id id);
bool qk_action_get_value_b(qk_action_id id);

void qk_action_set_callback(void (*fnc)(qk_action_id));

void qk_sampling_set_callback(qk_sampling_callback id, void (*fnc)(void));

#endif // QK_IS_DEVICE

#ifdef __cplusplus
}
#endif

#endif /* QK_DEVICE_H */
