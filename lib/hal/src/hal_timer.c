/*
 * hal_timer.c
 *
 *  Created on: 5 de Dez de 2013
 *      Author: mribeiro
 */

#include "../sys/qk_system.h"

hal_timer_t _hal_timer[2];

void _hal_timer_init()
{
  memset(&_hal_timer[HAL_TIMER_ID_1], 0, sizeof(hal_timer_t));
  memset(&_hal_timer[HAL_TIMER_ID_2], 0, sizeof(hal_timer_t));
}
