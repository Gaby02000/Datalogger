/**********************************************************************
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Ricardo L�pez     03/11/2011  1ra. versi�n
* Sebasti�n Wahler  08/05/2013  2da. versi�n
* Marcelo G�mez     09/03/2014  Portado a MPLAB.X
* Sebasti�n Wahler  04/06/2019  Simplificado
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "p33FJ256GP710.h"
#include "config.h"

void config( void )
{
	
	

	//Inicializaci�n de PORTS I/O

	TRISA=0xFF00;
    TRISD=0xFFFF; 
		
	/* Initializar UART2*/
	InitUART2();

	/* Inicializar Timers necesarios */
	Init_Timer4();
    Init_Timer6();
    init_CNI(); //inicializacion del CN
}
