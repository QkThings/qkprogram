/*!
 * \file hal_hwspecs.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_HWSPECS_H_
#define HAL_HWSPECS_H_

/* Available clock frequencies (obtained through external crystals/oscilators or internal
 * oscilators */
typedef enum hal_clock_freq {
  HAL_CLOCK_FREQ_16MHz = 0, // Default
} hal_clock_freq_t;

#define HAL_CLOCK_FREQ_FASTER   HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_FAST     HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_NORMAL   HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_SLOW     HAL_CLOCK_FREQ_16MHz
#define HAL_CLOCK_FREQ_SLOWER   HAL_CLOCK_FREQ_16MHz

#endif /* HAL_HWSPECS_H_ */
