/*
 * P11_Timer_delay.c
 * generar base de tiempo
 * Created: 26/09/2017 12:01:28 p. m.
 * Author : a
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int entrar = 0;
int mili = 0;

void conf_puertos(){
	DDRC=0b00001;
}
	
void conf_timer0(){
	TCCR0A = (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(0<<WGM01)|(0<<WGM00);//timer 0 en modo normal
	TCCR0B = (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(0<<CS01)|(0<<CS00);//mantenemos el timer detenido
	TCNT0 = 0; //inicia el timer en 0
	OCR0A = 200; //auxiliares nos dice cuando entra la interrupocion (a que momento entra)
	OCR0B = 0;
	TIMSK0 = (0<<OCIE0B)|(1<<OCIE0A)|(0<<TOIE0);//HABILITAMOS LA INTERRUPCION COMPARE MATCH A
	TIFR0 = (0<<OCF0B)|(0<<OCF0A)|(0<<TOV0);//reti() modifica TOV0 |banderas de interruocion en 0
	
}

void start_timer0(){
	TCCR0B = (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(0<<CS01)|(1<<CS00);//iniciar sin prescaler 
}

ISR(TIMER0_COMPA_vect,ISR_NAKED){
	TCNT0=0;//iniciamos contador en 0
	entrar++;
	//cada vez entra 200um si entra 5 veces es para tener 1ms
	if(entrar==5){
		entrar=0;
		mili++;//acumula el milisegundo
	}
	reti();//regreso de interrupcion y limpiamos la bandera
}


int main(void)
{
	conf_puertos();
	conf_timer0();
	start_timer0();
	sei(); //habilita de manera global la interrupcion
	
    while (1) 
    {
		if (mili==500)
		{
			PORTC=0b000001;
		}
		
		if (mili==1000)
		{
			PORTC=0b000000;
			mili=0;
		}
    }
}

