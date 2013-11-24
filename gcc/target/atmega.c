#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

#define F_CPU 16000000UL
#define baud 57600
#define ubrr ((F_CPU / (baud * 16UL)) - 1)
#define buffer 32

volatile uint8_t data[buffer];
volatile uint8_t dataIdx;

void hal_gpio_init(void)
{
  DDRB = (1<<5); // Define data direction on porto b, 5 bit for Led
}

//void hal_uart_init(void)
//{
//  UBRR0H = (uint8_t)(ubrr>>8); // Set baud rate
//  UBRR0L = (uint8_t)ubrr;
//
//  UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable receiver and transmitter
//  UCSR0C = (1<<USBS0)|(3<<UCSZ00); // Set frame format: 8data, 2stop bit
//}
//
//void hal_uart_writeByte(uint8_t b)
//{
//  while ( !( UCSR0A & (1<<UDRE0)) ); // Wait for empty transmit buffer
//  UDR0 = b; // Put data into buffer, sends the data
//}
//
//void hal_uart_writeString(uint8_t* ptr)
//{
//  while(*ptr != 0x00)
//  {
//    hal_uart_writeByte(*ptr);
//    ptr++;
//  }
//
//  hal_uart_writeByte('\n');
//	hal_uart_writeByte('\r');
//}
//
//uint8_t hal_uart_readByte(void)
//{
//  while ( !(UCSR0A & (1<<RXC0)) ); // Wait for data to be received
//  return UDR0; // Get and return received data from buffer
//}
//
//void hal_uart_readString(void) // Ultra blocking function =D
//{
//  uint8_t* ptr = data;
//  while(*ptr != 0x00)
//  {
//    *ptr = hal_uart_readByte();
//    ptr++;
//  }
//}

bool _getLED(void)
{ 
  return (PORTB & 0x20 ? true : false);
}

void _setLED(bool on)
{
  if (on == true)
    PORTB |= (1<<5); 
  else
    PORTB &= ~(1<<5); 
}

bool _toggleLED(void)
{
  bool tmp = _getLED();
  _setLED(!tmp); 
  
  return !tmp;
}

//ISR(USART_RX_vect)
//{
//  data[dataIdx] = UDR0;
//
//	if (data[dataIdx] == '\n')
//  {
//		//data[dataIdx] == '\0';
//		dataIdx = 0;
//    // TEST if buffer full store in another buffer (bigger one) because \n wasnt received and therefore message is not complete
//		// if \n is received then message is finished
//		// Do something fast HERE because next Rx will overwrite buffer
//    // perhaps strcat() it with ..., store it, or dispatch it
//  }
//
//	if (dataIdx < buffer)
//  {
//    dataIdx++;
//  }
//  else
//  {
//    dataIdx = 0;
//  }
//
//}
//
////ISR(USART_TX_vect)
////{
//  //UDR0 = data[0];
////}
//
////ISR(TIMER1_OVF_vect) //Overflow timer, n muda f aquisicao
////{
////  _toggleLED(); // testing
////}
//
//ISR(TIMER1_COMPA_vect) //compare timer, decimacao
//{ 
//  _toggleLED();           // testing
//  hal_uart_writeString(data); // testing
//} 
//
////void hal_enable_timer_ovf(void)
////{
////  TCCR1A = 0;             // normal mode defined
////  TCCR1B |= (1 << CS12);  // timer1 same as system clock
////  TIMSK1 |= (1 << TOIE1); // enable timer1 overflow interrupt
////}
//
//void hal_enable_timer(int prescale,unsigned long tick)
//{
//  //OCR1A = 0x3D08;        // 0x3D08 = 1s  @ 16Mhz Clock (mode 4) in case 1024
//  OCR1A = tick;	           // Will trigger Clock/prescale*tick times a second
//
//  TCCR1B |= (1 << WGM12);  // (mode 4)
//  TIMSK1 |= (1 << OCIE1A); // Set compare match to TIMER1_COMPA_vect
//
//	switch (prescale)
//  {
//    case 1: TCCR1B |= (1 << CS10); break ;                  // Clock, for very fast measurements
//    case 8: TCCR1B |= (1 << CS11); break ;                  // Clock/8
//		case 64: TCCR1B |= (1 << CS11) | (1 << CS10); break;    // Clock/64
//    case 256: TCCR1B |= (1 << CS12); break ;                // Clock/256
//    case 1024: TCCR1B |= (1 << CS12) | (1 << CS10); break ; // Clock/1024, 0.001024 seconds per tick
//    default:
//    	TCCR1B |= (1 << CS12) | (1 << CS11) | (1 << CS10);    // External Clock buddy
//  }
//
//}
//
//void hal_enable_interrupts(void)
//{
//  UCSR0B |= (1 << RXCIE0); // Enable the USART Recieve Complete interrupt (USART_RXC);
//  //UCSR0B |= (1 << TXCIE0); // Enable the USART Transmit Complete interrupt (USART_TXC);
//
//  hal_enable_timer(1024,0x3D08);
//	//hal_enable_timer_ovf();
//
//  sei();
//}

int main(void) 
{
  
  hal_gpio_init();
//  hal_uart_init();
//  hal_enable_interrupts();

	dataIdx = 0;

  while (1)
  {
    
		// 1 char echo test 
    //data[0] = hal_uart_readByte();
    //hal_uart_writeByte(data[0]);
    
    
		// String echo testing  
    //hal_uart_readString();
    //_delay_ms(500);
    //hal_uart_writeString("teste");
    _toggleLED();
    //_setLED(true);
    _delay_ms(200);
    //_toggleLED();
    //_setLED(false);
    //_delay_ms(200);
    
  }
  
  return 0;
}
