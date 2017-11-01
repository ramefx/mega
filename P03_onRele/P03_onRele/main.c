/*
 * P03_onRele.c
 *
 * Created: 13/12/2016 05:33:01 p. m.
 * Author : a
 */ 

/*
 * P03_OnRelay.cpp
 *
 * Created: 13/12/2016 05:19:20 p. m.
 * Author : a
 */ 
#define F_CPU 14745600
#include <avr/io.h>
#include <util/delay.h> //libreria de delays

//void conf_puertos();

void conf_puertos(){
	DDRB=0x01; //configurar puerto de salida en hexa 0x01
	PORTB=0x01; // en bin 0b0000001
}


int main(void)
{
	conf_puertos();
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(2000);		//espero 2 seg
		PORTB=0x00;				//apago puerto
		_delay_ms(2000);		//espero 2 sef
		PORTB=0x01;				//prendo puerto
    }
}
