/*
 * QkThings LICENSE
 * The open source framework and modular platform for smart devices.
 * Copyright (C) 2014 <http://qkthings.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 *  
 * This file is part of qkprogram
 */

#ifndef QK_DEVICE_H
#define QK_DEVICE_H

/** \addtogroup QkDevice
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_settings.h"


#if defined(QK_IS_DEVICE) || defined( DOXYGEN )

/******************************************************************************
   ENUMS
 ******************************************************************************/

/**
 * Data type
 */
typedef enum
{
  QK_DATA_TYPE_INT,
  QK_DATA_TYPE_FLOAT
} qk_data_type;

/**
 * Action type
 */
typedef enum
{
  QK_ACTION_TYPE_BOOL,
  QK_ACTION_TYPE_INT,
  QK_ACTION_TYPE_TEXT
} qk_action_type;

/******************************************************************************
   TYPES
 ******************************************************************************/

/**
 * Action ID
 */
typedef int32_t qk_action_id;

typedef volatile struct
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

/**
 * Data
 */
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

/**
 * Event
 */
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

/**
 * Action
 */
typedef struct
{
  qk_action_type      type;
  qk_action_value     value;
  qk_action_callbacks callbacks;
  qk_action_prop      properties;
} qk_action;

//TODO move all sampling related stuff to qk_device
/**
 * Sampling callback ID
 */
typedef enum
{
  QK_SAMPLING_CALLBACK_SAMPLE,
  QK_SAMPLING_CALLBACK_START,
  QK_SAMPLING_CALLBACK_STOP
} qk_sampling_callback;


/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

/**
 * @brief .
 */
void qk_data_set_buffer(qk_data *buf, uint32_t count);

/**
 * @brief .
 */
void qk_data_set_count(uint32_t count);

/**
 * @brief .
 */
void qk_data_set_label(uint8_t idx, char *label);

/**
 * @brief .
 */
void qk_data_set_type(qk_data_type type);

/**
 * @brief .
 */
void qk_data_set_value_i(uint8_t idx, int32_t value);

/**
 * @brief .
 */
void qk_data_set_value_f(uint8_t idx, float value);

/**
 * @brief .
 */
void qk_event_set_buffer(qk_event *buf, uint32_t count);

/**
 * @brief .
 */
void qk_event_set_label(uint8_t idx, const char *label);

/**
 * @brief .
 */
void qk_event_set_args(uint8_t idx, float *args, uint8_t count);

/**
 * @brief .
 */
bool qk_event_generate(uint8_t idx, char *message);

/**
 * @brief .
 */
void qk_action_set_buffer(qk_action *buf, unsigned int size);

/**
 * @brief .
 */
void qk_action_set_label(qk_action_id id, const char *label);

/**
 * @brief .
 */
void qk_action_set_type(qk_action_id id, qk_action_type type);

/**
 * @brief .
 */
void qk_action_set_value_i(qk_action_id id, int32_t value);

/**
 * @brief .
 */
void qk_action_set_value_b(qk_action_id id, bool value);

/**
 * @brief .
 */
qk_action_type qk_action_get_type(qk_action_id id);

/**
 * @brief .
 */
int32_t qk_action_get_value_i(qk_action_id id);

/**
 * @brief .
 */
bool qk_action_get_value_b(qk_action_id id);

/**
 * @brief .
 */
void qk_action_set_callback(void (*fnc)(qk_action_id));

/**
 * @brief .
 */
void qk_sampling_set_callback(qk_sampling_callback id, void (*fnc)(void));

#endif // QK_IS_DEVICE

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* QK_DEVICE_H */
