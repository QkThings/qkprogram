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

#include "qk_hal.h"
#include "qk_hal_p.h"

void qk_hal_init(void)
{
  _qk_peripheral_setup();
  qk_gpio_set_mode(_QK_HAL_LED, QK_GPIO_MODE_OUTPUT);
}

//#include "qk_system.h"
//#include "hal_mcu.h"
//
//void qk_hal_init()
//{
////  _hal_clock_init();
////  _hal_gpio_init();
////  _hal_timer_init();
////  _hal_uart_init();
////
////  hal_mcu_init();
////  hal_clock_init();
////  hal_gpio_init();
////  hal_timer_init();
////  hal_uart_init();
////
////  hal_finalize();
////
////#ifdef _QK_FEAT_RTC_
////  // RTC
////#endif
////#ifdef _QK_FEAT_EEPROM_
////  // EEPROM
////#endif
//}
