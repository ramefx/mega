/*
 * P03_1_LedMatrix.c
 *
 * Created: 07/02/2017 11:02:28 p. m.
 * Author : a
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

int numero[1][5]={0x00,0x00,0x14,0x10,0x10};
int contador=0;
int i,j;

int main(void)
{
	DDRD=0xFF;
	DDRC=0xFF;
	PORTC=0x10;
	while(1){
		for(j=0;j<10000000000000;j++){ //Ciclo de numero de barridos
			for(i=0;i<5;i++){ //Ciclo de barrido de columnas
				PORTD=numero[contador][i]; //Se le asigna al PORTD el código respecto a la columna
				_delay_ms(1);
				PORTC=PORTC>>1; //Siguiente columna
			}
			PORTC=0x10; //Se inicializa a las primera columna
		}
		contador++; //Incrementar en 1 el contador
		if (contador==10) //Si el contador llega a 10
		contador=0; //que vuelva a ser 0
	}
}
