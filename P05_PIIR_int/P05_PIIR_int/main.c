/*
 * P05_PIIR_int.c
 *
 * Created: 13/12/2016 07:16:04 p. m.
 * Author : a
 */ 
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>		//para utilizar los vectores de interrupcion
#include <util/delay.h>
void conf_puertos();			//se declara ptroroptipo de funcion
void conf_iterrupcopn_externa();//prototipo interrupciopnes

void conf_puertos(){
	DDRB=0x01;		//salidas
	DDRD=0x00;	//entradas
	PORTB=0x01;			//mantenemos el rele apagado
	
}

void conf_iterrupcopn_externa(){
	EICRA = (1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(0<<ISC00);//Configura el registro de interrupcion con flacos de bajada 0b00001010
	EIMSK = (1<<INT1)|(0<<INT0);//habilitamos las iterrupciones INT0 e INT1
	EIFR = (0<<INTF1)|(0<<INTF0);//LIMPIAMOS LAS BANDERAS DE INTERRUPCION
	
}

ISR(INT1_vect,ISR_NAKED){
	
	PORTB=0x00;//modulo de reles activa en 0
	_delay_ms(10000);
	PORTB=0x01;
	reti();//regresamos de interrupcion
	
}

int main(void)
{
	conf_puertos();
	conf_iterrupcopn_externa();
	sei();//habilitar de forma global las interrupciones
	/* Replace with your application code */
	while (1)
	{
	}
}

