/*!
 * \file hal.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_H
#define HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void hal_setLED(bool state);
bool hal_toggleLED();
void hal_blinkLED(uint8_t n, uint16_t msec);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* HAL_H */
