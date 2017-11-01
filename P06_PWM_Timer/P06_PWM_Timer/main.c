/*
 * P06_PWM_Timer.c
 *
 * Created: 14/12/2016 04:57:27 p. m.
 * Author : a
 */ 
//con un cristal de 14745600
#define F_CPU 14745600

#include <avr/io.h>
void conf_puertos();//declaracion de prototipo
void conf_Timer1();//funcion prototipo de timer
void start_Timer1();

void conf_puertos(){
	//DDRB=0x04;//configurar OC1B como salida
	DDRB=0x02; //configurar OC1A COMO SALIDA
}

void conf_Timer1(){
	//----------------------------------------------------/   ICR1 como top      /--------------------------------
	TCCR1A=(1<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(1<<WGM11)|(0<<WGM10);//configuracion el OC1A	como salida
	//TCCR1A=(0<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(1<<WGM11)|(0<<WGM10);//configuracion el OC1B	como salida
	
	//modo 14 hoja de datos---------------------------/    prescaler                 /-----------------------------------------------------
	TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(0<<CS10);//Configuramos fast PWM timer detenido al inicio con los prescalers en 0
	
	TCCR1C=(0<<FOC1A)|(0<<FOC1B);//no forzamos ninguna salida, porque estamos utilizando PWM (son para forzar salidas por canal A oB)
	
	TCNT1=0x00;//inicimos el timer a cero
	
	ICR1=36863;//TOP con prescaler de 8 N=8
	
	OCR1A=25804;//iniciara en 70% de ciclo util |/| estamos utilizando el 50% 18432 |/| varia frecuencia y el cliclo util, salida OC1A -> PB1
	
	OCR1B=0;//se inicia en 0 porque se utilica OCR1A|/|iniciando PWM con ciclo de trabajo al 30%, ciclo util, salida OC1B -> PB2
	
	TIMSK1=(0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(0<<TOIE1);//para habiliar las interrupciones del timer/
	 
	TIFR1=(0<<ICF1)|(0<<OCF1B)|(0<<OCF1A)|(0<<TOV1);//las baderas de las interrupciones TIMSK1
}

void start_Timer1(){
	//Iniciar timer con prescales de 8----------------/           N=8              /---------
	TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
}

int main(void)
{
	conf_puertos();
	conf_Timer1();
	start_Timer1();
    /* Replace with your application code */
    while (1) 
    {
    }
}

