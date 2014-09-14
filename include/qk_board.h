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

#ifndef QK_BOARD_H
#define QK_BOARD_H

/** \addtogroup QkBoard
 *  @{
 *  @brief .
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_settings.h"
#include "qk_utils.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/
/**
 * Configuration type.
 */
typedef enum
{
  QK_CONFIG_TYPE_INTDEC,  //!< Integer represented as decimal number
  QK_CONFIG_TYPE_INTHEX,  //!< Integer represented as hexadecimal number
  QK_CONFIG_TYPE_FLOAT,   //!< Floating point number (single precision)
  QK_CONFIG_TYPE_BOOL,    //!< Boolean value
  QK_CONFIG_TYPE_COMBO,   //!< Combo list
  QK_CONFIG_TYPE_TIME,    //!< Time
  QK_CONFIG_TYPE_DATETIME,//!< Date and time
} qk_config_type;

/******************************************************************************
   STRUCTS
 ******************************************************************************/

typedef struct qk_config_prop
{
  char label[_QK_LABEL_SIZE];
  int32_t min, max;
  uint8_t size;
} qk_config_prop;

typedef struct qk_config_value
{
  union {
    bool     b;
    int32_t  i;
    uint32_t ui;
    uint32_t bytes_value;
    uint8_t  bytes[4];
    float    f;
    char    **items;
    qk_datetime dateTime;
  };
} qk_config_value;

typedef volatile struct
{
  unsigned int changed : 1;
} qk_config_flags;


/**
 * Configuration word.
 */
typedef struct
{
  qk_config_type  type;
  qk_config_value value;
  qk_config_prop  proprieties;
  qk_config_flags flags;
} qk_config;


/******************************************************************************
   PROTOTYPES
 ******************************************************************************/

/**
 * @brief .
 */
void qk_board_set_version(uint16_t version);

/**
 * @brief .
 */
void qk_board_set_name(const char *name);

/**
 * @brief .
 */
int qk_board_get_version();

/**
 * @brief .
 */
char* qk_board_get_name();

/**
 * @brief .
 */
void qk_board_led_set(bool state);

/**
 * @brief .
 */
void qk_board_led_blink(unsigned int n, unsigned int msec);

/**
 * @brief .
 */
void qk_config_set_buffer(qk_config *buffer, uint8_t count);

/**
 * @brief .
 */
void qk_config_set_label(uint8_t idx, const char *label);

/**
 * @brief .
 */
void qk_config_set_type(uint8_t idx, qk_config_type type);

/**
 * @brief .
 */
void qk_config_set_value_b(uint8_t idx, bool value);

/**
 * @brief .
 */
void qk_config_set_value_i(uint8_t idx, int32_t value);

/**
 * @brief .
 */
void qk_config_set_value_f(uint8_t idx, float value);

/**
 * @brief .
 */
void qk_config_set_value_dt(uint8_t idx, qk_datetime dateTime);

/**
 * @brief .
 */
uint8_t qk_config_count();

/**
 * @brief .
 */
qk_config_type qk_config_get_type(uint8_t idx);

/**
 * @brief .
 */
bool qk_config_value_b(uint8_t word);

/**
 * @brief .
 */
uint32_t qk_config_value_i(uint8_t idx);

/**
 * @brief .
 */
float qk_config_value_f(uint8_t idx);

/**
 * @brief .
 */
qk_datetime qk_config_dt(uint8_t idx);


bool qk_config_is_changed(uint8_t idx);
void qk_config_set_handled(uint8_t idx);

void qk_set_init_callback(void (*fnc)(void)); //TODO is this useful?

/**
 * @brief .
 */
void qk_config_set_callback(void (*fnc)(void));

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* QK_BOARD_H */
