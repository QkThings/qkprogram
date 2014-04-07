/**
 * @file qk_debug.h
 *
 * This file is part of qkprogram
 */

#ifndef QK_DEBUG_H
#define QK_DEBUG_H

#include <stdint.h>

//#define QK_LOG_NO_OUTPUT

typedef enum
{
  QK_LOG_LEVEL_DEBUG = (1<<0),
  QK_LOG_LEVEL_ERROR = (1<<1),
  QK_LOG_LEVEL_WARNING = (1<<2),
  QK_LOG_LEVEL_INFO = (1<<3)
} QK_LOG_LEVEL;

typedef uint8_t qk_log_mask;

void qk_set_log_levels(qk_log_mask mask);

//#ifdef QK_LOG_NO_OUTPUT
//#define QK_LOG(...)
//#else

#define QK_LOG_DEBUG(...)   _QK_LOG(QK_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define QK_LOG_ERROR(...)   _QK_LOG(QK_LOG_LEVEL_ERROR, __VA_ARGS__)
#define QK_LOG_WARNING(...) _QK_LOG(QK_LOG_LEVEL_WARNING, __VA_ARGS__)
#define QK_LOG_INFO(...)    _QK_LOG(QK_LOG_LEVEL_INFO, __VA_ARGS__)

void _QK_LOG(QK_LOG_LEVEL level, char *text, ...);
//#endif


#endif /* QK_DEBUG_H */
