/*
 * QkThings LICENSE
 * The open source framework and modular platform for smart devices.
 * Copyright (C) 2014 <http://qkthings.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
//#define _QK_DEFAULT_SAMPFREQ         (2)
//#define _QK_MAX_FIRED_EVENTS         (8)
//#define _QK_MAX_PENDING_EVENTS       (8)
//#define _QK_EVENT_MAX_ARGS           (8)
//#define _QK_ACTION_MAX_ARGS          (8)
//#define _QK_PACKET_DATBUF_SIZE       (64)
//#define _QK_BOARD_NAME_SIZE          (20)
//#define _QK_LABEL_SIZE               (20)
////#define QK_EVENT_NAME_SIZE          (16)
////#define QK_CONFIG_NAME_SIZE         (10)
//#define _QK_IDLE_TIMEOUT             (1)   // Idle timeout (s)
//#define _QK_GOTOPD_TIMEOUT           (30)  // Go to power-down timeout (100's of ms)
//#define _QK_WAITACK_TIMEOUT          (999) // Wait ACK timeout (ms)
//
//#define _HAL_UART_RXBUF_SIZE         (32)
//
//#define _HAL_UART_BAUD_DEFAULT_LOW   38400
//#define _HAL_UART_BAUD_DEFAULT_HIGH  125000


#ifndef _QK_EVENT_MAX_PENDING
#define _QK_EVENT_MAX_PENDING       (8)
#endif

#ifndef _QK_EVENT_MAX_ARGS
#define _QK_EVENT_MAX_ARGS           (8)
#endif

#ifndef _QK_ACTION_MAX_ARGS
#define _QK_ACTION_MAX_ARGS          (8)
#endif

#ifndef _QK_PACKET_DATBUF_SIZE
#define _QK_PACKET_PAYLOAD_SIZE       (64)
#endif

#ifndef _QK_BOARD_NAME_SIZE
#define _QK_BOARD_NAME_SIZE          (20)
#endif

#ifndef _QK_LABEL_SIZE
#define _QK_LABEL_SIZE               (20)
#endif

#ifndef _HAL_UART_RXBUF_SIZE
#define _HAL_UART_RXBUF_SIZE         (32)
#endif

#define _QK_DEFAULT_SAMPFREQ        (2)
#define _QK_EVENT_MAX_FIRED         (8)
#define _QK_IDLE_TIMEOUT             (1)   // Idle timeout (s)
#define _QK_GOTOPD_TIMEOUT           (30)  // Go to power-down timeout (100's of ms)
#define _QK_WAITACK_TIMEOUT          (999) // Wait ACK timeout (ms)

#define _HAL_UART_BAUD_DEFAULT_LOW   38400
#define _HAL_UART_BAUD_DEFAULT_HIGH  125000

// Settings
// _QK_HAL_UART_POLLING

/*****************************************************************************
 * DEBUG / DEVELOPMENT
 *****************************************************************************/
//#define       _QK_DEBUG_

#endif /* QK_SETTINGS_H */
