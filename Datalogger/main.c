
#include "common.h"
#include <xc.h>
#include "confbits.h"
#include "p33FJ256GP710.h"
#include "config.h"
#define TRUE 1
char bcch;
char bccl;
extern int cantVehiculos;
extern int contEjes;
extern char marco;

int paqueteInvalido;
extern char comando;
extern char dest;
int estado=0;
int counterTog;
extern int velocidades;
volatile unsigned char hours;
volatile unsigned char minutes;
volatile unsigned char seconds;


void UpdateClock (void)
{
   
    
    seconds=seconds+1;
    if (seconds > 59)
    {
        seconds=0;
        minutes=minutes+1;
    }
    if (minutes > 59)
    {
        minutes=0;
        hours=hours+1;
    }
    if (hours == 24)
    {
        hours=0;
    }

}

void chequeoPaquete(void){    //selecciona el tipo de comando recibido
    switch(comando){
        case 0x41 : //A
            paqueteA();  
            break;
        case 0x42 : //B
            paqueteB();   
            break;
        case 0x43 : //C
            paqueteC();   
            break;
        case 0x44 : //D
            paqueteD();
            break;
        case 0x45 : //E
            paqueteE();
            break;
    }
}

int main ( void )
{
    config();
    while (1)
    {
       if (counterTog > 3) {
        UpdateClock();  //Actualizar hora del dispositivo
       }
        if (paqueteInvalido==1){ //entra cuando la bandera de paquete invalido este arriba
            asignoNack();
        }
        if(estado==1){  //entra una vez haya llegado el paquete en condiciones
           if (marco>=8){ 
               estado=2;
               chequeoPaquete();
           }
        }
        if(estado==2){      //entra una vez haya chequeado el paquete
            if(dest==0x03){
                checkSum();
                   
            }
        }    
    }
}


