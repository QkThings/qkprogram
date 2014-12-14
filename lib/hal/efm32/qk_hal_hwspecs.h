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

#ifndef QK_HAL_HWSPECS_H_
#define QK_HAL_HWSPECS_H_

#define _QK_HAL_UART_TX QK_GPIO_PIN( E, 10)
#define _QK_HAL_UART_RX QK_GPIO_PIN( E, 11)

#if defined( EFM32G_STK )
#define _QK_HAL_LED   QK_GPIO_PIN( C , 3)
#define _QK_HAL_PB    QK_GPIO_PIN( B , 9)
#define _QK_HAL_DET   QK_GPIO_PIN( C , 12)
#elif defined( EFM32G_OLIMEX )
#define _QK_HAL_LED   QK_GPIO_PIN( A , 0)
#define _QK_HAL_PB    QK_GPIO_PIN( A , 1)
#define _QK_HAL_DET   QK_GPIO_PIN( A , 2)
#elif defined( EFM32TG_DEVBOARD )
#define _QK_HAL_LED   QK_GPIO_PIN( A , 10)
#define _QK_HAL_PB    QK_GPIO_PIN( C , 0)
#define _QK_HAL_DET   QK_GPIO_PIN( C , 1)
#define _QK_HAL_HWFCO QK_GPIO_PIN( F, 3)
#define _QK_HAL_HWFCI QK_GPIO_PIN( F, 2)
#elif defined( EFM32_COMM_BT_2_0 )
#define _QK_HAL_LED   QK_GPIO_PIN( A , 10)
#define _QK_HAL_PB    QK_GPIO_PIN( C , 0)
#define _QK_HAL_DET   QK_GPIO_PIN( C , 1)
#define _QK_HAL_HWFCO QK_GPIO_PIN( F, 3)
#define _QK_HAL_HWFCI QK_GPIO_PIN( F, 2)
#else
#error "EFM32 board not defined"
#endif


#endif /* QK_HAL_HWSPECS_H_ */
