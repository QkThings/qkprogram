///*
// * QkThings LICENSE
// * The open source framework and modular platform for smart devices.
// * Copyright (C) 2014 <http://qkthings.com>
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// */
//
//#include "../sys/qk_system.h"
//#include <avr/sleep.h>
//#include <avr/interrupt.h>
//
//hal_power_t _hal_power;
//
//static void goto_sleep();
//
//void hal_power_init()
//{
//  _hal_power.info.batteryStatus = HAL_BATTERY_STATUS_OK;
//
//
//}
//
//void hal_power_batteryCheck()
//{
//
//}
//
//void hal_power_EM1()
//{
//  set_sleep_mode(SLEEP_MODE_IDLE);
//  goto_sleep();
//}
//
//void hal_power_EM2()
//{
//
//}
//
//void hal_power_EM3()
//{
//
//}
//
//static void goto_sleep()
//{
//  sleep_enable();
//  sei();
//  sleep_cpu();
//  sleep_disable();
//}
//
