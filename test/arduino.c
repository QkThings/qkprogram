#include <qk_program.h>
#include <qk_debug.h>

#define DAT_COUNT 3
#define EVT_COUNT 1

qk_data  dat_buf[DAT_COUNT];
qk_event evt_buf[EVT_COUNT];

volatile uint16_t counter = 10;
volatile float args[2];

volatile float time = 0.0;

void test_sample()
{
  time += 0.01;

  hal_toggleLED();
  QK_DEBUG("test_sample() [called %d times]", counter);
  //qk_setDataValueI(0, counter++);
  qk_data_set_value_f(0, time);
  qk_data_set_value_f(1, sin(2.0*M_PI*2.0*time));
  qk_data_set_value_f(2, sin(2.0*M_PI*3.0*time)*cos(2.0*M_PI*time));

  args[0] = 123.123;
  args[1] = 456.456;
  qk_event_generate(0, args, 2, "tuntz");
  args[0] = 1.123;
  args[1] = 2.456;
  qk_event_generate(0, args, 2, "sample event fired! %0 %1");
}

void qk_setup()
{
  qk_board_set_name("ArduinoTest");
  qk_board_set_version(0x1234);

  qk_data_set_buffer(dat_buf, DAT_COUNT);
  qk_data_set_type(QK_DATA_TYPE_FLOAT);
  qk_data_set_label(0, "TIME");
  qk_data_set_label(1, "SIN");
  qk_data_set_label(2, "WAVE");

  qk_event_set_buffer(evt_buf, EVT_COUNT);
  qk_event_set_label(0, "SAMPLE");

  qk_set_sample_callback(test_sample);
}

int main(void) 
{
  return qk_main();
}
