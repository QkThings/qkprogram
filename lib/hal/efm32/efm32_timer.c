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

#include "../sys/qk_system.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_emu.h"

hal_timer_t _hal_timer[2];

#define CAL_Q 7

/******************************************************************************/
bool _delay_bw = false;

const uint32_t _1ms_scaler[] =
{
    14, // 14 Mhz (default)
    16, // 1 Mhz
    14, // 7 Mhz
    11, // 11 Mhz
    21, // 21 Mhz
    27, // 28 Mhz
};

const uint32_t _1ms_pre_mask[] =
{
    TIMER_CTRL_PRESC_DIV1024, // 14 Mhz (default)
    TIMER_CTRL_PRESC_DIV64,   // 1 Mhz
    TIMER_CTRL_PRESC_DIV512,  // 7 Mhz
    TIMER_CTRL_PRESC_DIV1024, // 11 Mhz
    TIMER_CTRL_PRESC_DIV1024, // 21 Mhz
    TIMER_CTRL_PRESC_DIV1024, // 28 Mhz
};

const int32_t _1ms_cal[] =
{
    42,   // 14 Mhz (default)
    96,   // 1 Mhz
    42,   // 7 Mhz
    42,   // 11 Mhz
    74,   // 21 Mhz
    -44,  // 28 Mhz
};

const uint32_t _1us_scaler[] =
{
    7,  // 14 Mhz (default)
    1,  // 1 Mhz
    7,  // 7 Mhz
    11, // 11 Mhz
    21, // 21 Mhz
    7,  // 28 Mhz
};

const uint32_t _1us_pre_mask[] =
{
    TIMER_CTRL_PRESC_DIV2,  // 14 Mhz (default)
    TIMER_CTRL_PRESC_DIV1,  // 1 Mhz
    TIMER_CTRL_PRESC_DIV1,  // 7 Mhz
    TIMER_CTRL_PRESC_DIV1,  // 11 Mhz
    TIMER_CTRL_PRESC_DIV1,  // 21 Mhz
    TIMER_CTRL_PRESC_DIV4,  // 28 Mhz
};
/******************************************************************************/
static void handleInterrupt(hal_timer_t *tmrHAL);
static void start(TIMER_TypeDef *timer);
static void stop(TIMER_TypeDef *timer);
static void reset(TIMER_TypeDef *timer);
static void setPeriod(TIMER_TypeDef *timer, uint32_t per, hal_timer_scale_t s);
static void setFrequency(TIMER_TypeDef *timer, uint32_t freq);
static uint32_t calcTop(uint32_t period, uint32_t scaler, int32_t cal);
/******************************************************************************/
#define QK_TIMER1 TIMER0
#define QK_TIMER2 TIMER1

#define QK_TIMER1_IRQn        TIMER0_IRQn
#define QK_TIMER2_IRQn        TIMER1_IRQn

#define QK_TIMER1_IRQHandler  TIMER0_IRQHandler
#define QK_TIMER2_IRQHandler  TIMER1_IRQHandler
/******************************************************************************/

void hal_timer_init()
{
  memset(&_hal_timer[HAL_TIMER_ID_1], 0, sizeof(hal_timer_t));
  memset(&_hal_timer[HAL_TIMER_ID_2], 0, sizeof(hal_timer_t));

  hal_timer_reinit();

  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_TIMER0;
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_TIMER1;

  QK_TIMER1->IEN |= TIMER_IF_OF;
  QK_TIMER2->IEN |= TIMER_IF_OF;

  NVIC_EnableIRQ(QK_TIMER1_IRQn);
  NVIC_EnableIRQ(QK_TIMER2_IRQn);
}

void hal_timer_reinit()
{

}
/******************************************************************************/
static hal_timer_t* getTimerHAL(hal_timer_id_t tmr)
{
	return &(_hal_timer[tmr]);
}
static TIMER_TypeDef* getTimerTypeDef(hal_timer_id_t tmr)
{
	switch(tmr)
	{
	case HAL_TIMER_ID_1: return QK_TIMER1;
	case HAL_TIMER_ID_2: return QK_TIMER2;
	}
	return 0;
}

void hal_timer_start(hal_timer_id_t tmr)
{
	start(getTimerTypeDef(tmr));
}
void hal_timer_stop(hal_timer_id_t tmr)
{
	stop(getTimerTypeDef(tmr));
}
void hal_timer_reset(hal_timer_id_t tmr)
{
	reset(getTimerTypeDef(tmr));
}
void hal_timer_setPeriod(hal_timer_id_t tmr, uint32_t per, hal_timer_scale_t s)
{
  setPeriod(getTimerTypeDef(tmr), per, s);
}
void hal_timer_setFrequency(hal_timer_id_t tmr, uint32_t freq)
{
  setFrequency(getTimerTypeDef(tmr), freq);
}
void hal_timer_setCallback(hal_timer_id_t tmr, void (*c)(void))
{
	getTimerHAL(tmr)->callbacks.timeout = c;
}

