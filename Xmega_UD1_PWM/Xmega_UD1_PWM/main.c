/*
 * Xmega_UD1_PWM.c
 *
 * Created: 01/11/2017 12:04:02 p. m.
 * Author : a
 */ 

#define F_CPU 29491200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


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
	// Enable the PLL
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


// Ports initialization
void ports_init(void)
{
	PORTA.OUT=0x00;
	PORTA.DIR=0xFF;//todo de salida
	PORTA.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN7CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTA.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	PORTA.INT0MASK=0x00;
	PORTA.INT1MASK=0x00;
	
	PORTD.OUT=0x80;//Colocamos 
	PORTD.DIR=0x80;//tx salida
	PORTD.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.PIN7CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTD.INTCTRL=(PORTD.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	PORTD.INT0MASK=0x00;
	PORTD.INT1MASK=0x00;
	
	PORTC.OUT=0x00;
	PORTC.DIR=0x0F;
	PORTC.PIN0CTRL=PORT_OPC_TOTEM_gc;
	PORTC.PIN1CTRL=PORT_OPC_TOTEM_gc;
	PORTC.PIN2CTRL=PORT_OPC_TOTEM_gc;
	PORTC.PIN3CTRL=PORT_OPC_TOTEM_gc;
	PORTC.PIN4CTRL=PORT_OPC_TOTEM_gc;
	PORTC.PIN5CTRL=PORT_OPC_TOTEM_gc;
	PORTC.PIN6CTRL=PORT_OPC_TOTEM_gc;
	PORTC.PIN7CTRL=PORT_OPC_TOTEM_gc;
	PORTC.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	PORTC.INT0MASK=0x00;
	PORTC.INT1MASK=0x00;
}

//timer puerto c pwm
void T0_init(){
	unsigned  char n;
	n= SREG;
	TCC0.CTRLA = 0x00; //aseguramos que el timer esta detenido
	TCC0.CTRLFSET =0b1100;//Reset timer
	TCC0.CTRLA = 0b0101;//Selecciona preescaler
	//Habilitamos los canales A, B, C, D y Utilizamos PWM Single Slop = PWM rapido
	TCC0.CTRLB=(1<<TC0_CCDEN_bp)|(1<<TC0_CCCEN_bp)|(1<<TC0_CCBEN_bp)|(1<<TC0_CCAEN_bp)|(0<<TC0_WGMODE2_bp)|(1<<TC0_WGMODE1_bp)|(1<<TC0_WGMODE0_bp);
	TCC0.CTRLD=0x00; //no habiloamos los eventos
	TCC0.CTRLE=0x00;//configuramos el timer para funcionamiento de manera normal
	TCC0.INTCTRLA=0x00;//no utilizamos interrupciones de desbordamiento ni de error
	TCC0.INTCTRLB=0x00; //no utilizamos interrupciones
	HIRESC.CTRLA&=~HIRES_HREN0_bm;//no utilizamos
	TCC0.INTFLAGS=TCC0.INTFLAGS;//limpiamos baderas de interrupciones
	TCC0.CNT=0x00;//inicializamos el contador en ceros
	TCC0.PER=9215;//configuramos nuestro TOP en 38 para una frecuencia de 750kHz
	TCC0.CCA = 184;//2% del ciclo util
	TCC0.CCB = 184;//25
	TCC0.CCC = 184;//
	TCC0.CCD = 184;//
	
	SREG =n;
}

// USARTD1 initialization
void usartd1_init(void)
{

	PORTD.OUTSET=0x80;//el pin tx debe estar en alto
	
	//asicncrono
	//deshabilitamos paridad
	//utilizamos 8 bits de datos
	USARTD1.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;

	//habilitamos las interrupciones de recepcion de bajo nivel
	USARTD1.CTRLA=(USARTD1.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
	USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;
	
	//configuramos el baud rate a 9600bps
	USARTD1.BAUDCTRLA=0xF0;
	USARTD1.BAUDCTRLB=((0x0C << USART_BSCALE_gp) & USART_BSCALE_gm) | 0x0B;
	
	//habilitamos el receptor, el transmisor y no utilizamos comunicacion multiprocesador ni doblamos la velocidad
	USARTD1.CTRLB=(USARTD1.CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
	USART_RXEN_bm | USART_TXEN_bm;
}


ISR(USARTD1_RXC_vect,ISR_NAKED)
{
	char data;
	data=USARTD1.DATA;
	
	switch (data)
	{
		case 'a': TCC0.CCA = 184; break; 
		case 'b': TCC0.CCA = 461; break; 
		case 'c': TCC0.CCA = 922; break;
		/*
		
		*/
		
		default: TCC0.CCA = 0; break; 
	}
	
	switch(data){
		case 'd': TCC0.CCB = 184; break;
		case 'e': TCC0.CCB = 461; break;
		case 'f': TCC0.CCB = 922; break;
		default: TCC0.CCB = 0; break;
		
	}
	
	switch(data){
		case 'g': TCC0.CCC = 184; break;
		case 'h': TCC0.CCC = 461; break;
		case 'i': TCC0.CCC = 922; break;
		default:TCC0.CCC = 0; break;		
	}
	
	switch(data){
		
		case 'j': TCC0.CCD = 184; break;
		case 'k': TCC0.CCD = 461; break;
		case 'l': TCC0.CCD = 922; break;
		default : TCC0.CCD = 0; break;
		
	}
	
	
	reti();
}


void putchar_usartd1(char c)//funcion para enviar datos seriales
{
	while ((USARTD1.STATUS & USART_DREIF_bm) == 0);
	USARTD1.DATA=c;
}


int main(void)
{
	unsigned char n;

	n=(PMIC.CTRL & (~(PMIC_RREN_bm | PMIC_IVSEL_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm))) |	PMIC_LOLVLEN_bm;
	CCP=CCP_IOREG_gc;
	PMIC.CTRL=n;
	
	PMIC.INTPRI=0x00;
	
	T0_init();
	
	
	system_clocks_init();//habilitamos los relojes dek mcu

	ports_init(); //inicializamos los puertos

	usartd1_init();//inicializamos puerto serial
	
	
	sei();
	
	while (1)
	{
		putchar_usartd1('a');//enviamos el caracter 'a' cada 300ms
		_delay_ms(300);
	}
}

