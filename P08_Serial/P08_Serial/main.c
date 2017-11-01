//#define F_CPU 11059200
#define F_CPU 14745600
//#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
void conf_puertos();
void con_usart0();//9600baudios s/p con 1 bit de error

void conf_puertos(){
	DDRC=0x3F;		//configurar puerto c de salida
	DDRD=0x02;		//configurar pd1 tx como salida 
	PORTC=0x00;		//limpiar pueto
	
}

void con_usart0(){//configuracion de puerto serie UART/USART
	//--------------|velocidad|--------------
	UCSR0A=(0<<TXC0)|(0<<U2X0)|(0<<MPCM0);//no utilizamos doble de velocidad y no utilizamos comunicacion multiprocesador
	UCSR0B=(1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)/*recepcion de datos*/|(1<<TXCIE0)/*habilitar trasmision*/|(0<<UCSZ02)|(0<<TXB80);
	UCSR0C=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
	//UCSR0C=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UMSEL00)/*aqui esta mal*/|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
	UBRR0=95;//CONFIGURAMOS UTILIZANDO UN CRISTAL DE EN DSHEET /14.7456mhZ 95//11.0592Mhz 71//8MHz 51//1Mhz 6/
	//UBRR0=51;
}

ISR(USART_RX_vect,ISR_NAKED){//Interrupcion de resepcion de datos 
	char dato;
	dato=UDR0;//recepcion de caracter por puerto PD1 Rx
	switch(dato){
		case 'r': PORTC = 0x11; break;//cambia a 0x01
		case 'a': PORTC = 0x02; break;
		case 'm': PORTC = 0x04; break;
		case 'i': PORTC = 0x08; break;
		case 's': PORTC = 0x10; break;
		case 'p': PORTC = 0x20; break;
		case '+': PORTC = 0x00; break;
		default: PORTC = 0x3f; break;
	}
	UDR0=dato;/*se agrego esta linea, tendra que ve con UMSEL?, es el echo*/
	reti();//regreso de la interrupcion
}


int main(void)
{
	conf_puertos();
	con_usart0();
	sei();
    /* Replace with your application code */
    while (1) 
    {
    }
}