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
 * This file is part of qkprogram
 */

#ifndef QK_DEBUG_H
#define QK_DEBUG_H

/** \addtogroup QkDebug
 *  @{
 */

#include <stdint.h>


#if defined( QK_DEBUG )
  #include <stdio.h>
  #define PRINT(...) printf(__VA_ARGS__)
#else
  #define PRINT(...)
#endif

//#define QK_LOG_NO_OUTPUT
#define _QK_LOG_BUFSIZE  128

/**
 * Logging level
 */
typedef enum
{
  QK_LOG_LEVEL_DEBUG = (1<<0),  //!< QK_LOG_LEVEL_DEBUG
  QK_LOG_LEVEL_ERROR = (1<<1),  //!< QK_LOG_LEVEL_ERROR
  QK_LOG_LEVEL_WARNING = (1<<2),//!< QK_LOG_LEVEL_WARNING
  QK_LOG_LEVEL_INFO = (1<<3)    //!< QK_LOG_LEVEL_INFO
} QK_LOG_LEVEL;

typedef uint8_t qk_log_mask;

#ifndef QK_LOG_NO_OUTPUT
#define _LOG(level, ...) {\
		if((level && _log_mask) != 0){ \
			sprintf(_qk_log_str, __VA_ARGS__);\
			_qk_log_send();\
		}}
#else
#define _LOG(level, ...) {}
#endif


/** . */
#define QK_LOG_DEBUG(...)   _LOG(QK_LOG_LEVEL_DEBUG, __VA_ARGS__)
/** . */
#define QK_LOG_ERROR(...)   _LOG(QK_LOG_LEVEL_ERROR, __VA_ARGS__)
/** . */
#define QK_LOG_WARNING(...) _LOG(QK_LOG_LEVEL_WARNING, __VA_ARGS__)
/** . */
#define QK_LOG_INFO(...)    _LOG(QK_LOG_LEVEL_INFO, __VA_ARGS__)

extern qk_log_mask _log_mask;
extern char _qk_log_str[_QK_LOG_BUFSIZE];

/**
 * @brief .
 */
void qk_log_set_levels(qk_log_mask mask);

void _qk_log_send();

/** @}*/

#endif /* QK_DEBUG_H */
