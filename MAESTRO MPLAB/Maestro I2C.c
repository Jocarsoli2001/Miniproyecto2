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
#define IMU 0x68                                    // Dirección de la IMU + 1 cero (LSb)
#define PIC_esclavo 0x50                            // Constante que lleva la dirección del PIC esclavo

//-----------------------Variables------------------------------------

//Variables para giroscopio
int Ax = 0;                                         // Aceleración sobre eje X de acelerómetro
int Ay = 0;                                         // Aceleración sobre eje Y de acelerómetro
int Az = 0;                                         // Aceleración sobre eje Z de acelerómetro
int Temp = 0;                                       // Temperatura medida por IMU
int Gx = 0;                                         // Medición del giroscopio sobre eje X 
int Gy = 0;                                         // Medición del giroscopio sobre eje Y
int Gz = 0;                                         // Medición del giroscopio sobre eje Z

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
void divisor_dec(uint8_t b, char dig1[]);           // Función para dividir valores en sus dígitos decimales

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
        I2C_Master_Start();
        I2C_Master_Write(IMU+0);                        // Slave: IMU | Operación: Write (+0)
        I2C_Master_Write(ACCEL_XOUT_H);                 // CMD: Selección de registro inicial = ACCEL_XOUT_H 
        I2C_Master_Stop();
        
        I2C_Master_RepeatedStart();                     // Inicio de lectura continua
        I2C_Master_Write(IMU+1);                        // Slave: IMU | Operación: Read (+1)
        
        // Lectura de giroscopio
        // NOTA: Se realizará una lectura continua de todos los valores que devuelve el IMU. El IMU
        // retorna 7 valores distintos en 14 registros diferentes, por lo que será necesario
        // hacer las lecturas continuas y concatenar valores en 7 diferentes variables. No se estará
        // mandando el bit de acknowledge cada vez que se realiza un lectura, ya que de esa forma
        // el IMU cambiará de registro y en cada lectura se obtiene un valor distinto. El único momento
        // donde se enviará el bit de acknowledge es en la última lectura, ya que acá ya no queremos recibir
        // más valores.
        
        
        // Bits más significativos<<8 + Menos significativos
        Ax = ((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0);       
        Ay = ((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0);
        Az = ((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0);
        Temp = ((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0);
        Gx = ((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0);
        Gy = ((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0);
        Gz = ((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(1);
        
        I2C_Master_Stop();                              // Fin del "burst read" o lectura continua
        
        //---------------------------------------------------------------------- 
        // IMPRIMIR VALORES A LCD
        //----------------------------------------------------------------------
        
        // Divisor de valores en dígitos
        divisor_dec(Gx,Giro_digx);                      // Divide el valor leído por el osciloscopio en 
        
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
        
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0;                                      // Pines digitales
    ANSELH = 0;
    
    TRISA = 0;                                      // PORTA como salida
    TRISB = 0;                                      // PORTB como salida
    TRISD = 0;                                      // PORTD como salida
    TRISE = 0;                                      // PORTE como salida
    
    PORTA = 0;                                      // Limpiar PORTA
    PORTD = 0;                                      // Limpiar PORTD
    PORTE = 0;                                      // Limpiar PORTE
    PORTB = 0;                                      // Limpiar PORTB
    PORTC = 0;
    
    //Configuración de oscilador
    initOsc(_4MHz);                                 // Oscilador a 4 mega hertz
    
    //Configuración de I2C
    I2C_Master_Init(100000);
    
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

void divisor_dec(uint8_t b, char dig1[]){            // Divide un número para obtener sus dígitos decimales
    for(int n = 0; n<3 ; n++){                      // De i = 0 hasta i = 2
        dig1[n] = b % 10;                            // array[i] = cont_vol mod 10(retornar residuo). Devuelve digito por dígito de un número decimal.
        b = (b - dig1[n])/10;                        // a = valor sin último digito.
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

