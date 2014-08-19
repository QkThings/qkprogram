/*!
 * \file hal_gpio.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"
#include "em_gpio.h"

#if defined( EFM32G_STK )
#define LED_PORT  gpioPortC
#define LED_PIN   3
#define PB_PORT   gpioPortB
#define PB_PIN    9
#define DET_PORT  gpioPortC
#define DET_PIN   12
#elif defined( EFM32G_OLIMEX )
#define LED_PORT  gpioPortA
#define LED_PIN   0
#define PB_PORT   gpioPortA
#define PB_PIN    1
#define DET_PORT  gpioPortA
#define DET_PIN   2
#elif defined( EFM32TG_DEVBOARD )
#define LED_PORT  gpioPortA
#define LED_PIN   10
#define PB_PORT   gpioPortC
#define PB_PIN    0
#define DET_PORT  gpioPortC
#define DET_PIN   1
#else
#error "EFM32 board not defined"
#endif

hal_gpio_t _hal_gpio;

static void handleInputChanged();

void hal_gpio_init()
{
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;


  /* Setting MODEn to 0b0000 disables the pin, reducing power consumption
   * to a minimum. */

  /* Configure LED_PORT pin LED_PIN (User LED) as push/pull outputs */
  GPIO_PinModeSet(LED_PORT,         /* Port */
                  LED_PIN,          /* Pin */
                  gpioModePushPull, /* Mode */
                  0 );              /* Output value */

#if 1
  /* Set PB0_PORT PB0_PIN (Push button 0) as input */
  GPIO_PinModeSet(PB_PORT,        /* Port */
                  PB_PIN,                /* Pin */
                  gpioModeInputPull,    /* Mode */
                  1 );              /* Output value */

  GPIO_PinModeSet(DET_PORT,        /* Port */
                  DET_PIN,                /* Pin */
                  gpioModeInputPull,    /* Mode */
                  1 );              /* Output value */

  GPIO_IntConfig(PB_PORT, PB_PIN, false, true, true);
  GPIO_IntConfig(DET_PORT, DET_PIN, true, true, true);
#endif

  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void GPIO_EVEN_IRQHandler(void) // Rise
{
  hal_toggleLED();
  handleInputChanged();
  GPIO_IntClear(0xFFFF); // Clear all GPIO IF
}

void GPIO_ODD_IRQHandler(void) // Fall
{
  hal_toggleLED();
  handleInputChanged();
  GPIO_IntClear(0xFFFF); // Clear all GPIO IF
}

bool hal_getPB()
{
  return GPIO_PinInGet(PB_PORT, PB_PIN);
}

bool hal_getDET()
{
  return GPIO_PinInGet(DET_PORT, DET_PIN);
}

bool hal_getLED()
{
  return GPIO_PinInGet(LED_PORT, LED_PIN);
}

void hal_setLED(bool on)
{
  if(on)
    GPIO_PinOutClear(LED_PORT, LED_PIN);
  else
    GPIO_PinOutSet(LED_PORT, LED_PIN);
}

bool hal_toggleLED()
{
  GPIO_PinOutToggle(LED_PORT, LED_PIN);
  return (GPIO_PinOutGet(LED_PORT, LED_PIN) == 1 ? true : false);
}

static void handleInputChanged()
{
  if(_hal_gpio.callbacks.inputChanged != 0) {
    _hal_gpio.callbacks.inputChanged();
  }
  _hal_gpio.flags.inputChanged = 1;
}
