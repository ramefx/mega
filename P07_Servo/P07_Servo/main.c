/*
 * P07_Servo.c
 *
 * Created: 14/12/2016 06:43:39 p. m.
 * Author : a
 */ 
#define F_CPU 14745600
#include <avr/io.h>
#include <util/delay.h>
void conf_puertos();//declaracion de prototipo
void conf_Timer1();//funcion prototipo de timer
void start_Timer1();

void conf_puertos(){
	DDRB=0x04;//configurar OC1B como salida
}

void conf_Timer1(){
	TCCR1A=(0<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(1<<WGM11)|(0<<WGM10);//configuracion el OC1B	como salida
	TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(0<<CS10);//Configuramos fast PWM timer detenido con los prescalers en 0
	TCCR1C=(0<<FOC1A)|(0<<FOC1B);//no forzamos ninguna salida, porque estamos utilizando PWM
	TCNT1=0x00;//inicimos el timer a cero
	ICR1=36863;//TOP con prescaler de 8 N=8
	OCR1A=0;//no estamos utilizando el
	OCR1B=3870;//iniciando PWM con ciclo de trabajo a 90°
	TIMSK1=(0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(0<<TOIE1);
	TIFR1=(0<<ICF1)|(0<<OCF1B)|(0<<OCF1A)|(0<<TOV1);
}

void start_Timer1(){
	TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
}

int main(void)
{
	conf_puertos();
	conf_Timer1();
	start_Timer1();
	_delay_ms(2000);
	OCR1B=2396;//ciclo de trabajo
	_delay_ms(2000);
	OCR1B=1100;
	/* Replace with your application code */
	while (1)
	{
	}
}

