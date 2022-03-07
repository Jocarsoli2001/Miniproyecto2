 /*
 * File            : I2C.h
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordoñez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __I2C_H
#define	__I2C_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

// Setup de MSSP
typedef enum{
    SPI_MASTER_FOSC4    = 0b00000000,   // Master SPI con Clock = FOSC/4
    SPI_MASTER_FOSC16   = 0b00000001,   // Master SPI con Clock = FOSC/16
    SPI_MASTER_FOSC64   = 0b00000010,   // Master SPI con Clock = FOSC/64
    SPI_MASTER_TMR2     = 0b00000011,   // Master SPI con Clock = TMR2/2
    SPI_SLAVE_SS_EN     = 0b00000100,   // Slave SPI con Clock = SCK | SS pin = ON
    SPI_SLAVE_SS_DIS    = 0b00000101,   // Slave SPI con Clock = SCK | SS pin = OFF
    I2C_SLAVE_7BIT      = 0b00000110,   // Slave I2C con dirección de 7 bits
    I2C_SLAVE_10BIT     = 0b00000111,   // Slave I2C con dirección de 10 bits
    I2C_MASTER_FOSC     = 0b00001000,   // Master I2C con Clock = FOSC/4(SSPADD+1)
    I2C_SLAVE_7BIT_SS   = 0b00001110,   // Slave I2C con dirección de 7 bits e interrupciones por bits START y STOP
    I2C_SLAVE_10BIT_SS  = 0b00001110    // Slave I2C con dirección de 10 bits e interrupciones por bits START y STOP
}MSSP_Mode;

// Master: Iniciar comunicación
typedef enum{
    Standard = 1,
    Repeated = 0
} Start_Type; 

// Master: Leer
typedef enum{
    ACK = 0,
    NACK = 1
} ACK_Type; 

//*****************************************************************************
// Función para inicializar I2C Maestro
//*****************************************************************************
void InitMSSP(MSSP_Mode Modo, unsigned long Dato);
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2CMasterCheck();
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start(Start_Type Modo_Inicio);
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//*****************************************************************************
void I2CMasterStop(void);
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2CMasterWrite(char Datos);
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
char I2CMasterRead(ACK_Type ACK);
#endif	/* __I2C_H */

