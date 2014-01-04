/*!
 * \file qk_program.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_PROGRAM_H
#define QK_PROGRAM_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#if defined ( BUILD_GATEWAY )
  #define QK_IS_GATEWAY
#elif defined( BUILD_NETWORK )
  #define QK_IS_NETWORK
#elif defined( BUILD_COMM )
  #define QK_IS_COMM
#elif defined( BUILD_DEVICE )
  #define QK_IS_DEVICE
#else
  #error "build target not specified"
#endif

#include "../src/core/qk_utils.h"
#include "../src/core/qk_core.h"
#include "../src/core/qk_gateway.h"
#include "../src/core/qk_network.h"
#include "../src/core/qk_device.h"
#include "../src/core/qk_board.h"

int qk_main();

#endif /* QK_PROGRAM_H */
