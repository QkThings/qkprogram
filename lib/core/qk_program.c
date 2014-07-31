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

void qk_init()
{
  hal_init();
  hal_blinkLED(1, 40);
  qk_core_init();
  qk_board_init();
  hal_blinkLED(1, 40);
  qk_setup();
  qk_board_setup();
  qk_board_ready();
  hal_blinkLED(2, 100);
}


int qk_main()
{
  qk_init();
  qk_loop();
  return 0;
}
