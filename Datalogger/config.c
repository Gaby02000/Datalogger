/**********************************************************************
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Ricardo López     03/11/2011  1ra. versión
* Sebastián Wahler  08/05/2013  2da. versión
* Marcelo Gómez     09/03/2014  Portado a MPLAB.X
* Sebastián Wahler  04/06/2019  Simplificado
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/
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
