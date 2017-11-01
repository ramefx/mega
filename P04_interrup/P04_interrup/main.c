/*
 * P04_interrup.c
 * interrupciones incremento decremento
 * Created: 13/12/2016 05:57:47 p. m.
 * Author : a
 */ 
#define  F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>		//para utilizar los vectores de interrupcion
#include <util/delay.h>
void conf_puertos();			//se declara ptroroptipo de funcion
void conf_iterrupcopn_externa();//prototipo interrupciopnes

void conf_puertos(){
	DDRC=0b0011111;		//salidas
	DDRD=0b00000000;	//entradas
	PORTC=0x00;			//limpia puerto
	
}

void conf_iterrupcopn_externa(){
	EICRA = (1<<ISC11)|(0<<ISC10)|(1<<ISC01)|(0<<ISC00);//Configura el registro de interrupcion con flacos de bajada 0b00001010
	EIMSK = (1<<INT1)|(1<<INT0);//habilitamos las iterrupciones INT0 e INT1
	EIFR = (0<<INTF1)|(0<<INTF0);//LIMPIAMOS LAS BANDERAS DE INTERRUPCION cuando se genera un evento se genero un bit que indica que paso
	
}
//vector de interrupcion
ISR(INT0_vect,ISR_NAKED)
	{
	PORTC++;//incrementamos en 1 PORTC--
	_delay_ms(200);//reducir el ruido del boton sin cap
	reti();//regresamos de la interrupcion y limpia la bandera
	} 
	
//Vector de interrupcion
ISR(INT1_vect,ISR_NAKED)
	{
	PORTC--;//decrementamos en 1 PORTC++
	_delay_ms(200);
	reti();//regresamos de la interrupcion y limpia la bandera
	
}

int main(void)
{
	conf_puertos();
	conf_iterrupcopn_externa();
	sei();//habilitar de forma global las interrupciones cli() deshabilita las interrupciones de forma global
    while (1) 
    {
    }
}

