#include "p33FJ256GP710.h"
#include "config.h"

#define ValPR4	39062
#define ValPR6 31250

int velocidades;
int cantVehiculos=0;
int contEjes=0;
float tiempo;
extern int counterTog;


/*---------------------------------------------------------------------
  Function Name: _T4Interrupt
  Description:   Timer4 Interrupt Handler

Comments: 39062
-----------------------------------------------------------------------*/
void __attribute__((interrupt, auto_psv)) _T4Interrupt( void )
{
	/* reset Timer 4 interrupt flag */
 	IFS1bits.T4IF = 0;
	
	
}

void __attribute__((interrupt, auto_psv)) _T6Interrupt( void )
{
    /* reset Timer 6 interrupt flag */
    IFS2bits.T6IF=0;
      
    counterTog++;
}

void __attribute__((interrupt, auto_psv)) _CNInterrupt( void ) {
 IFS1bits.CNIF = 0;
 if (PORTDbits.RD13==1){
     if (contEjes==0){      //unicamente la rueda delantera va a entrar
        if (PORTDbits.RD6==1){           
            T4CONbits.TON=1;    
       
        }
        if (PORTDbits.RD7==1){
            T4CONbits.TON = 0; 
            tiempo=(1*8*TMR4)/1000;          
            velocidades=((300/tiempo)*18)/5;      //calculo de kilometraje
           
    }
    if (velocidades>60){
        //se acciona la camara del bit 0 del PORTA
        PORTAbits.RA0=1;
        PORTAbits.RA0=0;
    } 
     }
    contEjes++;
 }
 else{
 informacion();
 contEjes=0;
 }
 
}

/*---------------------------------------------------------------------
  Function Name: Init_Timer4
  Description:   Initialize Timer4

-----------------------------------------------------------------------*/
void init_CNI (void){  //configuracion del CN

CNEN1bits.CN15IE = 1;
CNEN2bits.CN16IE = 1;
CNEN2bits.CN19IE = 1;
IEC1bits.CNIE = 1;  //se prende la interrupcion por CN
}


void Init_Timer4( void )
{
	/* ensure Timer 4 is in reset state */
	T4CON = 0;
	T4CONbits.TCKPS = 1; //Prescaler 8

	/* reset Timer 4 interrupt flag */
 	IFS1bits.T4IF = 0;
 	
 	/* set Timer interrupt priority level */
	IPC6bits.T4IP = 5;

	/* enable Timer interrupt */
 	IEC1bits.T4IE = 1;
 	  	
	/* set Timer period register */
	PR4 = ValPR4;
	
   
}

void Init_Timer6( void )
{
    
    /* ensure Timer 6 is in reset state */
    T6CON = 0;
    T6CONbits.TCKPS = 1; //Prescaler 8

    /* reset Timer 6 interrupt flag */
     IFS2bits.T6IF = 0;
     
     /* set Timer interrupt priority level */
    IPC11bits.T6IP = 5;

    /* enable Timer interrupt */
     IEC2bits.T6IE = 1;
           
    /* set Timer period register */
    PR6 = ValPR6;
    T6CONbits.TON = 1;     //habilito Timer

}