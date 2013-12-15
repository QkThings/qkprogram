#include <qk_program.h>
#include <qk_debug.h>

#define DAT_COUNT 1
#define EVT_COUNT 1

qk_data dat_buf[DAT_COUNT];
qk_event evt_buf[EVT_COUNT];

volatile uint16_t counter = 10;
volatile float args[2];

void test_sample()
{
  _toggleLED();
  QK_DEBUG("test_sample() [called %d times]", counter);
  qk_setDataValueI(0, counter++);
  //while(1) {
    //_toggleLED();
  //}
  //_qk_debug("hello");
 // _qk_debug("test_sample() [called %d times]", counter);

  //_qk_comm_sendString("asdas\0", _comm_board);

  args[0] = 123.123;
  args[1] = 456.456;

  qk_fireEvent(0, args, 2, "tuntz");

//  args[0] = 1.123;
//  args[1] = 2.456;
//
//  qk_fireEvent(0, args, 2, "sample event fired! %0 %1");
}

void qk_setup()
{
  qk_setBoardName("ArduinoTest");
  qk_setBoardVersion(0x1234);

  qk_setDataBuffer(dat_buf, DAT_COUNT);
  qk_setDataType(QK_DATA_TYPE_INT);
  qk_setDataLabel(0, "CNT");

  qk_setEventBuffer(evt_buf, EVT_COUNT);
  qk_setEventLabel(0, "SAMPLE");

  qk_setSampleCallback(test_sample);
}

int main(void) 
{
  qk_main();

//  hal_init();
//  _blinkLED(1, 20);
//  qk_core_init();
//  qk_board_init();
//  _blinkLED(1, 20);
//  qk_setup();
//  qk_board_setup();
//  _blinkLED(3, 30);
//
//  qk_loop();
//
//  delay_ms(1000);
//	_setLED(true);
//  while (1)
//  {
//    _setLED(false);
//    delay_ms(150);
//    _setLED(true);
//    delay_ms(150);
//  }

  return 0;
}
