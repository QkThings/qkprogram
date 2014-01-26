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

Here you should tell us about how your game works.  How to play,
any special rules you have, etc.  Also, explain any non-trivial
design decisions you make, like how your ball gets its position
updated, how you designed your buffer for readchar(), etc.  You should
also comment on the stability of your code.  Any big bugs should be listed
here.  Basically, anything that you think we need to know in general about
your project should go here.

Any additional comments you want to make can go here.  Did you like the
project?  Was it too hard, too easy?  My TA smells bad.  Well, you get
the idea.

As a last little note, please make sure that all the documentation you
turn in is true and accurate.  Otherwise, we will deduct points.

*/

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

#include "qk_utils.h"
#include "qk_core.h"
#include "qk_gateway.h"
#include "qk_network.h"
#include "qk_device.h"
#include "qk_board.h"

int qk_main();

#endif /* QK_PROGRAM_H */
