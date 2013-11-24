/*!
 * \file qk_core.c
 *
 * \author mribeiro
 *  
 * This file is part of QkProgram
 */

#include "../sys/qk_system.h"

//#include "em_emu.h"

qk_t _qk;
/******************************************************************************/
static void handleBoardDetection();
static void handleInputChanged();
/******************************************************************************/

void qk_core_init()
{
  memset(&_qk, 0, sizeof(qk_t));
  _qk.currentState = QK_STATE_IDLE;
  _qk.info.baudRate = HAL_UART_BAUD_DEFAULT_LOW;
#if defined( QK_IS_DEVICE )
  _qk.sampling.frequency = 0; // invalid
  _qk.sampling.mode = QK_SAMP_CONTINUOUS;
  _qk.sampling.triggerClock = QK_TRIGGER_CLOCK_10SEC;
  _qk.sampling.triggerScaler = 1;
  _qk.sampling.N = 10;

  qk_setSamplingFrequency(QK_DEFAULT_SAMPFREQ);
#endif

#if defined( _QK_FEAT_EEPROM_ )
  qk_restore();
#endif

  handleBoardDetection();
  handleInputChanged();
}

bool qk_setClockMode(qk_clock_mode_t mode)
{
  bool changed = false;
  switch(mode)
  {
  case QK_CLOCK_MODE_NORMAL:
    changed = hal_clock_setFrequency(HAL_CLOCK_FREQ_NORMAL);
    break;
  case QK_CLOCK_MODE_FASTER:
    changed = hal_clock_setFrequency(HAL_CLOCK_FREQ_FASTER);
    break;
  case QK_CLOCK_MODE_FAST:
    changed = hal_clock_setFrequency(HAL_CLOCK_FREQ_FAST);
    break;
  case QK_CLOCK_MODE_SLOW:
    changed = hal_clock_setFrequency(HAL_CLOCK_FREQ_SLOW);
    break;
  case QK_CLOCK_MODE_SLOWER:
    changed = hal_clock_setFrequency(HAL_CLOCK_FREQ_SLOWER);
    break;
  default: ;
  }
  if(changed) {
    _qk.clockMode = mode;
  }
  return changed;
}

void qk_run()
{
  uint32_t i, count;
  /*************************************
   * PROCESSING
   ************************************/

  if(_hal_gpio.flags.inputChanged == 1) {
    handleInputChanged();
    _hal_gpio.flags.inputChanged = 0;
  }

  //TODO Status notifications
  //TODO Events

  for(i = 0; i < QK_COMM_STRUCT_COUNT; i++)
  {
    if(_qk_comm[i].callbacks.processBytes != 0)
      _qk_comm[i].callbacks.processBytes();

    if(_qk_comm[i].flags.reg & QK_COMM_FLAGMASK_NEWPACKET)
    {
      if(_qk_comm[i].callbacks.processPacket != 0)
        _qk_comm[i].callbacks.processPacket();

      _qk_comm[i].flags.reg &= ~QK_COMM_FLAGMASK_NEWPACKET;
    }
  }

#if defined( QK_IS_DEVICE )
  count = _qk_maxFiredEvents();
  while(!qk_cb_isEmpty(&_pendingEvents) && count--)
  {
    qk_event_t firedEvent;
    qk_cb_read(&_pendingEvents, &firedEvent);
    _qk_comm_sendEvent(&firedEvent, _comm_board);
  }
#endif


  _qk_handleStateChange();

  /*************************************
   * STATE MACHINE
   ************************************/
  switch(_qk.currentState)
  {
  case QK_STATE_SLEEP:
    break;
  case QK_STATE_IDLE:
    hal_timer_stop(HAL_TIMER_ID_2);
    break;
#if defined(QK_IS_DEVICE)
  case QK_STATE_START:
    if(_qk_device->callbacks.start != 0)
      _qk_device->callbacks.start();
    hal_timer_reset(HAL_TIMER_ID_2);
    hal_timer_start(HAL_TIMER_ID_2);
    _qk.currentState = QK_STATE_RUNNING;
    break;
  case QK_STATE_RUNNING:
    if(_hal_timer_2->flags.timeout == 1)
    {
      if(_qk_device->callbacks.sample != 0)
        _qk_device->callbacks.sample();
      _qk_comm_sendCode(QK_PACKET_CODE_DATA, _comm_board);
      _hal_timer_2->flags.timeout = 0;
    }
    break;
  case QK_STATE_STANDBY:
    break;
  case QK_STATE_STOP:
    if(_qk_device->callbacks.stop != 0)
      _qk_device->callbacks.stop();
    _qk.currentState = QK_STATE_IDLE;
    break;
#endif
  default:
    _qk.currentState = QK_STATE_IDLE;
  }

  /*************************************
   * POWER MANAGEMENT
   ************************************/
#ifdef _QK_FEAT_POWER_MANAGEMENT_
  hal_power_EM1();
#endif /*_QK_FEAT_POWER_MANAGEMENT_*/
}

