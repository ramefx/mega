/*
 * Xmega_01.c
 *
 * Created: 16/10/2017 11:52:12 a. m.
 * Author : a
 */ 

#define F_CPU 29491200 //frecuencia de cristal con el PLL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void conf_relosj()
{
	unsigned char n, s;
	s=SREG;//leemos el estado de  las banderas del microcontrolador
	cli(); //deshabilidtamos las interrupciones
	OSC.XOSCCTRL=OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	//ACTIVAMOS EL OSCILADOR /CRISTAL EXTERNO
	OSC.CTRL|=OSC_XOSCEN_bm;
	//eSPERAMOS QUE SE ESTABILISE EL crista/oscilador
	while((OSC.STATUS & OSC_XOSCRDY_bm)==0);
	//configurando el PLL
	n=(CLK.PSCTRL & (~(OSC_PLLSRC_gm|OSC_PLLDIV_bm|OSC_PLLFAC_gm)))|OSC_PLLSRC_XOSC_gc|(0<<OSC_PLLDIV_bp)|2;
	CCP=CCP_IOREG_gc;//habilitando escritura en registro
	OSC.PLLCTRL=n;
	OSC.CTRL|=OSC_PLLEN_bm;//habilitamos el PLL
	
	//Configuramos los prescaladores (divisores)
	n=(CLK.PSCTRL&(~(CLK_PSADIV_gm|CLK_PSADIV1_bp|CLK_PSADIV0_bm)))|CLK_PSADIV_1_gc|CLK_PSBCDIV_1_1_gc;
	CCP=CCP_IOREG_gc;
	CLK.CTRL=n;
	//Esperamos estabilizacion PLL
	while ((OSC.STATUS & OSC_PLLRDY_bm)==0);
	
	//Seleccionamos el tipo de duente de reloj(PLL) Phase Look Loop
	n=(CLK.CTRL & (~CLK_SCLKSEL_gm))|CLK_SCLKSEL_PLL_gc;
	CCP=CCP_IOREG_gc;
	CLK.CTRL=n;
	
	//Deshabilitar los osciladores no utilizados como el de 2Hz, 32 MHz internos
	OSC.CTRL&=~(OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_RC32KEN_bm);
	
	//Deshabilitamos CKLPER salida de senal de cristal por el pin T0 o T1 del microcontrolador
	PORTCFG.CLKEVOUT &=~(PORTCFG_CLKOUTSEL_gm|PORTCFG_CLKOUT_gm);
	//Reescribimos el estado inicial de las banderas
	SREG=s;
	
}

void conf_puertos()
{
	PORTA.DIR=0xFF;//configurar salidad
	PORTA.OUT=0x00;//limpiamos puerto con ceros
	//configuramos el puerto A con salidas totempole
	PORTA.PIN0CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN1CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN2CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN3CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN4CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN5CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN6CTRL=PORT_OPC_TOTEM_gc;
	PORTA.PIN7CTRL=PORT_OPC_TOTEM_gc;
	
	//Interrupciones no habilitadas (tiene prioridad)
	//no utilizamos la prioridad de las interrupciones
	PORTA.INTCTRL = (PORTA.INTCTRL&(~(PORT_INT1LVL_gm|PORT_INT0LVL_gm)))|PORT_INT1LVL_OFF_gc|PORT_INT0LVL_OFF_gc;
	
	PORTA.INT0MASK=0x00;
	PORTA.INT1MASK=0x00;
}


int main(void)
{
	unsigned char n;
	//Vamos a asegurarnos que las interrupciones 
	n=(PMIC.CTRL&(~(PMIC_RREN_bm|PMIC_IVSEL_bm|PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm)));
	CCP=CCP_IOREG_gc;
	PMIC.CTRL=n;
	PMIC.INTPRI=0x00;
	
	conf_relosj();
	conf_puertos();
	
	
    /* Replace with your application code */
    while (1) 
    {
		//PORTA.OUT=0xFF;
		PORTA.OUT=PORTA.OUT+1;
		_delay_ms(300);
    }
}

