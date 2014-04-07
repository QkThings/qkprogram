/**
 * @file qk_board.h
 *  
 * This file is part of qkprogram
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
/**
 * Configuration type.
 */
typedef enum qk_config_type
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

typedef volatile struct qk_config_flags
{
  unsigned int changed : 1;
} qk_config_flags;


/**
 * Configuration word.
 */
typedef struct qk_config
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
 * @brief Sets the board's firmware version
 * @param version 16bit number
 */
void qk_board_set_version(uint16_t version);

/**
 * @brief Sets the board's name
 * @param name
 */
void qk_board_set_name(const char *name);

/**
 * @brief Get the board's firmware version
 */
int qk_board_version();

/**
 * @brief Sets the board's name
 */
char* qk_board_name();


void qk_config_set_buffer(qk_config *buffer, uint8_t count);

/**
 * @brief Sets the configuration's label
 * @param idx   index of the configuration label
 * @param label label
 */
void qk_config_set_label(uint8_t idx, const char *label);

/**
 * @brief Sets configuration's type
 * @param count
 */
void qk_config_set_type(uint8_t idx, qk_config_type type);
void qk_config_set_value_b(uint8_t idx, bool value);
void qk_config_set_value_i(uint8_t idx, int32_t value);
void qk_config_set_value_f(uint8_t idx, float value);
void qk_config_set_value_dt(uint8_t idx, qk_datetime dateTime);
uint8_t qk_config_count();
qk_config_type qk_config_get_type(uint8_t idx);
bool qk_config_value_b(uint8_t word);
uint32_t qk_config_value_i(uint8_t idx);
float qk_config_value_f(uint8_t idx);
qk_datetime qk_config_dt(uint8_t idx);
bool qk_config_is_changed(uint8_t idx);
void qk_config_set_handled(uint8_t idx);

void qk_set_init_callback(void (*fnc)(void));
void qk_set_config_callback(void (*fnc)(void));

#ifdef __cplusplus
}
#endif

#endif /* QK_BOARD_H */
