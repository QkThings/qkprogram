/*!
 * \file hal_timer.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "qk_system.h"

hal_timer_t _hal_timer[2];

/******************************************************************************/

void hal_timer_init()
{

}

void hal_timer_reinit()
{

}

void hal_timer_start(hal_timer_id_t tmr)
{

}
void hal_timer_stop(hal_timer_id_t tmr)
{

}
void hal_timer_reset(hal_timer_id_t tmr)
{

}
void hal_timer_setPeriod(hal_timer_id_t tmr, uint32_t per, hal_timer_scale_t s)
{

}
void hal_timer_setFrequency(hal_timer_id_t tmr, uint32_t freq)
{

}
void hal_timer_setCallback(hal_timer_id_t tmr, void (*c)(void))
{

}


void delay_setBusyWaiting(bool en)
{
//  _delay_bw = en;
}

void delay_ms(uint16_t value)
{

}

void delay_us(uint16_t value)
{

}

/******************************************************************************/
static void handleInterrupt(hal_timer_t *tmrHAL)
{
  if(tmrHAL->flags.timeout == 1) {
  	tmrHAL->flags.overlap = 1;
  }
  tmrHAL->flags.timeout = 1;

  if(tmrHAL->callbacks.timeout != 0)
  	tmrHAL->callbacks.timeout();
}

