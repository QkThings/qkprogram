/*!
 * \file hal_timer.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

hal_timer_t _hal_timer[2];

#define _TOP 16
#define _PRE 1024

/******************************************************************************/
bool _delay_bw = false;

void hal_timer_init()
{
  hal_timer_setPeriod(0, 256, 0);

  TCCR1B |= (1 << WGM12);  // (mode 4)
  TIMSK1 |= (1 << OCIE1A); // Set compare match to TIMER1_COMPA_vect

  #if _PRE == 1
    TCCR1B |= (1 << CS10);                                // Clock
  #elif _PRE == 8
    TCCR1B |= (1 << CS11);                                // Clock/8
  #elif _PRE == 64
    TCCR1B |= (1 << CS11) | (1 << CS10);                  // Clock/64
  #elif _PRE == 256
    TCCR1B |= (1 << CS12);                                // Clock/256
  #elif _PRE == 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);                  // Clock/1024
  #else
    TCCR1B |= (1 << CS12) | (1 << CS11) | (1 << CS10);    // External Clock buddy
  #endif
    sei();


    hal_timer_start(0);
}

void hal_timer_reinit()
{

}
void hal_timer_start(hal_timer_id_t tmr)
{
  PRR &= ~(1<<PRTIM1);
}
void hal_timer_stop(hal_timer_id_t tmr)
{
  PRR |= (1<<PRTIM1);
}
void hal_timer_reset(hal_timer_id_t tmr)
{
  TCNT1 = 0;
}
void hal_timer_setPeriod(hal_timer_id_t tmr, uint32_t per, hal_timer_scale_t s)
{
  //OCR1A = _TOP * per;   //1/fosc * pre * top = 1ms
  OCR1A = _TOP * 1000;   //1/fosc * pre * top = 1ms
}
void hal_timer_setFrequency(hal_timer_id_t tmr, uint32_t freq)
{
  OCR1A = _TOP * 1.0/((float)freq);   //1/fosc * pre * top = 1ms
}
void hal_timer_setCallback(hal_timer_id_t tmr, void (*c)(void))
{
  _hal_timer[tmr].callbacks.timeout = c;
}

/******************************************************************************/
void delay_setBusyWaiting(bool en)
{
  _delay_bw = en;
}

void delay_ms(uint16_t value)
{
  _delay_ms(value);
}

void delay_us(uint16_t value)
{

}

// ----
ISR(TIMER1_COMPA_vect)
{
  _toggleLED();
  hal_uart_writeByte(0, '*');
  return;

  hal_timer_t *tmrHAL = &(_hal_timer[HAL_TIMER_ID_2]);
  if(tmrHAL->flags.timeout == 1)
    tmrHAL->flags.overlap = 1;
  tmrHAL->flags.timeout = 1;

  if(tmrHAL->callbacks.timeout != 0)
    tmrHAL->callbacks.timeout();
}
