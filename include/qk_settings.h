/*!
 * \file qk_settings.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_SETTINGS_H
#define QK_SETTINGS_H

//TODO all settings/features should be defined in the makefile

/*****************************************************************************
 * BUILD TARGET
 *****************************************************************************/
//#define BUILD_GATEWAY
//#define BUILD_NETWORK
//#define BUILD_MODULE
//#define BUILD_DEVICE

/*****************************************************************************
 * FEATURES
 *****************************************************************************/
//#define _QK_FEAT_RTC_
//#define _QK_FEAT_EEPROM_
//#define _QK_FEAT_POWER_MANAGEMENT
//#define _QK_FEAT_CLOCK_SWITCHING_
//#define _QK_FEAT_FRAGMENTATION

/*****************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define _QK_DEFAULT_SAMPFREQ         (2)
#define _QK_MAX_FIRED_EVENTS         (8)
#define _QK_MAX_PENDING_EVENTS       (8)
#define _QK_EVENT_MAX_ARGS           (8)
#define _QK_ACTION_MAX_ARGS          (8)
#define _QK_PACKET_DATBUF_SIZE       (64)
#define _QK_BOARD_NAME_SIZE          (20)
#define _QK_LABEL_SIZE               (20)
//#define QK_EVENT_NAME_SIZE          (16)
//#define QK_CONFIG_NAME_SIZE         (10)
#define _QK_IDLE_TIMEOUT             (1)   // Idle timeout (s)
#define _QK_GOTOPD_TIMEOUT           (30)  // Go to power-down timeout (100's of ms)
#define _QK_WAITACK_TIMEOUT          (999) // Wait ACK timeout (ms)

#define _HAL_UART_RXBUF_SIZE         (32)

#define _HAL_UART_BAUD_DEFAULT_LOW   38400
#define _HAL_UART_BAUD_DEFAULT_HIGH  125000

/*****************************************************************************
 * DEBUG / DEVELOPMENT
 *****************************************************************************/
//#define       _QK_DEBUG_

#endif /* QK_SETTINGS_H */
