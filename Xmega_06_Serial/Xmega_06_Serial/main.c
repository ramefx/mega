/*
 * Xmega_06_Serial.c
 *
 * Created: 31/10/2017 01:49:00 p. m.
 * Author : a
 */ 


#define F_CPU 29491200
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>//interrupciones

// System Clocks initialization
void system_clocks_init(void)
{
	unsigned char n,s;

	s=SREG;
	OSC.XOSCCTRL=OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL|=OSC_XOSCEN_bm;

	while ((OSC.STATUS & OSC_XOSCRDY_bm)==0);

	n=(OSC.PLLCTRL & (~(OSC_PLLSRC_gm | OSC_PLLDIV_bm | OSC_PLLFAC_gm))) |
	OSC_PLLSRC_XOSC_gc | (0<<OSC_PLLDIV_bp) | 2;
	CCP=CCP_IOREG_gc;
	OSC.PLLCTRL=n;

	OSC.CTRL|=OSC_PLLEN_bm;

	n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
	CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
	CCP=CCP_IOREG_gc;
	CLK.PSCTRL=n;

	while ((OSC.STATUS & OSC_PLLRDY_bm)==0);

	n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_PLL_gc;
	CCP=CCP_IOREG_gc;
	CLK.CTRL=n;

	OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_RC32KEN_bm);

	PORTCFG.CLKEVOUT&= ~(PORTCFG_CLKOUTSEL_gm | PORTCFG_CLKOUT_gm);

	SREG=s;

}


void ports_init(void)
{
	PORTA.OUT=0x00;
	PORTA.DIR=0xFF;//direccionado de salida
	PORTA.PIN0CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN1CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN2CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN3CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN4CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN5CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN6CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN7CTRL=PORT_OPC_TOTEM_gc;
	PORTA.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	PORTA.INT0MASK=0x00;
	PORTA.INT1MASK=0x00;

}
void portD_init(void)
{
	PORTD.OUT=0x00;
	PORTD.DIR=0x08;//configuramos como salida Tx y entrada Rx
	PORTD.PIN0CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN1CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN2CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;//declaramos la entrada
	PORTD.PIN3CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN4CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN5CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN6CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN7CTRL=PORT_OPC_TOTEM_gc;
	PORTD.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	PORTD.INT0MASK=0x00;
	PORTD.INT1MASK=0x00;

}
void USART0_init(){
	PORTD.OUTSET=0x08;//mandando en alto el pin Tx
	//configuramos usartd0 como asincrono sin paridad un bit de stop y 8 bits de datos
	USARTD0.CTRLC=(0<<USART_CMODE1_bp)|(0<<USART_PMODE1_bp)|(0<<USART_PMODE0_bp)|(0<<USART_SBMODE_bp)|(0<<USART_CHSIZE2_bp)|(1<<USART_CHSIZE1_bp)|(1<<USART_CHSIZE0_bp);
	//habilitamos la interruocio de recepcion de datos con prioridad baja 
	USARTD0.CTRLA=(0<<USART_RXCINTLVL1_bp)|(1<<USART_RXCINTLVL0_bp)|(0<<USART_TXCINTLVL1_bp)|(0<<USART_TXCINTLVL0_bp)|(0<<USART_DREINTLVL1_bp)|(0<<USART_DREINTLVL0_bp);
	//configurar el baud rate 9600
	USARTD0.BAUDCTRLA=0xF0;
	USARTD0.BAUDCTRLB=0x0B;
	//habilitamos el receptor Rx el Tx sin multiplicador de frecuencia 2, no utiliamos comunicacion multiprocesador, no utilizar 9 bits de datos
	USARTD0.CTRLB=(1<<USART_RXEN_bp)|(1<<USART_TXEN_bp)|(0<<USART_CLK2X_bp)|(0<<USART_MPCM_bp)|(0<<USART_TXB8_bp);
	
}

