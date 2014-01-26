/*!
 * \file hal_clock.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_CLOCK_H
#define HAL_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_hwspecs.h"

/******************************************************************************
   ENUMS
 ******************************************************************************/

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct hal_clock_callbacks
{
  void (*changed)(uint64_t new_fosc);
} hal_clock_callbacks_t;

typedef struct hal_clock
{
  hal_clock_freq_t freq;
  uint32_t freq_value;
  hal_clock_callbacks_t callbacks;
} hal_clock_t;

/******************************************************************************
   DEFINES
 ******************************************************************************/

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern hal_clock_t _hal_clock;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void _hal_clock_init();
void hal_clock_init();
bool hal_clock_setFrequency(hal_clock_freq_t freq);
hal_clock_freq_t hal_clock_getFreqEnum(uint32_t value);
uint32_t hal_clock_getFreqValue(hal_clock_freq_t freq);
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* HAL_CLOCK_H */
