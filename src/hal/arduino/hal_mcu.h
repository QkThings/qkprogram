/*!
 * \file hal_mcu.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_MCU_H
#define HAL_MCU_H

#include <avr/interrupt.h>

static inline void hal_mcu_init()
{

}

static inline void hal_finalize()
{
  sei();
}

#endif /* HAL_MCU_H */
