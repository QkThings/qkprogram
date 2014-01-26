/*!
 * \file qk_system.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_SYSTEM_H
#define QK_SYSTEM_H

#define QK_VERSION_MAJOR     0
#define QK_VERSION_MINOR     1
#define QK_VERSION_PATCH     0

/*****************************************************************************
 *      Settings
 *****************************************************************************/
#include "qk_settings.h"

/*****************************************************************************
 *      General (C standard libs)
 *****************************************************************************/
#if defined( _QK_DEBUG_ )
  #include <stdio.h>
  #define PRINT(...) printf(__VA_ARGS__)
#else
  #define PRINT(...)
#endif

/*****************************************************************************
 *      Program (public)
 *****************************************************************************/
#include "qk_program.h"

/*****************************************************************************
 *      HAL (Hardware Abstraction Layer)
 *****************************************************************************/
#include "hal.h"

/*****************************************************************************
 *      Core (private)
 *****************************************************************************/
//#include "../core/qk_utils.h"
#include "qk_packet.h"
#include "qk_protocol.h"

//#include "../core/qk_core.h"
//#if defined( QK_IS_GATEWAY )
//  #include "../core/qk_gateway.h"
//#elif defined( QK_IS_NETWORK )
//  #include "../core/qk_network.h"
//#elif defined( QK_IS_MODULE )
//  #include "../core/qk_module.h"
//#elif defined( QK_IS_DEVICE )
//  #include "../core/qk_device.h"
//#endif
//#include "../core/qk_board.h"

//int qk_main();

#endif /* QK_SYSTEM_H */
