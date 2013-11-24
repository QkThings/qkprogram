/*!
 * \file hal_hwspecs.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_HWSPECS_H_
#define HAL_HWSPECS_H_

#include "efm32.h"

/* Available clock frequencies (obtained through external crystals/oscilators or internal
 * oscilators */
typedef enum hal_clock_freq {
  // Internal
  HAL_CLOCK_FREQ_14MHz = 0, // Default
  HAL_CLOCK_FREQ_1MHz,
  HAL_CLOCK_FREQ_7MHz,
  HAL_CLOCK_FREQ_11MHz,
  HAL_CLOCK_FREQ_21MHz,
  HAL_CLOCK_FREQ_28MHz,
  // External (note: change this acording to used crystal frequency */
  HAL_CLOCK_FREQ_32MHz
} hal_clock_freq_t;

#define HAL_CLOCK_FREQ_FASTER   HAL_CLOCK_FREQ_32MHz
#define HAL_CLOCK_FREQ_FAST     HAL_CLOCK_FREQ_28MHz
#define HAL_CLOCK_FREQ_NORMAL   HAL_CLOCK_FREQ_14MHz
#define HAL_CLOCK_FREQ_SLOW     HAL_CLOCK_FREQ_7MHz
#define HAL_CLOCK_FREQ_SLOWER   HAL_CLOCK_FREQ_1MHz

#endif /* HAL_HWSPECS_H_ */
