#include <qk_program.h>


qk_data_t dat_buf[1];

volatile uint16_t counter = 0;

void test_sample()
{
  qk_setDataValueI(0, counter++);
}

void qk_setup()
{
  qk_setBoardName("ArduinoTest");
  qk_setBoardVersion(0x1234);

  qk_setDataCount(1);
  qk_setDataBuffer(dat_buf);
  qk_setDataType(QK_DATA_TYPE_INT);
  qk_setDataLabel(0, "COUNTER");

  qk_setSampleCallback(test_sample);
}

int main(void) 
{
  hal_init();
  _blinkLED(1, 20);
  qk_core_init();
  qk_board_init();
  _blinkLED(1, 20);
  qk_setup();
  qk_board_setup();
  _blinkLED(3, 30);
  qk_loop();

  delay_ms(1000);
	_setLED(true);
  while (1)
  {
    _setLED(false);
    delay_ms(150);
    _setLED(true);
    delay_ms(150);
  }

  return 0;
}
