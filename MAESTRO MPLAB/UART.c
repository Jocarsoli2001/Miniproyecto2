/*
 * File:   USART.c
 * Author: Jos� Sanoli
 *
 * Created on 7 de febrero de 2022, 10:15
 */


#include <xc.h>

#include "UART.h"

void Config_USART(int baud_rate, int Freq){
    if(baud_rate == 9600 && Freq == 4){
        TXSTAbits.SYNC = 0;                             // Transmisi�n as�ncrona
        TXSTAbits.BRGH = 1;                             // Baud rate a velocidad baja
    
        BAUDCTLbits.BRG16 = 0;                          // Baud rate de 16 bits
    
        SPBRG = 25;                                     // SPBRG:SPBRGH = 25
        SPBRGH = 0;
    }
    
    if(baud_rate == 9600 && Freq == 8){
        TXSTAbits.SYNC = 0;                             // Transmisi�n as�ncrona
        TXSTAbits.BRGH = 0;                             // Baud rate a velocidad baja
    
        BAUDCTLbits.BRG16 = 1;                          // Baud rate de 16 bits
    
        SPBRG = 51;                                     // SPBRG:SPBRGH = 51
        SPBRGH = 0;
    }
    
    if(baud_rate == 9600 && Freq == 1){
        TXSTAbits.SYNC = 0;                             // Transmisi�n as�ncrona
        TXSTAbits.BRGH = 1;                             // Baud rate a velocidad baja
    
        BAUDCTLbits.BRG16 = 1;                          // Baud rate de 16 bits
    
        SPBRG = 25;                                     // SPBRG:SPBRGH = 25
        SPBRGH = 0;
    }
    
    if(baud_rate == 10417 && Freq == 4){
        TXSTAbits.SYNC = 0;                             // Transmisi�n as�ncrona
        TXSTAbits.BRGH = 1;                             // Baud rate a velocidad baja
    
        BAUDCTLbits.BRG16 = 1;                          // Baud rate de 16 bits
    
        SPBRG = 23;                                     // SPBRG:SPBRGH = 25
        SPBRGH = 0;
    }
    
    if(baud_rate == 10417 && Freq == 8){
        TXSTAbits.SYNC = 0;                             // Transmisi�n as�ncrona
        TXSTAbits.BRGH = 0;                             // Baud rate a velocidad baja
    
        BAUDCTLbits.BRG16 = 1;                          // Baud rate de 16 bits
    
        SPBRG = 47;                                     // SPBRG:SPBRGH = 25
        SPBRGH = 0;
    }
    
    if(baud_rate == 10417 && Freq == 1){
        TXSTAbits.SYNC = 0;                             // Transmisi�n as�ncrona
        TXSTAbits.BRGH = 0;                             // Baud rate a velocidad baja
    
        BAUDCTLbits.BRG16 = 1;                          // Baud rate de 16 bits
    
        SPBRG = 5;                                     // SPBRG:SPBRGH = 25
        SPBRGH = 0;
    }
    
    RCSTAbits.SPEN = 1;                                 // Puertos seriales habilitados
    RCSTAbits.RX9 = 0;                                  // Recepci�n de datos de 8 bits 
    RCSTAbits.CREN = 1;                                 // Recepci�n continua habilitada
    
    TXSTAbits.TXEN = 1;                                 // Transmisiones habilitadas
    PIE1bits.RCIE = 0;
}
