/*!
 * \file hal_gpio.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
   ENUMS
 ******************************************************************************/

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct hal_gpio_callbacks
{
  void (*inputChanged)(void);
} hal_gpio_callbacks_t;

typedef volatile struct hal_gpio_flags
{
  unsigned inputChanged : 1;
} hal_gpio_flags_t;

typedef struct hal_gpio
{
  hal_gpio_callbacks_t callbacks;
  hal_gpio_flags_t flags;
} hal_gpio_t;

/******************************************************************************
   DEFINES
 ******************************************************************************/

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern hal_gpio_t _hal_gpio;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void _hal_gpio_init();
void hal_gpio_init();

bool hal_getPB();
bool hal_getDET();
//bool _getLED();
//void _setLED(bool on);
//bool _toggleLED();

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_H */
