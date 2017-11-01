/*
* P02_blue_m_s
* Versión: 1
* Creado: 18/01/2017    07:01:22 p. m.
* Autor: r
*
* Microcontrolador: ATmega328P
* Tarjeta: NoMADA Pro
* Comentarios:
*/

#define DATA_REGISTER_EMPTY (1<<UDRE0)
#define RX_COMPLETE (1<<RXC0)
#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)



#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
char valor = 0; //variable que va a ametar y disminuir



#define RX_BUFFER_SIZE0 8
char rx_buffer0[RX_BUFFER_SIZE0];       //Buffer de Recepción
unsigned char rx_wr_index0=0;                 //Indice del Buffer de Recepción

void conf_puertos(void);         //Prototipos de las Funciones
void conf_interrupciones_externas(void);
void conf_usart0(void);

void conf_puertos(void)
{
	DDRB = (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
	DDRC = (1<<DDC5) | (1<<DDC4) | (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
	DDRD = (0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (1<<DDD1) | (0<<DDD0);
	
	PORTB = (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
	PORTC = (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);
	PORTD = (0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);
}

void conf_interrupciones_externas(void)
{
	EICRA = (0<< ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
	EIMSK = (1<< INT1) | (1<<INT0);
	EIFR = (1 << INTF1) | (1<<INTF0);

	PCICR = (0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0);
	PCIFR = (0<<PCIF2) | (0<<PCIF1) | (0<<PCIF0);
	PCMSK2 = (0<<PCINT23) | (0<<PCINT22) | (0<<PCINT21) | (0<<PCINT20) | (0<<PCINT19) | (0<<PCINT18) | (0<<PCINT17) | (0<<PCINT16);
	PCMSK1 = (0<<PCINT14) | (0<<PCINT13) | (0<<PCINT12) | (0<<PCINT11) | (0<<PCINT10) | (0<<PCINT9) | (0<<PCINT8);
	PCMSK0 = (0<<PCINT7) | (0<<PCINT6) | (0<<PCINT5) | (0<<PCINT4) | (0<<PCINT3) | (0<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);
}

void conf_usart0(void)
{
	UCSR0A = (0<<RXC0) | (0<< TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) |(0<<U2X0) | (0<<MPCM0);
	UCSR0B = (1<< RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<TXB80);
	UCSR0C = (0<< UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
	UBRR0 = 95;
}



ISR (INT0_vect, ISR_NAKED)
{
	UDR0 = valor++;
	//Inserta tu código aquí
	reti();
}

ISR (INT1_vect, ISR_NAKED)
{
	UDR0 = valor--;
	//Inserta tu código aquí
	reti();
}

ISR (USART_RX_vect, ISR_NAKED)
{
	
	char status,data;
	status=UCSR0A;
	data=UDR0;
	PORTC=data;

	reti();
}


int main(void)
{
	conf_puertos();
	conf_interrupciones_externas();
	conf_usart0();

	sei();       //Habilitación Global de las Interrupciones


	while(1)
	{
		//TODO:: Please write your application code
	}
}