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
//#define _QK_DEBUG
#if defined( _QK_DEBUG)
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
#include "hal_p.h"

/*****************************************************************************
 *      Core (private)
 *****************************************************************************/
//#include "../core/qk_utils.h"
#include "qk_packet_p.h"
#include "qk_protocol_p.h"
#include "qk_fragment_p.h"

#include "qk_core_p.h"
#include "qk_comm_p.h"
#include "qk_device_p.h"
#include "qk_board_p.h"


#endif /* QK_SYSTEM_H */
