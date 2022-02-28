/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once. 

#include <xc.h> // include processor files - each processor file is guarded.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------Constantes--------------------------------------
// Frecuencia para delay
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif


// Pines
#define LCD_EN PORTDbits.RD5                // Enable = RD5
#define LCD_RS PORTDbits.RD3                // Register Select = RD3
#define LCD_RW PORTDbits.RD4                // Read/Write = RD4

#define D0 PORTDbits.RD6                    // RD6 = D0
#define D1 PORTDbits.RD7                    // RD7 = D1
#define D2 PORTBbits.RB0                    // RB0 = D2
#define D3 PORTBbits.RB1                    // RB1 = D3
#define D4 PORTBbits.RB2                    // RB2 = D4
#define D5 PORTBbits.RB3                    // RB3 = D5
#define D6 PORTBbits.RB4                    // RB4 = D6
#define D7 PORTBbits.RB5                    // RB5 = D7

//-----------------------Variables utilizadas---------------------------------


//-----------------------Funciones definidas----------------------------------
void Escribir_comandoLCD(unsigned char);
void Escribir_datosLCD(char);
void Iniciar_LCD(void);
void Escribir_stringLCD(const char*);
void Limpiar_pantallaLCD(void);
void prender_ELCD(void);
void set_cursor(char linea, char posicion);
void Escribir_caracterLCD(uint8_t a);