/******************************************************************************/
void QK_TIMER1_IRQHandler(void)
{
  hal_timer_t *tmrHAL = &_hal_timer[HAL_TIMER_ID_1];
  handleInterrupt(tmrHAL);

  /* Clear flag for TIMER0 overflow interrupt */
  QK_TIMER1->IFC |= TIMER_IF_OF;
}
#ifdef HAL_USE_TIMER2
void QK_TIMER2_IRQHandler(void)
{
  hal_timer_t *tmrHAL = &_hal_timer[HAL_TIMER_ID_2];
  handleInterrupt(tmrHAL);

  /* Clear flag for TIMER1 overflow interrupt */
  QK_TIMER2->IFC |= TIMER_IF_OF;
}
#endif /*HAL_USE_TIMER2*/
/******************************************************************************/
void delay_setBusyWaiting(bool en)
{
  _delay_bw = en;
}

void delay_ms(uint16_t value)
{
  bool running = ((QK_TIMER1->STATUS & TIMER_STATUS_RUNNING) == 1 ? true : false);
  hal_clock_freq_t freq = _hal_clock.freq;
  QK_TIMER1->CTRL = (QK_TIMER1->CTRL & ~_TIMER_CTRL_PRESC_MASK) | _1ms_pre_mask[freq];

  uint32_t top = calcTop(value, _1ms_scaler[freq], _1ms_cal[freq]);

  if(_delay_bw) {
    if(!running) {
    	QK_TIMER1->CMD = TIMER_CMD_START;
    }
    QK_TIMER1->CNT = 0;
    while(QK_TIMER1->CNT < top);
  }
  else {
  	QK_TIMER1->IEN &= ~(TIMER_IF_OF);
  	QK_TIMER1->TOP = top;
  	QK_TIMER1->CNT = 0;
    _hal_timer[HAL_TIMER_ID_1].flags.timeout = 0;
    QK_TIMER1->IEN |= TIMER_IF_OF;
    if(!running)
    	QK_TIMER1->CMD = TIMER_CMD_START;
    while(_hal_timer[HAL_TIMER_ID_1].flags.timeout == 0) {
    	EMU_EnterEM1();
    }
  }

  if(!running)
  	QK_TIMER1->CMD = TIMER_CMD_STOP;
}

void delay_us(uint16_t value)
{
  /*Note: value > 100*/
  hal_clock_freq_t freq = _hal_clock.freq;
  uint32_t calibrated_val = _1us_scaler[freq]*value-99; //FIXME
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= 1;
  uint32_t temp = DWT->CYCCNT + calibrated_val;
  while (DWT->CYCCNT < temp);
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

static void start(TIMER_TypeDef *timer)
{
  timer->CMD = TIMER_CMD_START;
}

static void stop(TIMER_TypeDef *timer)
{
  timer->CMD = TIMER_CMD_STOP;
}

static void reset(TIMER_TypeDef *timer)
{
  timer->CNT = 0;
}

/*static uint32_t calculateScaler(float timeUnit, uint32_t pre)
{
  return (uint32_t)(timeUnit*((float)_hal_clock.fosc)/(float)(pre));
}*/

static uint32_t calcTop(uint32_t period, uint32_t scaler, int32_t cal)
{
  return ((scaler*period)-((period*cal)>>CAL_Q));
}

static void setPeriod(TIMER_TypeDef *timer, uint32_t per, hal_timer_scale_t s)
{
  hal_clock_freq_t freq = _hal_clock.freq;
  switch(s)
  {
  case HAL_TIMER_SCALE_MSEC:
    timer->CTRL = (timer->CTRL & ~_TIMER_CTRL_PRESC_MASK) | _1ms_pre_mask[freq];
    timer->TOP = calcTop(per, _1ms_scaler[freq], _1ms_cal[freq]);
    break;
  case HAL_TIMER_SCALE_USEC:
    timer->CTRL = (timer->CTRL & ~_TIMER_CTRL_PRESC_MASK) | _1us_pre_mask[freq];
    timer->TOP = calcTop(per, _1us_scaler[freq], 0);;
    break;
  default: ;
  }
  timer->CNT = 0;
}

static void setFrequency(TIMER_TypeDef *timer, uint32_t freq)
{
  uint16_t period = 0;
  hal_timer_scale_t scale;
  if(freq >= 1 && freq <= 1000) {
      period = (uint16_t)(1000.0/(float)freq); // msec
      scale = HAL_TIMER_SCALE_MSEC;
  }
  else if(freq > 1000) {
      period = (uint16_t)(1000000.0/(float)freq); // usec
      scale = HAL_TIMER_SCALE_USEC;
  }

  if(period > 0)
    setPeriod(timer, period, scale);
}
