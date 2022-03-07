/*
 * File:   Periferico I2C.c
 * Author: José Sanoli
 *
 * Created on 7 de marzo de 2022, 9:51
 */


//---------------------Bits de configuración-------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//-----------------Librerías utilizadas en código-------------------- 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------Librearías creadas por usuario--------------------
#include "I2C.h"
#include "Oscilador.h"

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 8000000

//-----------------------Constantes----------------------------------
#define PIC_esclavo 0x50                            // Constante que lleva la dirección del PIC esclavo

//-----------------------Variables------------------------------------

//Variables para giroscopio
char Dato_recibido = 0;
char semaforo = 0;
char sema_estado = 0;

//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Función de setup
char tabla_numASCII(char a);                        // Función para pasar un caracter a su equivalente en ASCII
void divisor_dec(int b, char dig1[]);               // Función para dividir valores en sus dígitos decimales

//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    if(PIR1bits.SSPIF   == 1){
        SSPCONbits.CKP  = 0;
        
        // Habilita comunicacion
        if((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            Dato_recibido = SSPBUF;                 // Limpiar buffer
            SSPCONbits.SSPOV = 0;                   // Sin overflow
            SSPCONbits.WCOL = 0;                    // Sin colision
            SSPCONbits.CKP = 1;                     // Habilita SCL
        }
        
        // Recibe
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            __delay_us(7);
            Dato_recibido = SSPBUF;                 // Limpiar buffer
            __delay_us(2);
            PIR1bits.SSPIF = 0;                     // Limpia bandera 
            SSPCONbits.CKP = 1;                     // Habilita SCL
            while(!SSPSTATbits.BF);                 // Esperar a que se realice toda la recepción
            semaforo = SSPBUF;                      // semaforo = led a prender en el semáforo
            __delay_us(250);
        }
        
        // Envia
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            Dato_recibido = SSPBUF;                 // Limpiar buffer
            SSPSTATbits.BF = 0;                     // SSPUF vacio y bandera limpia
            SSPBUF = sema_estado;         
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;    
        
    }
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();                                        // Contiene todas las subrutinas de configuración
    while(1){
        if(semaforo == 1){
            PORTCbits.RC7 = 1;
            PORTDbits.RD4 = 0;
            PORTDbits.RD5 = 0;
            sema_estado = 1;
        }
        else if(semaforo == 2){
            PORTDbits.RD4 = 1;
            PORTCbits.RC7 = 0;
            PORTDbits.RD5 = 0;
            sema_estado = 2;
        }
        else if(semaforo == 3){
            PORTDbits.RD4 = 0;
            PORTCbits.RC7 = 0;
            PORTDbits.RD5 = 1;
            sema_estado = 3;
        }
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0;                                      // Pines digitales
    ANSELH = 0;
    
    TRISA = 0;                                      // PORTA como salida
    TRISB = 0;                                      // PORTB como salida
    TRISD = 0;
    TRISC = 0;
    
    PORTA = 0;                                      // Limpiar PORTA
    PORTB = 0;                                      // Limpiar PORTB
    PORTD = 0;
    PORTC = 0;
    
    //Configuración de oscilador
    initOsc(_4MHz);                                 // Oscilador a 8 mega hertz
    
    // Inicialización de módulo MSSP = I2C (Master)
    // Frecuencia de Transmisión: 100 kHz
    InitMSSP(I2C_SLAVE_7BIT, PIC_esclavo);
    
}

char tabla_numASCII(char a){
    switch(a){
        case 0:                                     // Si a = 0
            return 48;                              // devolver valor 48 (0 en ASCII)
            break;
        case 1:                                     // Si a = 1
            return 49;                              // devolver valor 49 (1 en ASCII)
            break;
        case 2:                                     // Si a = 2
            return 50;                              // devolver valor 50 (2 en ASCII)
            break;
        case 3:                                     // Si a = 3
            return 51;                              // devolver valor 51 (3 en ASCII)
            break;
        case 4:                                     // Si a = 4
            return 52;                              // devolver valor 52 (4 en ASCII)
            break;
        case 5:                                     // Si a = 5
            return 53;                              // devolver valor 53 (5 en ASCII)
            break;
        case 6:                                     // Si a = 6
            return 54;                              // devolver valor 54 (6 en ASCII)
            break;
        case 7:                                     // Si a = 7
            return 55;                              // devolver valor 55 (7 en ASCII)
            break;
        case 8:                                     // Si a = 8
            return 56;                              // devolver valor 56 (8 en ASCII)
            break;
        case 9:                                     // Si a = 9
            return 57;                              // devolver valor 57 (9 en ASCII)
            break;
        default:
            break;
    }
}

