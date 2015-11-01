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

#ifndef QK_HAL_H
#define QK_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void qk_hal_init(void);
void qk_hal_led_set(bool state);
void qk_hal_hwfc_out(bool state);
bool qk_hal_hwfc_in(void);
int qk_hal_uart_read(uint8_t *buf, uint32_t count);
void qk_hal_uart_write(uint8_t *buf, uint32_t count);
void qk_hal_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* QK_HAL_H */
