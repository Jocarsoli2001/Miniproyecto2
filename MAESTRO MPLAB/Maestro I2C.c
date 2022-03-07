/*
 * File:   Maestro I2C.c
 * Author: José Sanoli
 *
 * Created on 28 de febrero de 2022, 16:53
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
#include "LCD.h"
#include "Oscilador.h"
#include "UART.h"
#include "MPU.h"

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 8000000

//-----------------------Constantes----------------------------------
#define PIC_esclavo 0x50                            // Constante que lleva la dirección del PIC esclavo

//-----------------------Variables------------------------------------

//Variables para giroscopio
char Ax1 = 0;                                           // Medición del Acelerómetro sobre eje X (bits menos significativos)
char Ax2 = 0;                                           // Medición del Acelerómetro sobre eje X (bits más significativos)
char Ay1 = 0;                                           // Medición del Acelerómetro sobre eje y (bits menos significativos)
char Ay2 = 0;                                           // Medición del Acelerómetro sobre eje y (bits más significativos)
char Az1 = 0;                                           // Medición del Acelerómetro sobre eje z (bits menos significativos)
char Az2 = 0;                                           // Medición del Acelerómetro sobre eje z (bits más significativos)
char T1 = 0;                                            // Medición del termómetro (bits menos significativos)
char T2 = 0;                                            // Medición del termómetro (bits más significativos)
char Gx1 = 0;                                           // Medición del giroscopio sobre eje X (bits menos significativos)
char Gx2 = 0;                                           // Medición del giroscopio sobre eje X (bits más significativos)
char Gy1 = 0;                                           // Medición del giroscopio sobre eje Y (bits menos significativos)
char Gy2 = 0;                                           // Medición del giroscopio sobre eje Y (bits más significativos)
char Gz1 = 0;                                           // Medición del giroscopio sobre eje Z (bits menos significativos)
char Gz2 = 0;                                           // Medición del giroscopio sobre eje Z (bits más significativos)

//Variables para valores en LCD (eje x)
char Giro_digx[];
char uni_x = 0;
char dec_x = 0;
char cen_x = 0;

//Variables para valores en LCD (eje y)
char Giro_digy[];
char uni_y = 0;
char dec_y = 0;
char cen_y = 0;

//Variables para valores en LCD (eje z)
char Giro_digz[];
char uni_z = 0;
char dec_z = 0;
char cen_z = 0;

//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Función de setup
char tabla_numASCII(char a);                        // Función para pasar un caracter a su equivalente en ASCII
void divisor_dec(int b, char dig1[]);               // Función para dividir valores en sus dígitos decimales

//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();                                        // Contiene todas las subrutinas de configuración
    while(1){
        //---------------------------------------------------------------------- 
        // COMUNICACIÓN I2C CON GIROSCOPIO
        //----------------------------------------------------------------------
        I2C_Master_Start(Standard);
        I2CMasterWrite(IMU+0);                          // Slave: IMU | Operación: Write (+0)
        I2CMasterWrite(ACCEL_XOUT_H);                   // CMD: Selección de registro inicial = GYRO_XOUT_H 
        
        I2C_Master_Start(Repeated);                     // Inicio de lectura continua
        I2CMasterWrite(IMU+1);                          // Slave: IMU | Operación: Read (+1)
        
        // Lectura de giroscopio
        // NOTA: Se realizará una lectura continua de todos los valores que devuelve el IMU. El IMU
        // retorna 7 valores distintos en 14 registros diferentes, por lo que será necesario
        // hacer las lecturas continuas y concatenar valores en 7 diferentes variables. No se estará
        // mandando el bit de acknowledge cada vez que se realiza un lectura, ya que de esa forma
        // el IMU cambiará de registro y en cada lectura se obtiene un valor distinto. El único momento
        // donde se enviará el bit de acknowledge es en la última lectura, ya que acá ya no queremos recibir
        // más valores.
       
        // En este caso se leen los bits más significativos y luego los menos significativos de todo el valor
        Ax1 = I2CMasterRead(ACK); Ax2 = I2CMasterRead(ACK);       
        Ay1 = I2CMasterRead(ACK); Ay2 = I2CMasterRead(ACK);  
        Az1 = I2CMasterRead(ACK); Az2 = I2CMasterRead(ACK); 
        T1 = I2CMasterRead(ACK); T2 = I2CMasterRead(ACK);
        Gx1 = I2CMasterRead(ACK); Gx2 = I2CMasterRead(ACK);       
        Gy1 = I2CMasterRead(ACK); Gy2 = I2CMasterRead(ACK);  
        Gz1 = I2CMasterRead(ACK); Gz2 = I2CMasterRead(NACK);
        
        I2CMasterStop();                                // Fin del "burst read" o lectura continua
        __delay_ms(10);
        
        
        //---------------------------------------------------------------------- 
        // IMPRIMIR VALORES A LCD
        //----------------------------------------------------------------------
        // Divisor de valores en dígitos
        divisor_dec(Gx1,Giro_digx);                      // Divide el valor leído por el osciloscopio en 
        
        // Paso de valores a ASCII            
        uni_x = tabla_numASCII(Giro_digx[0]);
        dec_x = tabla_numASCII(Giro_digx[1]);
        cen_x = tabla_numASCII(Giro_digx[2]);
        
        // Escritura de los valores en LCD
        set_cursor(1,0);
        Escribir_stringLCD("X:    Y:     S:");          // X en pos 0, Y en pos 7 y Z en pos 14
        
        set_cursor(2,0);
        Escribir_caracterLCD(cen_x);                    // Se escriben todos los valores en las posiciones correspondientes a la LCD
        Escribir_caracterLCD(dec_x);
        Escribir_caracterLCD(uni_x);
        
        // Divisor de valores en dígitos
        divisor_dec(Gy1,Giro_digy);                      // Divide el valor leído por el osciloscopio en 
        
        // Paso de valores a ASCII            
        uni_y = tabla_numASCII(Giro_digy[0]);
        dec_y = tabla_numASCII(Giro_digy[1]);
        cen_y = tabla_numASCII(Giro_digy[2]);
        
        set_cursor(2,6);
        Escribir_caracterLCD(cen_y);                    // Se escriben todos los valores en las posiciones correspondientes a la LCD
        Escribir_caracterLCD(dec_y);
        Escribir_caracterLCD(uni_y);
        
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
    
    PORTA = 0;                                      // Limpiar PORTA
    PORTB = 0;                                      // Limpiar PORTB
    PORTD = 0;
    
    //Configuración de oscilador
    initOsc(_4MHz);                                 // Oscilador a 8 mega hertz
    
    // Inicialización de módulo MSSP = I2C (Master)
    // Frecuencia de Transmisión: 100 kHz
    InitMSSP(I2C_MASTER_FOSC, 100000);
    
    //Inicialización y configuración de MPU6065
    InitMPU6050();
    
    // Configuración e inicialización de LCD
    Iniciar_LCD();                                  // Se inicializa la LCD en 8 bits
    Limpiar_pantallaLCD();
    set_cursor(1,0);
    Escribir_stringLCD("Miniproyecto 2");
    set_cursor(2,2);
    Escribir_stringLCD("Jose Santizo");
    __delay_ms(5000);
    Limpiar_pantallaLCD();
    
    //Configuración de TX y RX
    Config_USART(9600,4);
    
}

void divisor_dec(int b, char dig1[]){              // Divide un número para obtener sus dígitos decimales
    for(int n = 0; n<3 ; n++){                      // De i = 0 hasta i = 2
        dig1[n] = b % 10;                           // array[i] = cont_vol mod 10(retornar residuo). Devuelve digito por dígito de un número decimal.
        b = (b - dig1[n])/10;                       // a = valor sin último digito.
    }
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

