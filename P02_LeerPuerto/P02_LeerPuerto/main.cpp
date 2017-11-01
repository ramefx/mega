/*
 * P02_LeerPuerto.cpp
 *
 * Created: 13/12/2016 05:02:06 p. m.
 * Author : a
 */ 

#include <avr/io.h>

void Ports_Setup();	//Colocamos el prototipo de la función

void Ports_Setup() //desarrolamos la funcion y configuramos los puertos
{
	DDRD = 0x00; //Colocamos todo el puerto D de entrada 0b00000000
	DDRC = 0x3F; //Colocamos todo el puerto C de salida 0b111111
	PORTC = 0x00; // limpiamos el puerto para asegurarnos de no tener basura	
}


int main(void)
{
	int var =0;
	Ports_Setup();	//Mandamos llamar la función para configurar los puertos.
	while(1)
	{
		//TODO:: Please write your application code
		var = PIND;//leemos pueto D y asignamos a var
		PORTC = var;	//Lo que leemos por el puerto D sale por el puerto C
	}
}