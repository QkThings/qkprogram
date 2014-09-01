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
