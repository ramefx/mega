/*
 * P01_a_incrementDelay.c
 *
 * Created: 01/02/2017 05:23:30 p. m.
 * Author : a
 */ 

#define F_CPU 8000000	// 11059200 14745600 Especificamos la frecuencia del cristal que será utilizado para calcular el delay en la librería delay.h
#include <avr/io.h>
#include <util/delay.h>	// Colocamos la librería delay.h para colocar retardos en ms
void Ports_Setup();
void Ports_setup()
{
	DDRC = 0xFF;	//Colocamos todo el puerto de Salida
	PORTC = 0x00;
}
int main(void)
{
	Ports_setup();
	while(1)
	{
		_delay_ms(200);		//Espera 200 ms
		PORTC++;			//Despliega el dato en el Puerto C e incrementa el puerto C después de desplegarlo
	}
}