void qk_loop()
{
  while(1) { qk_run(); }
}

void _qk_requestStateChange(qk_state_t state)
{
  _qk.changeToState = state;
  _qk.flags.reg_internal |= QK_FLAGMASK_INTERNAL_RQSTATECHANGE;
}
void _qk_handleStateChange()
{
  if(_qk.flags.reg_internal & QK_FLAGMASK_INTERNAL_RQSTATECHANGE)
  {
    _qk.currentState = _qk.changeToState;
    _qk.flags.reg_internal &= ~QK_FLAGMASK_INTERNAL_RQSTATECHANGE;
  }
}

void qk_setBaudRate(uint32_t baud)
{
  hal_uart_setBaudRate(HAL_UART_ID_1, baud);
  _qk.info.baudRate = baud;
}

#ifdef QK_IS_DEVICE
void qk_setSamplingFrequency(uint32_t sampFreq)
{
  if(sampFreq == 0 || sampFreq == _qk.sampling.frequency)
  {
    return;
  }
  hal_timer_setFrequency(HAL_TIMER_ID_2, sampFreq);
  _qk.sampling.frequency = sampFreq;
  _qk.sampling.period = (uint32_t)(1000000.0/(float)sampFreq); // usec
}
void qk_setSamplingPeriod(uint32_t usec)
{
  uint32_t msec;
  if(usec >= 1000) {
    msec = usec*1000;
    hal_timer_setPeriod(HAL_TIMER_ID_2, msec, HAL_TIMER_SCALE_MSEC);
  }
  else {
    hal_timer_setPeriod(HAL_TIMER_ID_2, usec, HAL_TIMER_SCALE_USEC);
  }
  _qk.sampling.period = usec;
  _qk.sampling.frequency = (uint32_t)(1000000.0/(float)usec);

}
#endif /*QK_IS_DEVICE*/


static void handleBoardDetection()
{
  bool detected = !_getDET(); // DET pin is pulled-up

  if(flag(_qk.flags.reg_status, QK_FLAGMASK_STATUS_DET) == detected) {
    return;
  }

  if(detected)
  {
    _blinkLED(2, 50);
    _qk.flags.reg_status |= QK_FLAGMASK_STATUS_DET;

    hal_uart_setBaudRate(HAL_UART_ID_1, _qk.info.baudRate);

    if(_qk.callbacks.boardAttached != 0) {
      _qk.callbacks.boardAttached();
    }

#if defined( QK_IS_DEVICE )
//start
#endif
  }
  else
  {
    _blinkLED(1, 50);
    _qk.flags.reg_status &= ~QK_FLAGMASK_STATUS_DET;

    hal_uart_setBaudRate(HAL_UART_ID_1, HAL_UART_BAUD_DEFAULT_LOW);

    if(_qk.callbacks.boardRemoved != 0)
      _qk.callbacks.boardRemoved();

#if defined( QK_IS_DEVICE )
//stop
#endif
  }
}

static void handleInputChanged()
{
  handleBoardDetection();

#if defined( QK_IS_MODULE )
  uint8_t count;
  bool buttonPressed = !_getPB(); // PB pin is pulled-up

  if(buttonPressed) {
    delay_ms(250);
    if(_getPB() == 1) { // quick
      //if(_qk.flags.reg & QK_FLAGMASK_DET) {
        //TODO startDevice
      //}
      //else {
        //TODO stopDevice
      //}
    }
    else { // hold
      for(count = 0; count < QK_GOTOPD_TIMEOUT; count++) {
        delay_ms(100);
        if(_getPB()) break;
      }
      if(count == QK_GOTOPD_TIMEOUT) {
        _blinkLED(1, 100);
        while(_getPB() == 0) {
          delay_ms(100);
        }
        //TODO goto power down (deep sleep)
      }
    }
  }
#endif /*QK_IS_MODULE*/

  if(_qk.callbacks.inputChanged != 0) {
    _qk.callbacks.inputChanged();
  }
}
