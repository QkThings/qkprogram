#ifndef QK_SETTINGS_H
#define QK_SETTINGS_H

/*****************************************************************************
 * FEATURES
 *****************************************************************************/
//#define _QK_FEAT_POWER_MANAGEMENT

/*****************************************************************************
 * CONSTANTS
 *****************************************************************************/

#ifndef _QK_EVENT_MAX_PENDING
#define _QK_EVENT_MAX_PENDING       	(8)
#endif

#ifndef _QK_EVENT_MAX_ARGS
#define _QK_EVENT_MAX_ARGS           	(8)
#endif

#ifndef _QK_ACTION_MAX_ARGS
#define _QK_ACTION_MAX_ARGS          	(8)
#endif

#ifndef _QK_PACKET_DATBUF_SIZE
#define _QK_PACKET_PAYLOAD_SIZE      	(48)
#endif

#ifndef _QK_BOARD_NAME_SIZE
#define _QK_BOARD_NAME_SIZE          	(20)
#endif

#ifndef _QK_LABEL_SIZE
#define _QK_LABEL_SIZE               	(20)
#endif

#ifndef _HAL_UART_RXBUF_SIZE
#define _HAL_UART_RXBUF_SIZE         	(32)
#endif

#define _QK_DEFAULT_SAMPFREQ        	(2)
#define _QK_EVENT_MAX_FIRED         	(8)
#define _QK_IDLE_TIMEOUT             	(1)   // Idle timeout (s)
#define _QK_GOTOPD_TIMEOUT           	(30)  // Go to power-down timeout (100's of ms)
#define _QK_WAITACK_TIMEOUT          	(999) // Wait ACK timeout (ms)

#define _HAL_UART_BAUD_DEFAULT_LOW   	38400
#define _HAL_UART_BAUD_DEFAULT_HIGH  	125000

#endif /* QK_SETTINGS_H */
