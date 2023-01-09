#include "p33FJ256GP710.h"
#include "config.h"
#include "common.h"
//UART
#define FCY 40000000
#define BAUDRATE 9600
#define BRGVAL ((FCY / BAUDRATE) / 16) -1
#define COMANDO 32
extern int estado;
char aux;
char paquete[COMANDO];
int k=0;
char marco;
char NACK[8]={0xFE,0x08,0x02,0x03,0x80,0x47,0x80,0x52};
int aux1=0;
int cont1=0;
char comando=0; 
char dest;
extern char bcch;
extern char bccl;
extern int cantVehiculos;
extern int paqueteInvalido;
extern int contEjes;
extern int velocidades;
extern volatile unsigned char hours;
extern volatile unsigned char minutes;
extern volatile unsigned char seconds;


/*---------------------------------------------------------------------
  Function Name: UART2Interrupt
  Description:   UART2 Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/

typedef struct {
    unsigned int ejes;
    unsigned int velocidad;
    unsigned int horas;
    unsigned int minutos;
    unsigned int segundo;
}tVehiculos;
tVehiculos datalogger[MAX];

void informacion(void){   //guardo la informacion en un registro
    datalogger[cantVehiculos].ejes=contEjes;
    datalogger[cantVehiculos].horas=hours;
    datalogger[cantVehiculos].minutos=minutes;
    datalogger[cantVehiculos].segundo=seconds;
    datalogger[cantVehiculos].velocidad=velocidades;
    cantVehiculos++;
}

void checkSum(void){ //hace el calculo del check sum
    int i=0;
    char var;
    char acum;
    bcch = paquete[i];
    i++;
    for (i=1;i<marco;i++){
        bcch = paquete[i];
        i++;
        if((marco-2) != i)
            bccl = paquete[i++];
        else
            bccl = 0x00;
        var = (bcch << 8) + bccl;
        acum += var;
    }
    bcch=acum>>8;
    bccl=acum & 0x00FF; 
}
void asignoNack(void){   //pega el paquete NACK a un paquete generico
    int i;
    for(i=0;i<8;i++){
        paquete[i]=NACK[i];
        
    }
    paqueteInvalido=0;  //bajo bandera de paquete invalido
    estado=2;
    IFS1bits.U2TXIF = 1; //fuerzo tx
}
void paqueteA(void){   
    paquete[1]=0x09;
    paquete[2]=0x02;
    paquete[3]=0x03;
    checkSum();  //modifica checksum
    paquete[6]=cantVehiculos;  //asigna la cantidad de vehiculos pasados hasta el momento
    paquete[7]=bcch;
    paquete[8]=bccl; 
    IFS1bits.U2TXIF = 1; //fuerzo tx
    
}
void paqueteB(void){
    paquete[2]=0x02;
    paquete[3]=0x03;
    checkSum();  //modifica checksum
    paquete[6]=bcch;
    paquete[7]=bccl; 
    cantVehiculos=0;   //resetea la cantidad de vehiculos, de esta manera los datos nuevos van a sobreescribir los viejos
    IFS1bits.U2TXIF = 1; //fuerzo tx
}
void paqueteC(void){
    int i=0;
    int aux=0;
    paquete[1]=0x09;
    paquete[2]=0x02;
    paquete[3]=0x03;
    checkSum();  //modifica checksum
    while (i<cantVehiculos){   //cuenta la cantidad de vehiculos con mas de 2 ejes
        if(datalogger[cantVehiculos].ejes>2){
            aux=datalogger[cantVehiculos].ejes;
        }    
    }
    paquete[6]=aux;
    paquete[7]=bcch;
    paquete[8]=bccl; 
    IFS1bits.U2TXIF = 1; //fuerzo tx
}
void paqueteD(void){
    int i;
    int varAux;
    int L;
    
    paquete[3]=0x03;
    varAux=paquete[6];
    L=6;
    for(i=0;i<cantVehiculos;i++){  //cuenta la cantidad de vehiculos en una hora determinada
        if(datalogger[i].horas==varAux){
            paquete[L]=datalogger[i].horas;
            L++;
            paquete[L]=datalogger[i].minutos;
            L++;
            paquete[L]=datalogger[i].segundo;
            L++;
            paquete[L]=datalogger[i].velocidad;
            L++;
            paquete[L]=datalogger[i].ejes;
            L++;
            marco=marco+5;
        }
        
    }
    checkSum();   //modifica checksum
    paquete[L+1]=bcch;
    paquete[L+2]=bccl; 
    
   IFS1bits.U2TXIF = 1;  //fuerzo tx
    
}
void paqueteE(void){
    PORTAbits.RA0=1; //prendo camara
    PORTAbits.RA0=0; //apago camara
}


void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void )
{
	IFS1bits.U2RXIF = 0;
    if(estado==0){ //unicamente recibe cuando el estado esta en 0
        if (cont1==0){
            aux=U2RXREG;
        
    }
        if (aux==0xFE)  {   //si el paquete comienza con un FE entra
            aux1=U2RXREG;
            paquete[k]=aux1;  //asigna los caracteres al arreglo paquete
            if (cont1==1){
                marco=U2RXREG;   //guarda el marco
        } 
            if(cont1==2){
                dest=U2RXREG;  //guarda el destinatario
        }
            if (cont1==5){
                comando=U2RXREG;   //guarda el comando
        }
            
            if(marco==k) {   //una vez haya asigando todos los valores se pone estados en 1,haciendo que no siga recibiendo
                estado=1;
                k=-1;
                cont1=-1;
        }
            k++;
            cont1++;
        }else{       //si no es FE se levanta la bandera de paquete invalido
        paqueteInvalido=1;
    }    
}
}           

//Rutina de INT para transmisión (no hace nada)
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
    if(estado==2) {    //transmite el paquete 
        int g=0;
        U2TXREG=paquete[g];
        g++; 
        if(g==marco){
            estado=0;   //se pone estado en 0 una vez haya terminado de asignar 
        }
    }    
}
   
         


/*---------------------------------------------------------------------
  Function Name: InitUART2
  Description:   Inicializar UART2
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void InitUART2(void)
{
	// The HPC16 board has a DB9 connector wired to UART2, 
	// so we will be configuring this port only
	// configure U2MODE
	U2MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
	U2MODEbits.RTSMD = 1;	// Bit11 Simplex Mode

	// Load a value into Baud Rate Generator.  Example is for 9600.
	U2BRG = BRGVAL;	// 40Mhz osc, 9600 Baud

	IPC7 = 0x4400;	// Mid Range Interrupt Priority level, no urgent reason

	IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts

	U2MODEbits.UARTEN = 1;	// And turn the peripheral on
	U2STAbits.UTXEN = 1;	// Empieza a transmitir. Se dispara el Flag TXIF

	IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
}
