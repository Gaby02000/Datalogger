#include "p33FJ256GP710.h"
#include "config.h"

void config( void )
{
	
	

	//Inicialización de PORTS I/O

	TRISA=0xFF00;
    TRISD=0xFFFF; 
		
	/* Initializar UART2*/
	InitUART2();

	/* Inicializar Timers necesarios */
	Init_Timer4();
    Init_Timer6();
    init_CNI(); //inicializacion del CN
}
