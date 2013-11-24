/*!
 * \file hal_mcu.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_MCU_H
#define HAL_MCU_H

#include "em_chip.h"

static inline void hal_mcu_init()
{
  CHIP_Init();
}


#endif /* HAL_MCU_H */
