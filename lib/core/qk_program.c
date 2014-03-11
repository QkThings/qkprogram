/**
 * @file qk_program.c
 *
 * @author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"
#include "qk_program.h"

extern void qk_setup();

/** @brief The Qk main function
 */
int qk_main()
{
  hal_init();
  hal_blinkLED(1, 50);
  qk_core_init();
  qk_board_init();
  hal_blinkLED(1, 50);
  qk_setup();
  qk_board_setup();
  hal_blinkLED(2, 100);

  qk_loop();
  return 0;
}
