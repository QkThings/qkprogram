/*!
 * @file qk_program.h
 *
 * @author qkthings
 *  
 * This file is part of QkProgram
 */

#ifndef QK_PROGRAM_H
#define QK_PROGRAM_H

/**
@mainpage QkProgram

QkProgram provides the main loop of your program, meaning you can focus on writing application-specific ode. By and large, you do that by declaring some buffers and defining some
callback functions.

*/

/** \addtogroup QkProgram
 *  @{
 */

#if defined( BUILD_COMM )
  #define QK_IS_COMM
#elif defined( BUILD_DEVICE )
  #define QK_IS_DEVICE
#else
  #error "BUILD_COMM or BUILD_DEVICE?"
#endif


#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "hal.h"
#include "qk_utils.h"
#include "qk_core.h"
#include "qk_comm.h"
#include "qk_device.h"
#include "qk_board.h"
#include "qk_packet.h"
#include "qk_protocol.h"

/**
 * @brief .
 */
extern void qk_setup();

/**
 * @brief .
 */
void qk_init();

/**
 * @brief .
 */
int qk_main();

/** @}*/

#endif /* QK_PROGRAM_H */
