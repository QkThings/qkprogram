#ifndef QK_RAW_H
#define QK_RAW_H

#include <stdint.h>

extern void qk_board_led_blink(unsigned int n, unsigned int msec);
extern void qk_board_hwfc_out(bool state);
extern bool qk_board_hwfc_in(void);
extern int qk_uart_read(uint8_t uart_id, uint8_t *buf, uint16_t count);
extern void qk_uart_write(uint8_t uart_id, uint8_t *buf, uint16_t count);

#endif