/*
 * P02_control_motor.c
 *
 * Created: 17/01/2017 05:20:14 p. m.
 * Author : a
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void conf_puertos(){
	DDRB = 0b000100;//configuramos pb2 Como salida (OC1B) PWM de fase correctiva
	DDRC = 0x03;//configuramos PC0 y PC1 de salida al driver l298 para el giro y paro del motor 
	DDRD = 0x00;//configuramos el puerto D de entrada para las interrupciones pinchange
	PORTC = 0x00;//limpiamos puerto c
}

void conf_timer1_PWM_Fase_corr(){
	TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(1<<WGM11)|(0<<WGM10);//configuramos PWM fase correctiva pr OC1B con top en icr1
	TCCR1B = (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(0<<CS10);//
	TCCR1C = 0x00;//registros para forsar salida por canal a o b
	TCNT1 = 0;//inicializar timer1 a cero
	ICR1 = 246; //TOP
	OCR1A = 0x00;// controla el ciclo util el en pwm
	OCR1B = 0x00;//Inicializamos a 0 el ciclo util
	TIMSK1 = 0x00; //no utilizaremos interrupcion
	TIFR1 = 0x00;
}

void start_timer1(){
	TCCR1B = (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);//iniciamos timer con un preescalador de 1
}

void conf_interrupciones_externas(){
	PCICR = (1<<PCIE2)|(0<<PCIE1)|(0<<PCIE0);//habilitamos las interrupciones pinchange de [23:16]
	PCIFR = 0x00; //limpiamos banderas de las interrupciones
	PCMSK2 = 0xF8;//activamos las interrupciones de la pinchange de [23:16]
	PCMSK1 = 0x00;//No habilitamos interrupciones pinchange
	PCMSK0	= 0x00; //No habilitamos interrupciones pinchange
}

ISR(PCINT2_vect,ISR_NAKED){
	
	if((PIND & (1<<PIND7))==0)//izquierda
	{
		PORTC = 0x01;
	}
	
	if((PIND & (1<<PIND6))==0)//derecha
	{
		PORTC = 0x02;
	}
	if((PIND & (1<<PIND5))==0)//stop
	{
		PORTC = 0x03;
	}
	
	if((PIND & (1<<PIND4))==0)//aumenta
	{
		if (OCR1B==240)//Si ORCB1 es igual a 240
		{
			OCR1B = 240; //no incrementamos
		} 
		else //de lo contrario
		{
			OCR1B += 24;//si no incrementamos
		}
		
	}
	
	if((PIND & (1<<PIND3))==0)//disminuye
	{
		if (OCR1B==0)//si orcb1 igual a 0
		{
			OCR1B = 0; //se queda en o
		} 
		else //de lo contrario
		{
			OCR1B -= 24 ;//decrementa
		}
	
	}
	reti();
}


int main(void)
{
	conf_puertos();
	conf_timer1_PWM_Fase_corr();
	conf_interrupciones_externas();
	start_timer1();
	sei();//habilitamos interrupciones de forma global
	//OCR1B = 200;
    /* Replace with your application code */
    while (1) 
    {
    }
}

