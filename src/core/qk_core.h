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

typedef enum qk_state_t
{
  QK_STATE_SLEEP,
  QK_STATE_IDLE,
  QK_STATE_START,
  QK_STATE_RUNNING,
  QK_STATE_STANDBY,
  QK_STATE_STOP
} qk_state_t;

typedef enum qk_clock_mode_t
{
  QK_CLOCK_MODE_NORMAL = 0,
  QK_CLOCK_MODE_FASTER,
  QK_CLOCK_MODE_FAST,
  QK_CLOCK_MODE_SLOW,
  QK_CLOCK_MODE_SLOWER
} qk_clock_mode_t;

typedef enum qk_samp_mode_t
{
  QK_SAMP_SINGLE,
  QK_SAMP_CONTINUOUS,
  QK_SAMP_TRIGGERED
} qk_samp_mode_t;

typedef enum qk_trigger_clock_t
{
  QK_TRIGGER_CLOCK_1SEC,
  QK_TRIGGER_CLOCK_10SEC,
  QK_TRIGGER_CLOCK_1MIN,
  QK_TRIGGER_CLOCK_10MIN,
  QK_TRIGGER_CLOCK_1HOUR
} qk_trigger_clock_t;

/******************************************************************************
   STRUCTS
 ******************************************************************************/
typedef struct qk_info_t
{
  uint32_t baudRate;
} qk_info_t;

typedef struct qk_callbacks_t
{
  void (*boardAttached)(void);
  void (*boardRemoved)(void);
  void (*inputChanged)(void);
  void (*clockChanged)(uint32_t new_value);
} qk_callbacks_t;

typedef volatile struct qk_flags_t
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
} qk_flags_t;

typedef struct qk_sampling_t
{
  //uint16_t  lastPer;            //! Last valid sampling period
  uint32_t  N;                  //! Number of samples
  uint32_t  period;             //! Current sampling period [us]
  uint32_t  frequency;          //! Current sampling frequency [Hz]
  uint8_t   mode;               //! Sampling mode
  uint8_t   triggerClock;       //! Trigger clock
  uint8_t   triggerScaler;      //! Trigger scaler
} qk_sampling_t;

typedef struct qk_t
{
  volatile qk_state_t currentState;
  volatile qk_state_t changeToState;
  qk_info_t       info;
  qk_clock_mode_t clockMode;
  qk_callbacks_t  callbacks;
  qk_flags_t      flags;
#if defined(QK_IS_DEVICE)
  qk_sampling_t   sampling;
#endif
} qk_t;



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
extern qk_t _qk;

/******************************************************************************
   PROTOTYPES
 ******************************************************************************/
void qk_core_init();
bool qk_setClockMode(qk_clock_mode_t mode);
void qk_run();
void qk_loop();

void _qk_requestStateChange(qk_state_t state);
void _qk_handleStateChange();

#ifdef _QK_FEAT_SERIAL_
void qk_setBaudRate(uint32_t baud);
#endif

#if defined( QK_IS_DEVICE )
void qk_setSamplingFrequency(uint32_t sampFreq);
void qk_setSamplingPeriod(uint32_t usec);
#endif

/******************************************************************************/
static inline
bool _qk_canSleep()
{
  return (_qk.flags.reg_sleep == 0);
}

#if defined( QK_IS_DEVICE )
static inline void qk_setSamplingMode(qk_samp_mode_t mode)
{
  _qk.sampling.mode = mode;
}
static inline void qk_setTriggerClock(qk_trigger_clock_t clock)
{
  _qk.sampling.triggerClock = clock;
}
static inline void qk_setTriggerScaler(uint8_t scaler)
{
  _qk.sampling.triggerScaler = scaler;
}
static inline void qk_setNumberOfSamples(uint32_t N)
{
  _qk.sampling.N = N;
}
#endif


#ifdef __cplusplus
}
#endif

#endif /* QK_CORE_H */
