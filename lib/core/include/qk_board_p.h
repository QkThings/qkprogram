/**
 * @file qk_board.h
 *  
 * This file is part of QkProgram
 */

#ifndef QK_BOARD_P_H
#define QK_BOARD_P_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qk_board.h"
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

/******************************************************************************
   STRUCTS
 ******************************************************************************/

typedef struct qk_board_info {
  uint64_t address;
  uint16_t version;
  char     name[_QK_BOARD_NAME_SIZE];
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

#ifdef __cplusplus
}
#endif

#endif /* QK_BOARD_H */
