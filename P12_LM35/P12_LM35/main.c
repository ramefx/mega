/*
 * P12_LM35.c
 *
 * Created: 28/09/2017 01:59:01 p. m.
 * Author : a
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

void conf_puertos(){
	
	DDRD = 0xFF; //salidas 0b11111111
	DDRB = 0xFF; //salidas en uno 0b101000
	DDRC = 0x00;//configuracion de entrada
	
}

void conf_adc(){
	//------|referencia 1.1 vref |ladoiz-der|sedeccionar canal de adc--------ADC1--|
	ADMUX = (1<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0);
	//------habilitmaos analogio ------------------------------|precalador de 128 
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(0<<ADIF)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//habilitamos interrupcion de adc habilitamos prescaler 128
	/*configurar fuente de disparo--------- */
	ADCSRB = (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);//ACME coparador analogico registro no se usa
	
	DIDR0 = (0<<ADC5D)|(0<<ADC4D)|(0<<ADC3D)|(0<<ADC2D)|(0<<ADC1D)|(0<<ADC0D);
	
}

ISR(ADC_vect,ISR_NAKED){
	//PORTD = ADCL;
	//PORTB = ADCH;
	if (ADC>=251 && ADC<344)
	{
		PORTD=0b001000;
	} 
	
	else if (ADC>=344)
	{
		PORTD=0b100000;
	}
	else
	{
		PORTB=0b000000;
	}
	/*if
	(condición) { extracto; }
	else if
	(condición) { extracto; }
	else{ extracto; }*/
	//PORTD = (ADC & 0xFF);//los 8 bits en la parte baja o menos significativa (ADC & 0xff)descarta los 2 bits mas significativos
	//PORTB = ADC>>8;//los 2 bits en la parte alta o mas significativa [mover los mas significaitivos a menos sig con corrimieto a la dercha (ADC>>8)
	reti();
	
}
int main(void)
{
	conf_puertos();
	conf_adc();
	sei();
    /* Replace with your application code */
    while (1) 
    {
    }
}

