/*!
 * \file hal_clock.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include "em_cmu.h"
#include "hal_hwspecs.h"

hal_clock_t _hal_clock;

void hal_clock_init()
{
  /* HFRCO is chosen by hardware as the clock source for HFCLK when
   * the device starts up (e.g. after reset and after waking up from
   * EM2 and EM3. After reset, the HFRCO frequency is 14 MHz.*/
  _hal_clock.freq = HAL_CLOCK_FREQ_14MHz;
  _hal_clock.freq_value = 14000000UL;

  /* Output clock */
  //CMU->CTRL |= CMU_CTRL_CLKOUTSEL0_HFRCO;

  //hal_clock_setFrequency(HAL_CLOCK_FREQ_28MHz);
}

bool hal_clock_setFrequency(hal_clock_freq_t freq)
{
  uint32_t new_value = hal_clock_getFreqValue(freq);
  uint32_t cal_mask;

  if(_hal_clock.freq_value == new_value)
    return false;

  switch(freq)
  {
    case HAL_CLOCK_FREQ_1MHz:
      cal_mask = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND1_MASK) >> _DEVINFO_HFRCOCAL0_BAND1_SHIFT;
      CMU->HFRCOCTRL = CMU_HFRCOCTRL_BAND_1MHZ | cal_mask;
      break;
    case HAL_CLOCK_FREQ_7MHz:
      cal_mask = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND7_MASK) >> _DEVINFO_HFRCOCAL0_BAND7_SHIFT;
      CMU->HFRCOCTRL = CMU_HFRCOCTRL_BAND_7MHZ | cal_mask;
      break;
    case HAL_CLOCK_FREQ_11MHz:
      cal_mask = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND11_MASK) >> _DEVINFO_HFRCOCAL0_BAND11_SHIFT;
      CMU->HFRCOCTRL = CMU_HFRCOCTRL_BAND_11MHZ | cal_mask;
      break;
    case HAL_CLOCK_FREQ_14MHz:
      cal_mask = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND14_MASK) >> _DEVINFO_HFRCOCAL0_BAND14_SHIFT;
      CMU->HFRCOCTRL = CMU_HFRCOCTRL_BAND_14MHZ | cal_mask;
      break;
    case HAL_CLOCK_FREQ_21MHz:
      cal_mask = (DEVINFO->HFRCOCAL1 & _DEVINFO_HFRCOCAL1_BAND21_MASK) >> _DEVINFO_HFRCOCAL1_BAND21_SHIFT;
      CMU->HFRCOCTRL = CMU_HFRCOCTRL_BAND_21MHZ | cal_mask;
      break;
    case HAL_CLOCK_FREQ_28MHz:
      cal_mask = (DEVINFO->HFRCOCAL1 & _DEVINFO_HFRCOCAL1_BAND28_MASK) >> _DEVINFO_HFRCOCAL1_BAND28_SHIFT;
      CMU->HFRCOCTRL = CMU_HFRCOCTRL_BAND_28MHZ | cal_mask;
      break;
    case HAL_CLOCK_FREQ_32MHz:
      //TODO
      //CMU->CMD = CMU_CMD_HFCLKSEL_HFXO;
      //break;
    default: return false;
  }

  _hal_clock.freq = freq;
  _hal_clock.freq_value = new_value;
  if(_hal_clock.callbacks.changed != 0) {
    _hal_clock.callbacks.changed(new_value);
  }

  return true;
}

hal_clock_freq_t hal_clock_getFreqEnum(uint32_t fosc)
{
  switch(fosc)
  {
  case 1000000UL: return HAL_CLOCK_FREQ_1MHz;
  case 7000000UL: return HAL_CLOCK_FREQ_7MHz;
  case 11000000UL: return HAL_CLOCK_FREQ_11MHz;
  case 14000000UL: return HAL_CLOCK_FREQ_14MHz;
  case 21000000UL: return HAL_CLOCK_FREQ_21MHz;
  case 28000000UL: return HAL_CLOCK_FREQ_28MHz;
  case 32000000UL: return HAL_CLOCK_FREQ_32MHz;
  default: return 0;
  }
}
uint32_t hal_clock_getFreqValue(hal_clock_freq_t freq)
{
  switch(freq)
  {
  case HAL_CLOCK_FREQ_1MHz: return 1000000UL;
  case HAL_CLOCK_FREQ_7MHz: return 7000000UL;
  case HAL_CLOCK_FREQ_11MHz: return 11000000UL;
  case HAL_CLOCK_FREQ_14MHz: return 14000000UL;
  case HAL_CLOCK_FREQ_21MHz: return 21000000UL;
  case HAL_CLOCK_FREQ_28MHz: return 28000000UL;
  case HAL_CLOCK_FREQ_32MHz: return 32000000UL;
  default: return 0;
  }
}
