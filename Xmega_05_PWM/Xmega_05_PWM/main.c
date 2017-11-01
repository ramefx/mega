/*
 * Xmega_05_PWM.c
 *
 * Created: 31/10/2017 11:29:00 a. m.
 * Author : a
 */ 

/*
 * A1.c
 *
 * Created: 17/10/2017 09:27:42 a.m.
 * Author : SolaceLabsHardware
 */ 

#define F_CPU 29491200
#include <avr/io.h>
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
void portD_init(void)
{
	PORTD.OUT=0x00;
	PORTD.DIR=0x0F;
	PORTD.PIN0CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN1CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN2CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN3CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN4CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN5CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN6CTRL=PORT_OPC_TOTEM_gc;
	PORTD.PIN7CTRL=PORT_OPC_TOTEM_gc;
	PORTD.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	PORTD.INT0MASK=0x00;
	PORTD.INT1MASK=0x00;

}

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
}
int main(void)
{
	system_clocks_init();

	ports_init();
	portD_init();
	T0_init();
	T0D_init();
	
	while (1)
	{

	}
}



