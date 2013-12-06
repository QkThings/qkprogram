/*!
 * \file hal.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include "hal_mcu.h"

void hal_init()
{
  hal_mcu_init();

//  _hal_clock_init();
//  _hal_gpio_init();
//  _hal_timer_init();
//  //_hal_uart_init();

  hal_clock_init();
  hal_gpio_init();
  hal_timer_init();
  hal_uart_init();

#ifdef _QK_FEAT_RTC_
  // RTC
#endif
#ifdef _QK_FEAT_EEPROM_
  // EEPROM
#endif
}
