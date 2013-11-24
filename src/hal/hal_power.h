/*!
 * \file hal_power.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef HAL_POWER_H_
#define HAL_POWER_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
   ENUMS
 ******************************************************************************/
typedef enum hal_battery_status
{
  HAL_BATTERY_STATUS_OK,
  HAL_BATTERY_STATUS_LOW,
  HAL_BATTERY_STATUS_VERYLOW
} hal_battery_status_t;

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct hal_power_info
{
  hal_battery_status_t batteryStatus;
} hal_power_info_t;

typedef struct hal_power_callbacks
{
  void (*batteryStatusChanged)(hal_battery_status_t new_status);
} hal_power_callbacks_t;


typedef struct hal_power
{
  hal_power_info_t      info;
  hal_power_callbacks_t callbacks;
} hal_power_t;

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern hal_power_t _hal_power;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void hal_power_init();
void hal_power_batteryCheck();
void hal_power_EM1(); // Energy mode 1 (only CPU is inactive)
void hal_power_EM2(); // Energy mode 2 (must wake up from UART RX interrupt)
void hal_power_EM3(); // Energy mode 3 (must wake up from RTC, if available)

#ifdef __cplusplus
}
#endif

#endif /* HAL_POWER_H_ */
