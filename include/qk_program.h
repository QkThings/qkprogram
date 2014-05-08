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
@mainpage qkprogram

qkprogram provides the main loop of your program, meaning that you can focus on writing the code that is specific
to the application you want to develop. By and large, you do that by declaring some buffers and defining some
callback functions.

*/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#if defined( BUILD_COMM )
  #define QK_IS_COMM
#elif defined( BUILD_DEVICE )
  #define QK_IS_DEVICE
#else
  #error "BUILD_TARGET is not defined"
#endif

#include "hal.h"
#include "qk_utils.h"
#include "qk_core.h"
#include "qk_comm.h"
#include "qk_device.h"
#include "qk_board.h"
#include "qk_packet.h"
#include "qk_protocol.h"

extern void qk_setup();

void qk_init();
int qk_main();

#endif /* QK_PROGRAM_H */
