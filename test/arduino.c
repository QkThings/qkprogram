#include <qk_program.h>
#include <qk_debug.h>

#define DAT_COUNT 3
#define EVT_COUNT 1

qk_data dat_buf[DAT_COUNT];
qk_event evt_buf[EVT_COUNT];

volatile uint16_t counter = 10;
volatile float args[2];

volatile float time = 0.0;

void test_sample()
{
  time += 0.01;

  _toggleLED();
  QK_DEBUG("test_sample() [called %d times]", counter);
  //qk_setDataValueI(0, counter++);
  qk_setDataValueF(0, time);
  qk_setDataValueF(1, sin(2.0*M_PI*2.0*time));
  qk_setDataValueF(2, sin(2.0*M_PI*3.0*time)*cos(2.0*M_PI*time));

  args[0] = 123.123;
  args[1] = 456.456;
  qk_fireEvent(0, args, 2, "tuntz");
  args[0] = 1.123;
  args[1] = 2.456;
  qk_fireEvent(0, args, 2, "sample event fired! %0 %1");
}

void qk_setup()
{
  qk_setBoardName("ArduinoTest");
  qk_setBoardVersion(0x1234);

  qk_setDataBuffer(dat_buf, DAT_COUNT);
  qk_setDataType(QK_DATA_TYPE_FLOAT);
  qk_setDataLabel(0, "TIME");
  qk_setDataLabel(1, "SIN");
  qk_setDataLabel(2, "WAVE");

  qk_setEventBuffer(evt_buf, EVT_COUNT);
  qk_setEventLabel(0, "SAMPLE");

  qk_setSampleCallback(test_sample);
}

int main(void) 
{
  return qk_main();
}
