/*
 * Xmega_04_interr.c
 *
 * Created: 17/10/2017 12:55:34 p. m.
 * Author : a
 */ 

#define F_CPU 29491200
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


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

void ports_init(void)//entradas e inteerrupcipones
{
	PORTA.OUT=0x00;
	
	PORTA.DIR=0x00;

	PORTA.PIN0CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN1CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN2CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;
	
	PORTA.PIN3CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN4CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN5CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN6CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.PIN7CTRL=PORT_OPC_TOTEM_gc|PORT_ISC_BOTHEDGES_gc;

	PORTA.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_LO_gc | PORT_INT0LVL_LO_gc;

	PORTA.INT0MASK=0x01;

	PORTA.INT1MASK=0x80;

}
void conf_puertoB()//salidas
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
	PORTB.INTCTRL=(PORTB.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;

	PORTB.INT0MASK=0x00;

	PORTB.INT1MASK=0x00;
}

ISR(PORTA_INT0_vect,ISR_NAKED)
{
	if ((PORTA.IN&(1<<0))==0)
	{
		
		PORTB.OUT=0xFF;
		_delay_ms(200);
		PORTB.OUT=0x00;
		_delay_ms(200);
		PORTB.OUT=0xFF;
		_delay_ms(200);
		PORTB.OUT=0x00;
		_delay_ms(200);
		PORTB.OUT=0xFF;
		_delay_ms(200);
		PORTB.OUT=0x00;
		_delay_ms(200);
			
		
		
	}
	
	reti();

}

ISR(PORTA_INT1_vect,ISR_NAKED)
{
	int i = 0;
	if ((PORTA.IN&(1<<7))==0)
	{
		for (i=0;i<+20;i++)
		{
			PORTB.OUT++;
			_delay_ms(200);
		}
		
	}
	reti();

}

int main(void)
{
	unsigned char	n;
	
	system_clocks_init();

	ports_init();
	
	conf_puertoB();
	
	n=(PMIC.CTRL&(~(PMIC_RREN_bm|PMIC_IVSEL_bm|PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm)))|PMIC_LOLVLEN_bm;
	CCP=CCP_IOREG_gc;
	PMIC.CTRL=n;
	
	PMIC.INTPRI=0x00;
	sei();
	
	while (1)
	{

	}
}