#include <qk_program.h>
#include <qk_debug.h>

#define CFG_COUNT 6
#define DAT_COUNT 3
#define EVT_COUNT 1

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
  uint8_t datCount = qk_config_value_i(CFG_TEST_INT)+1;

  if(datCount > DAT_COUNT)
    datCount = DAT_COUNT;
  qk_data_set_count(datCount);

  if(qk_config_value_b(CFG_TEST_BOOL))
    qk_data_set_type(QK_DATA_TYPE_FLOAT);
  else
    qk_data_set_type(QK_DATA_TYPE_INT);
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
  if(qk_data_type() == QK_DATA_TYPE_FLOAT)
  {
    dat0_f += 0.1;
    dat1_f += 0.1;
    dat2_f += 0.1;
    qk_data_set_value_f(DAT_TEST_0, dat0_f);
    qk_data_set_value_f(DAT_TEST_1, dat1_f);
    qk_data_set_value_f(DAT_TEST_2, dat2_f);
  }
  else
  {
    dat0 += 1;
    dat1 += 1;
    dat2 += 1;
    qk_data_set_value_i(DAT_TEST_0, dat0);
    qk_data_set_value_i(DAT_TEST_1, dat1);
    qk_data_set_value_i(DAT_TEST_2, dat2);
  }
  args[0] = 123.123;
  args[1] = 456.456;

  //qk_setEventArgs(EVT_SAMPLE, );

  qk_event_generate(EVT_SAMPLE, args, 2, "tuntz");

  args[0] = 1.123;
  args[1] = 2.456;

  qk_event_generate(EVT_SAMPLE, args, 2, "sample event fired! %0 %1");
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
  qk_board_set_name("qktest");
  qk_board_set_version(0xABCD);

  // Configurations
  qk_setConfigCount(CFG_COUNT);
  qk_config_set_buffer(cfg_buf);
  qk_set_config_callback(test_config);

  qk_config_set_label(CFG_TEST_INT, "INT");
  qk_config_set_type(CFG_TEST_INT, QK_CONFIG_TYPE_INTDEC);
  qk_config_set_value_i(CFG_TEST_INT, 123);

  qk_config_set_label(CFG_TEST_HEX, "HEX");
  qk_config_set_type(CFG_TEST_HEX, QK_CONFIG_TYPE_INTHEX);
  qk_config_set_value_i(CFG_TEST_HEX, 0xEFDA);

  qk_config_set_label(CFG_TEST_BOOL, "BOOL");
  qk_config_set_type(CFG_TEST_BOOL, QK_CONFIG_TYPE_BOOL);
  qk_config_set_value_b(CFG_TEST_BOOL, true);

  qk_config_set_label(CFG_TEST_FLOAT, "FLOAT");
  qk_config_set_type(CFG_TEST_FLOAT, QK_CONFIG_TYPE_FLOAT);
  qk_config_set_value_f(CFG_TEST_FLOAT, 10.123);

  qk_datetime dateTime;
  dateTime.year = 13;
  dateTime.month = 8;
  dateTime.day = 17;
  dateTime.hours = 22;
  dateTime.minutes = 38;
  dateTime.seconds = 1;

  qk_config_set_label(CFG_TEST_DATETIME, "DATETIME");
  qk_config_set_type(CFG_TEST_DATETIME, QK_CONFIG_TYPE_DATETIME);
  qk_config_set_value_dt(CFG_TEST_DATETIME, dateTime);

  qk_config_set_label(CFG_TEST_TIME, "TIME");
  qk_config_set_type(CFG_TEST_TIME, QK_CONFIG_TYPE_TIME);
  qk_config_set_value_dt(CFG_TEST_TIME, dateTime);

#if defined( QK_IS_DEVICE )
  // Data
  qk_data_set_buffer(dat_buf, DAT_COUNT);
#ifdef FLOAT
  qk_data_set_type(QK_DATA_TYPE_FLOAT);
#else
  qk_data_set_type(QK_DATA_TYPE_INT);
#endif

  qk_data_set_label(DAT_TEST_0, "DAT0");
  qk_data_set_label(DAT_TEST_1, "DAT1");
  qk_data_set_label(DAT_TEST_2, "DAT2");

  // Events
  qk_event_set_buffer(evt_buf, EVT_COUNT);

  qk_event_set_label(EVT_SAMPLE, "SAMPLE");
#endif

  // Callbacks
  qk_set_init_callback(test_init);

  qk_set_sample_callback(test_sample);
  qk_set_start_callback(test_start);
  qk_set_stop_callback(test_stop);

}

int main(void)
{
  return qk_main();
}
