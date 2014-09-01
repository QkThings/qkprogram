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
