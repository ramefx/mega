/*
 * Xmega_USARD0.c
 *
 * Created: 01/11/2017 10:49:28 a. m.
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
	
	PORTD.OUT=0x08;//Colocamos 
	PORTD.DIR=0x08;//tx salida
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
}
	
// USARTD0 initialization
void usartd0_init(void)
{

	PORTD.OUTSET=0x08;//el pin tx debe estar en alto
	
	//asicncrono
	//deshabilitamos paridad
	//utilizamos 8 bits de datos
	USARTD0.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;

	//habilitamos las interrupciones de recepcion de bajo nivel
	USARTD0.CTRLA=(USARTD0.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
	USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;
	
	//configuramos el baud rate a 9600bps
	USARTD0.BAUDCTRLA=0xF0;
	USARTD0.BAUDCTRLB=((0x0C << USART_BSCALE_gp) & USART_BSCALE_gm) | 0x0B;
	
	//habilitamos el receptor, el transmisor y no utilizamos comunicacion multiprocesador ni doblamos la velocidad
	USARTD0.CTRLB=(USARTD0.CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
	USART_RXEN_bm | USART_TXEN_bm;
}


ISR(USARTD0_RXC_vect,ISR_NAKED)
{
	char data;
	data=USARTD0.DATA;
	
	switch (data)
	{
		case 'a': PORTA.OUT=0x01; break; 
		case 'b': PORTA.OUT=0x02; break; 
		case 'c': PORTA.OUT=0x04; break;
		case 'd': PORTA.OUT=0x08; break;
		case 'e': PORTA.OUT=0x10; break;
		case 'f': PORTA.OUT=0x20; break;
		case 'g': PORTA.OUT=0x40; break;
		case 'h': PORTA.OUT=0x80; break;
		default: PORTA.OUT=0x00; break; 
	}
	
	reti();
}


void putchar_usartd0(char c)//funcion para enviar datos seriales
{
	while ((USARTD0.STATUS & USART_DREIF_bm) == 0);
	USARTD0.DATA=c;
}


int main(void)
{
	unsigned char n;

	n=(PMIC.CTRL & (~(PMIC_RREN_bm | PMIC_IVSEL_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm))) |	PMIC_LOLVLEN_bm;
	CCP=CCP_IOREG_gc;
	PMIC.CTRL=n;
	
	PMIC.INTPRI=0x00;
	
	system_clocks_init();//habilitamos los relojes dek mcu

	ports_init(); //inicializamos los puertos

	usartd0_init();//inicializamos puerto serial
	
	sei();
	
	while (1)
	{
		putchar_usartd0('a');//enviamos el caracter 'a' cada 300ms
		_delay_ms(300);
	}
}

