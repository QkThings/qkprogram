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
  PRINT("hal_init OK\n");

  qk_core_init();
  PRINT("hal_init OK\n");

  qk_board_init();
  hal_blinkLED(1, 40);
  PRINT("hal_init OK\n");

  qk_setup();
  PRINT("qk_setup OK\n");

  qk_board_setup();
  PRINT("qk_board_setup OK\n");

  qk_board_ready();
  PRINT("qk_board_ready OK\n");

  hal_blinkLED(2, 100);
}


int qk_main()
{
  qk_init();
  qk_loop();
  return 0;
}
