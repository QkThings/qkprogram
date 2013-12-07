#include <qk_program.h>
#include <qk_debug.h>

#define CFG_COUNT 6
#define DAT_COUNT 3
#define EVT_COUNT 1

#define FLOAT //datatype

enum
{
  CFG_TEST_BOOL,
  CFG_TEST_DATETIME,
  CFG_TEST_TIME,
  CFG_TEST_INT,
  CFG_TEST_HEX,
  CFG_TEST_FLOAT,
};

enum
{
  DAT_TEST_0,
  DAT_TEST_1,
  DAT_TEST_2,
};

enum
{
  EVT_SAMPLE
};

qk_config cfg_buf[CFG_COUNT];

#if defined( QK_IS_DEVICE )
qk_data dat_buf[DAT_COUNT];
qk_event evt_buf[EVT_COUNT];
#endif

void test_init()
{

}

void test_config()
{
#if defined( QK_IS_DEVICE )
  uint8_t datCount = qk_configValueI(CFG_TEST_INT)+1;

  if(datCount > DAT_COUNT)
    datCount = DAT_COUNT;
  qk_setDataCount(datCount);

  if(qk_configValueB(CFG_TEST_BOOL))
    qk_setDataType(QK_DATA_TYPE_FLOAT);
  else
    qk_setDataType(QK_DATA_TYPE_INT);
#endif
}

void test_start()
{

}

void test_stop()
{

}

volatile float dat0_f = 0.0, dat1_f = 10.0, dat2_f = 20.00;
volatile int32_t dat0 = 0, dat1 = 10, dat2 = 20;


volatile int count = 0;

float args[2];

void test_sample()
{
  QK_DEBUG("test_sample() [called %d times]", ++count);

#if defined( QK_IS_DEVICE )
  if(qk_dataType() == QK_DATA_TYPE_FLOAT)
  {
    dat0_f += 0.1;
    dat1_f += 0.1;
    dat2_f += 0.1;
    qk_setDataValueF(DAT_TEST_0, dat0_f);
    qk_setDataValueF(DAT_TEST_1, dat1_f);
    qk_setDataValueF(DAT_TEST_2, dat2_f);
  }
  else
  {
    dat0 += 1;
    dat1 += 1;
    dat2 += 1;
    qk_setDataValueI(DAT_TEST_0, dat0);
    qk_setDataValueI(DAT_TEST_1, dat1);
    qk_setDataValueI(DAT_TEST_2, dat2);
  }
  args[0] = 123.123;
  args[1] = 456.456;

  //qk_setEventArgs(EVT_SAMPLE, );

  qk_fireEvent(EVT_SAMPLE, args, 2, "tuntz");

  args[0] = 1.123;
  args[1] = 2.456;

  qk_fireEvent(EVT_SAMPLE, args, 2, "sample event fired! %0 %1");
#endif

}

void isr()
{
  QK_DEBUG("isr()");
  //hal_uart_writeByte(HAL_UART_ID_1, 'A');
  _toggleLED();
}

void qk_setup()
{
  // Board
  qk_setBoardName("QkTestBoard");
  qk_setBoardVersion(0xABCD);

  // Configurations
  qk_setConfigCount(CFG_COUNT);
  qk_setConfigBuffer(cfg_buf);
  qk_setConfigCallback(test_config);

  qk_setConfigLabel(CFG_TEST_INT, "INT");
  qk_setConfigType(CFG_TEST_INT, QK_CONFIG_TYPE_INTDEC);
  qk_setConfigValueI(CFG_TEST_INT, 123);

  qk_setConfigLabel(CFG_TEST_HEX, "HEX");
  qk_setConfigType(CFG_TEST_HEX, QK_CONFIG_TYPE_INTHEX);
  qk_setConfigValueI(CFG_TEST_HEX, 0xEFDA);

  qk_setConfigLabel(CFG_TEST_BOOL, "BOOL");
  qk_setConfigType(CFG_TEST_BOOL, QK_CONFIG_TYPE_BOOL);
  qk_setConfigValueB(CFG_TEST_BOOL, true);

  qk_setConfigLabel(CFG_TEST_FLOAT, "FLOAT");
  qk_setConfigType(CFG_TEST_FLOAT, QK_CONFIG_TYPE_FLOAT);
  qk_setConfigValueF(CFG_TEST_FLOAT, 10.123);

  qk_datetime dateTime;
  dateTime.year = 13;
  dateTime.month = 8;
  dateTime.day = 17;
  dateTime.hours = 22;
  dateTime.minutes = 38;
  dateTime.seconds = 1;

  qk_setConfigLabel(CFG_TEST_DATETIME, "DATETIME");
  qk_setConfigType(CFG_TEST_DATETIME, QK_CONFIG_TYPE_DATETIME);
  qk_setConfigValueDT(CFG_TEST_DATETIME, dateTime);

  qk_setConfigLabel(CFG_TEST_TIME, "TIME");
  qk_setConfigType(CFG_TEST_TIME, QK_CONFIG_TYPE_TIME);
  qk_setConfigValueDT(CFG_TEST_TIME, dateTime);

#if defined( QK_IS_DEVICE )
  // Data
  qk_setDataCount(DAT_COUNT);
  qk_setDataBuffer(dat_buf);
#ifdef FLOAT
  qk_setDataType(QK_DATA_TYPE_FLOAT);
#else
  qk_setDataType(QK_DATA_TYPE_INT);
#endif

  qk_setDataLabel(DAT_TEST_0, "DAT0");
  qk_setDataLabel(DAT_TEST_1, "DAT1");
  qk_setDataLabel(DAT_TEST_2, "DAT2");

  // Events
  qk_setEventCount(EVT_COUNT);
  qk_setEventBuffer(evt_buf);

  qk_setEventLabel(EVT_SAMPLE, "SAMPLE");
#endif

  // Callbacks
  qk_setInitCallback(test_init);
#if defined( QK_IS_DEVICE )
  qk_setSampleCallback(test_sample);
  qk_setStartCallback(test_start);
  qk_setStopCallback(test_stop);
#endif

  //qk_setSamplingFrequency(5);

  /*hal_timer_setCallback(HAL_TIMER_ID_2, &isr);
  hal_timer_setPeriod(HAL_TIMER_ID_2, 1000, HAL_TIMER_SCALE_MSEC);
  hal_timer_start(HAL_TIMER_ID_2);
  while(1);*/
}

int main(void)
{
  hal_gpio_init();
  while(1)
  {
    _setLED(false);
    delay_ms(100);
    _setLED(true);
    delay_ms(100);
  }

  return qk_main();
}
