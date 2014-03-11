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
  QK_SAMP_SINGLE,
  QK_SAMP_CONTINUOUS,
  QK_SAMP_TRIGGERED
} qk_samp_mode;

/**
 * Sampling Trigger Clock
 */
typedef enum qk_trigger_clock
{
  QK_TRIGGER_CLOCK_1SEC, //!< QK_TRIGGER_CLOCK_1SEC
  QK_TRIGGER_CLOCK_10SEC,//!< QK_TRIGGER_CLOCK_10SEC
  QK_TRIGGER_CLOCK_1MIN, //!< QK_TRIGGER_CLOCK_1MIN
  QK_TRIGGER_CLOCK_10MIN,//!< QK_TRIGGER_CLOCK_10MIN
  QK_TRIGGER_CLOCK_1HOUR //!< QK_TRIGGER_CLOCK_1HOUR
} qk_trigger_clock;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
bool qk_setClockMode(qk_clock_mode mode);

#ifdef _QK_FEAT_SERIAL_
void qk_setBaudRate(uint32_t baud);
#endif

#if defined( QK_IS_DEVICE )
/**
 * @brief Sets sampling frequency
 * @param sampFreq  sampling frequency in Hertz, must be greater than zero
 */
void qk_setSamplingFrequency(uint32_t sampFreq);

/**
 * @brief Sets sampling period
 * @param usec  sampling period in microseconds
 */
void qk_setSamplingPeriod(uint32_t usec);
void qk_setSamplingMode(qk_samp_mode mode);
void qk_setTriggerClock(qk_trigger_clock clock);
void qk_setTriggerScaler(uint8_t scaler);
void qk_setNumberOfSamples(uint32_t N);

#endif // QK_IS_DEVICE

#ifdef __cplusplus
}
#endif

#endif /* QK_CORE_H */
