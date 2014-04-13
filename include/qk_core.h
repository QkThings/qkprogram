/*!
 * \file qk_core.h
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#ifndef QK_CORE_H
#define QK_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
   ENUMS
 ******************************************************************************/

/**
 * Clock Mode
 */
typedef enum qk_clock_mode
{
  QK_CLOCK_MODE_NORMAL = 0,
  QK_CLOCK_MODE_FASTER,
  QK_CLOCK_MODE_FAST,
  QK_CLOCK_MODE_SLOW,
  QK_CLOCK_MODE_SLOWER
} qk_clock_mode;

/**
 * Sampling Mode
 */
typedef enum qk_samp_mode
{
  QK_SAMPLING_MODE_SINGLE,
  QK_SAMPLING_MODE_CONTINUOUS,
  QK_SAMPLING_MODE_TRIGGERED
} qk_sampling_mode;

/**
 * Sampling Trigger Clock
 */
typedef enum qk_trigger_clock
{
  QK_SAMPLING_TRIGGER_CLOCK_1SEC, //!< QK_SAMPLING_TRIGGER_CLOCK_1SEC
  QK_SAMPLING_TRIGGER_CLOCK_10SEC,//!< QK_TRIGGER_CLOCK_10SEC
  QK_SAMPLING_TRIGGER_CLOCK_1MIN, //!< QK_SAMPLING_TRIGGER_CLOCK_1MIN
  QK_SAMPLING_TRIGGER_CLOCK_10MIN,//!< QK_SAMPLING_TRIGGER_CLOCK_10MIN
  QK_SAMPLING_TRIGGER_CLOCK_1HOUR //!< QK_TRIGGER_CLOCK_1HOUR
} qk_sampling_trigger_clock;


/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
bool qk_clock_set_mode(qk_clock_mode mode);
void qk_core_set_baudrate(uint32_t baud);

#if defined( QK_IS_DEVICE )
void qk_sampling_set_frequency(uint32_t sampFreq);
void qk_sampling_set_period(uint32_t usec);
void qk_sampling_set_mode(qk_sampling_mode mode);
void qk_sampling_set_trigger(qk_sampling_trigger_clock clock, uint8_t scaler);
void qk_sampling_set_N(uint32_t N);

#endif // QK_IS_DEVICE

#ifdef __cplusplus
}
#endif

#endif /* QK_CORE_H */
