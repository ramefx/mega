/*
 * Xmega_03_io.c
 *
 * Created: 17/10/2017 12:02:00 p. m.
 * Author : a
 */ 

#define F_CPU 29491200
#include <avr/io.h>
#include <util/delay.h>


// System Clocks initialization
void system_clocks_init(void)
{
	unsigned char n,s;

	s=SREG;//guarda la configuracion inicial del estado de la habilitacion de las interrupciones
	OSC.XOSCCTRL=OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;//seleccionar un cristal externo de 12 a 16 MHz
	OSC.CTRL|=OSC_XOSCEN_bm;//startup time habilita EL USo DE UN OSCILADOR externo

	while ((OSC.STATUS & OSC_XOSCRDY_bm)==0);//espera hata que sintoniza el oscilador
	
	//configuracion de multiplicadores PLL x2
	n=(OSC.PLLCTRL & (~(OSC_PLLSRC_gm | OSC_PLLDIV_bm | OSC_PLLFAC_gm))) |
	OSC_PLLSRC_XOSC_gc | (0<<OSC_PLLDIV_bp) | 2;
	CCP=CCP_IOREG_gc;
	OSC.PLLCTRL=n;

	OSC.CTRL|=OSC_PLLEN_bm;//habilitamos el PLL
	
	//configuramos el factor de division de 1 para CKLPER
	n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
	CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;//divisores de 1
	CCP=CCP_IOREG_gc;
	CLK.PSCTRL=n;

	while ((OSC.STATUS & OSC_PLLRDY_bm)==0);//esperamos que se estabilice PLL
	
	//conf el uso de PLL
	n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_PLL_gc;
	CCP=CCP_IOREG_gc;
	CLK.CTRL=n;
	
	//deshabilitar oscilador interno rc 2kHz 32kHz 32MHz
	OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_RC32KEN_bm);
	
	//salidas de freccuencia
	PORTCFG.CLKEVOUT&= ~(PORTCFG_CLKOUTSEL_gm | PORTCFG_CLKOUT_gm);
	
	//restauramoe el estado inicial
	SREG=s;

}

void ports_init(void)
{
	PORTA.OUT=0x00;//colocamos la salida del puerto A en 00
	
	PORTA.DIR=0x00; //Conf entradas 
	
	//conf individual de los pines
	PORTA.PIN0CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN1CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN2CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;
	
	PORTA.PIN3CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN4CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN5CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN6CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN7CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;
	//Interrupciones
	PORTA.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;

	PORTA.INT0MASK=0x00;

	PORTA.INT1MASK=0x00;

}
void conf_puertoB()
{
	PORTB.OUT=0x00;//colocamos la salida del puerto A en 00
	
	PORTB.DIR=0xFF; //configuracion salidaEscribimos unos en el puerto a
	
	//conf individual de los pines
	PORTB.PIN0CTRL=PORT_OPC_TOTEM_gc;

	PORTB.PIN1CTRL=PORT_OPC_TOTEM_gc;

	PORTB.PIN2CTRL=PORT_OPC_TOTEM_gc;
	
	PORTB.PIN3CTRL=PORT_OPC_TOTEM_gc;

	PORTB.PIN4CTRL=PORT_OPC_TOTEM_gc;

	PORTB.PIN5CTRL=PORT_OPC_TOTEM_gc;

	PORTB.PIN6CTRL=PORT_OPC_TOTEM_gc;

	PORTB.PIN7CTRL=PORT_OPC_TOTEM_gc;
	//Interrupciones
	PORTB.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;

	PORTB.INT0MASK=0x00;

	PORTB.INT1MASK=0x00;
}
int main(void)
{
	system_clocks_init();

	ports_init();
	
	conf_puertoB();
	
	//PORTA.OUT=0x01;
	//_delay_ms(300);
	while (1)
	{
		
		PORTB.OUT=PORTA.IN;
		/*_delay_ms(300);
		if (PORTA.OUT==128)
		{
			PORTA.OUT=0x00;
		}*/
	}
}
