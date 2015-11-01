/*
 * QkThings LICENSE
 * The open source framework and modular platform for smart devices.
 * Copyright (C) 2014 <http://qkthings.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qk_hal.h"
#include "qk_hal_p.h"

#ifdef QK_PROGRAM_USE_PERIPH
 #include "qk_peripheral.h"

void qk_hal_init(void)
{
  qk_peripheral_setup();

  qk_gpio_set_mode(_QK_HAL_LED, QK_GPIO_MODE_OUTPUT);

  qk_gpio_set_mode(_QK_HAL_HWFCO, QK_GPIO_MODE_OUTPUT);
  qk_gpio_set_mode(_QK_HAL_HWFCI, QK_GPIO_MODE_INPUT_PULL_UP);

  qk_gpio_set_mode(_QK_HAL_DET, QK_GPIO_MODE_INPUT);

//  qk_gpio_interrupt_set(_QK_HAL_DET, QK_GPIO_FLAG_RISE | QK_GPIO_FLAG_FALL);
}

void qk_hal_led_set(bool state)
{
#ifdef QK_PROGRAM_LED_ACTIVE_LOW
  state = !state;
#endif

  if(state)
  {
    qk_gpio_set_pin(_QK_HAL_LED, true);
  }
  else
  {
    qk_gpio_set_pin(_QK_HAL_LED, false);
  }
}

void qk_hal_hwfc_out(bool state)
{
	qk_gpio_set_pin(_QK_HAL_HWFCO, state);
}

bool qk_hal_hwfc_in(void)
{
	return qk_gpio_get_pin(_QK_HAL_HWFCI);
}

int qk_hal_uart_read(uint8_t *buf, uint32_t count)
{
	return qk_uart_read(_QK_PROGRAM_UART_BOARD, buf, count);
}

void qk_hal_uart_write(uint8_t *buf, uint32_t count)
{
	qk_uart_write(_QK_PROGRAM_UART_BOARD, buf, count);
}

void qk_hal_delay_ms(uint32_t ms)
{
	delay_ms(ms);
}
#else
#warning "QK_PERIPHERAL NOT USED"
#endif