ISR(USARTD0_RXC_vect,ISR_NAKED){
	char dato;
	dato=USARTD0.DATA;//guardamos en dato el valor recibido
	
	switch(dato){
	case 'a':PORTA.OUT = 0x01; break;
	case 'e':PORTA.OUT = 0x02; break;
	case '2':PORTA.OUT = 0x04; break;
	default:PORTA.OUT = 0x08; break;
	}
	USARTD0.DATA = dato;
	reti();//regreso de la interrupcion y limpia banderas
}
/*
void T0_init(){
	unsigned  char n;
	n= SREG;
	TCC0.CTRLA = 0x00; //aseguramos que el timer esta detenido
	TCC0.CTRLFSET =0b1100;//Reset timer
	TCC0.CTRLA = 0x01;//Selecciona preescaler
	//Habilitamos los canales A, B, C, D y Utilizamos PWM Single Slop = PWM rapido
	TCC0.CTRLB=(1<<TC0_CCDEN_bp)|(1<<TC0_CCCEN_bp)|(1<<TC0_CCBEN_bp)|(1<<TC0_CCAEN_bp)|(0<<TC0_WGMODE2_bp)|(1<<TC0_WGMODE1_bp)|(1<<TC0_WGMODE0_bp); 
	TCC0.CTRLD=0x00; //no habiloamos los eventos
	TCC0.CTRLE=0x00;//configuramos el timer para funcionamiento de manera normal
	TCC0.INTCTRLA=0x00;//no utilizamos interrupciones de desbordamiento ni de error 
	TCC0.INTCTRLB=0x00; //no utilizamos interrupciones
	HIRESC.CTRLA&=~HIRES_HREN0_bm;//no utilizamos
	TCC0.INTFLAGS=TCC0.INTFLAGS;//limpiamos baderas de interrupciones
	TCC0.CNT=0x00;//inicializamos el contador en ceros
	TCC0.PER=38;//configuramos nuestro TOP en 38 para una frecuencia de 750kHz
	TCC0.CCA = 6;//15% del ciclo util
	TCC0.CCB = 10;//25
	TCC0.CCC = 12;//
	TCC0.CCD = 24;//
	
	SREG =n;
}

void T0D_init(){
	unsigned  char n;
	n= SREG;
	TCD0.CTRLA = 0x00; //aseguramos que el timer esta detenido
	TCD0.CTRLFSET =0b1100;//Reset timer
	TCD0.CTRLA = 0x01;//Selecciona preescaler
	//Habilitamos los canales A, B, C, D y Utilizamos PWM Single Slop = PWM rapido
	TCD0.CTRLB=(1<<TC0_CCDEN_bp)|(1<<TC0_CCCEN_bp)|(1<<TC0_CCBEN_bp)|(1<<TC0_CCAEN_bp)|(0<<TC0_WGMODE2_bp)|(1<<TC0_WGMODE1_bp)|(1<<TC0_WGMODE0_bp);
	TCD0.CTRLD=0x00; //no habiloamos los eventos
	TCD0.CTRLE=0x00;//configuramos el timer para funcionamiento de manera normal
	TCD0.INTCTRLA=0x00;//no utilizamos interrupciones de desbordamiento ni de error
	TCD0.INTCTRLB=0x00; //no utilizamos interrupciones
	HIRESD.CTRLA&=~HIRES_HREN0_bm;//no utilizamos
	TCD0.INTFLAGS=TCC0.INTFLAGS;//limpiamos baderas de interrupciones
	TCD0.CNT=0x00;//inicializamos el contador en ceros
	TCD0.PER=626;//configuramos nuestro TOP en 38 para una frecuencia de 750kHz
	TCD0.CCA = 94;//15% del ciclo util
	TCD0.CCB = 157;//25
	TCD0.CCC = 188;//
	TCC0.CCD = 388;//
	
	SREG =n;
}*/
int main(void)
{
	unsigned char	p;
	//habilitamos interrupciones de bajo nivel
	p=(PMIC.CTRL & (~(PMIC_RREN_bm|PMIC_IVSEL_bm|PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm)))|PMIC_LOLVLEN_bm;
	CCP=CCP_IOREG_gc;
	PMIC.CTRL=p;
	
	PMIC.INTPRI=0x00;
	
	system_clocks_init();
	ports_init();
	portD_init();
	USART0_init();
	sei();
	//T0_init();
	//T0D_init();
	
	while (1)
	{

	}
}





