/*
 * P10_ADC8.c
 *
 * Created: 21/08/2017 04:33:58 p. m.
 * Author : a
 */ 

#define F_CPU 8000000 // Frecuencia del cristal utilizado de 11.0592 MHz
#include <avr/io.h>
#include <avr/interrupt.h> // Se coloca la librería interrupt.h para el manejo de Interrupciones
unsigned int adc_valor=0;
void Ports_setup(); // Prototipos de las funciones
void ADC_setup(); //Prototipos de las funciones.
void Ports_setup() // Función para la configuración de los puertos
{
	DDRC = 0x00; // Puerto C entrada
	DDRD = 0xFF; // Puerto D salida
	PORTD = 0x00; // Puerto D puesto en ceros.
}
void ADC_setup() // Función para la configuración de los registros del ADC
{
	ADMUX = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(0<<ADIF)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
	DIDR0 = (0<<ADC5D)|(0<<ADC4D)|(0<<ADC3D)|(0<<ADC2D)|(0<<ADC1D)|(0<<ADC0D);
}
ISR(ADC_vect,ISR_NAKED) // Interrupción del ADC
{
	adc_valor = ADC;
	reti(); //Regreso de la interrupción
}
int main(void) // Programa principal
{
	Ports_setup(); // Llamado de las funciones
	ADC_setup(); // Llamado de las funciones
	sei(); //Activa las interrupciones de forma global.
	while(1)
	{
		PORTD = adc_valor/4; // Envía el valor del ADC (8 bits) por el puerto D
		//TODO:: Please write your application code
	}
}
