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

typedef enum qk_state
{
  QK_STATE_SLEEP,
  QK_STATE_IDLE,
  QK_STATE_START,
  QK_STATE_RUNNING,
  QK_STATE_STANDBY,
  QK_STATE_STOP
} qk_state;

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
   STRUCTS
 ******************************************************************************/
typedef struct qk_info
{
  uint32_t baudRate;
} qk_info;


typedef struct qk_callbacks
{
  void (*boardAttached)(void);
  void (*boardRemoved)(void);
  void (*inputChanged)(void);
  void (*clockChanged)(uint32_t new_value);
} qk_callbacks;

typedef volatile struct qk_flags
{
  union
  {
    uint64_t reg;
    struct
    {
      uint16_t reg_status;
      uint16_t reg_settings;
      uint16_t _rsvd_;
      uint8_t reg_internal;
      uint8_t reg_sleep;
    };
  };
} qk_flags;

typedef struct qk_sampling
{
  //uint16_t  lastPer;            //! Last valid sampling period
  uint32_t  N;
  uint32_t  period;
  uint32_t  frequency;
  uint8_t   mode;
  uint8_t   triggerClock;
  uint8_t   triggerScaler;
} qk_sampling;

typedef struct qk_core
{
  volatile qk_state currentState;
  volatile qk_state changeToState;
  qk_info       info;
  qk_clock_mode clockMode;
  qk_callbacks  callbacks;
  qk_flags      flags;
#if defined(QK_IS_DEVICE)
  qk_sampling   sampling;
#endif
} qk_core;



/******************************************************************************
   DEFINES
 ******************************************************************************/
#define QK_FLAGMASK_STATUS_RUNNING          (1<<0)
#define QK_FLAGMASK_STATUS_DET              (1<<1)
#define QK_FLAGMASK_SETTINGS_AUTOSAMP       (1<<0)
#define QK_FLAGMASK_SETTINGS_TIMESTAMP      (1<<1)
#define QK_FLAGMASK_SETTINGS_EVTNOTIF       (1<<2)
#define QK_FLAGMASK_SETTINGS_STATNOTIF      (1<<3)
#define QK_FLAGMASK_INTERNAL_RQSTATECHANGE  (1<<0)
#define QK_FLAGMASK_SLEEP_EVENTS_PENDING    (1<<0)

#define QK_DEFAULT_FLAGS  0

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
extern qk_core _qk_core;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_core_init();
bool qk_setClockMode(qk_clock_mode mode);
void qk_run();
void qk_loop();

void _qk_requestStateChange(qk_state state);
void _qk_handleStateChange();

#ifdef _QK_FEAT_SERIAL_
void qk_setBaudRate(uint32_t baud);
#endif

#if defined( QK_IS_DEVICE )
/**
 * @brief Sets sampling frequency
 * @param sampFreq  sampling frequency in Hertz, must be greater than zero
 */
void qk_setSamplingFrequency(uint32_t sampFreq);
#if defined( QK_IS_DEVICE )
/**
 * @brief Sets sampling period
 * @param usec  sampling period in microseconds
 */
void qk_setSamplingPeriod(uint32_t usec);
#endif

/******************************************************************************/
static inline
bool _qk_canSleep()
{
  return (_qk_core.flags.reg_sleep == 0);
}

#if defined( QK_IS_DEVICE )
static inline void qk_setSamplingMode(qk_samp_mode mode)
{
  _qk_core.sampling.mode = mode;
}
static inline void qk_setTriggerClock(qk_trigger_clock clock)
{
  _qk_core.sampling.triggerClock = clock;
}
static inline void qk_setTriggerScaler(uint8_t scaler)
{
  _qk_core.sampling.triggerScaler = scaler;
}
static inline void qk_setNumberOfSamples(uint32_t N)
{
  _qk_core.sampling.N = N;
}
#endif


#ifdef __cplusplus
}
#endif

#endif /* QK_CORE_H */
