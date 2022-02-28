/*
 * File:   LCD.c
 * Author: Jos� Sanoli
 *
 * Created on 31 de enero de 2022, 15:29
 */


#include "LCD.h"

void prender_ELCD(void){
    LCD_EN = 1;                                                 //Pulso a pin E para dejar pasar datos del data bus a la LCD
    __delay_us(500);
    LCD_EN = 0;
}

void Escribir_comandoLCD(unsigned char Comando){
    if((Comando & 0b00000001) == 0b00000001){D0 = 1;} else {D0 = 0;}
    if((Comando & 0b00000010) == 0b00000010){D1 = 1;} else {D1 = 0;}
    if((Comando & 0b00000100) == 0b00000100){D2 = 1;} else {D2 = 0;}
    if((Comando & 0b00001000) == 0b00001000){D3 = 1;} else {D3 = 0;}
    if((Comando & 0b00010000) == 0b00010000){D4 = 1;} else {D4 = 0;}
    if((Comando & 0b00100000) == 0b00100000){D5 = 1;} else {D5 = 0;}
    if((Comando & 0b01000000) == 0b01000000){D6 = 1;} else {D6 = 0;}
    if((Comando & 0b10000000) == 0b10000000){D7 = 1;} else {D7 = 0;}
    
    prender_ELCD();
}

void Iniciar_LCD(void){
    LCD_RS = 0;
    LCD_RW = 0;
    
    __delay_ms (14);
    Escribir_comandoLCD(0b00110000);
    __delay_ms (4);
    Escribir_comandoLCD(0b00110000);
    __delay_us (100);
    Escribir_comandoLCD(0b00110000);
    
    // Reset
    __delay_us (100);
    Escribir_comandoLCD(0b00111000);                            // 2 lineas y espacios de 5x8 pixeles
    __delay_us (100);
    Escribir_comandoLCD(0b00001000);                            // Apagar visualizador
    __delay_us (100);
    Escribir_comandoLCD(0b00000001);                            // Borrar visualizador
    __delay_us (100);
    Escribir_comandoLCD(0b00000110);                            // Modo de entrada con incremento y desplazamiento desactivado
    __delay_us (100);
    Escribir_comandoLCD(0b00001100);                      
    return;
}

void Escribir_stringLCD(const char *d){
    LCD_RS = 1; LCD_RW = 0;
    
    for (char i=0; d[i]!='\0'; i++){   
        Escribir_comandoLCD(d[i]);                              // Env�a el c�digo ASCII de la letra en la posici�n i hacia los puertos D           
    }   
}

void Escribir_caracterLCD(uint8_t a){                           // Función para poder escribir un solo caracter en la LCD
    LCD_RS = 1; LCD_RW = 0;
    
    Escribir_comandoLCD(a);
}

void Limpiar_pantallaLCD(void){
    LCD_RS = 0; LCD_RW = 0;
    
    Escribir_comandoLCD(0b00000001);
    __delay_ms(10);
    Escribir_comandoLCD(0b00000010);
}

void set_cursor(char linea, char posicion){
    LCD_RS = 0; LCD_RW = 0;                                     // Necesario para "Desplazar Cursor" y "Cambiar Direcci�n DDRAM"
    
    // Escribir en primera o segunda l�nea de LCD
    if (linea == 1){ Escribir_comandoLCD(0b10000000);}          // Colocarse en �ltima posici�n de segunda l�nea y sumar posiciones para subir a primera l�nea 
    else if (linea == 2){ Escribir_comandoLCD(0b11000000);}     // Colocarse en �ltima posici�n de primera l�nea y sumar posiciones para bajar a segunda l�nea

    __delay_ms(40);                                             // Delay para permitir que la LCD finalice operaciones
    
    if (posicion != 0){                                         // Si las posiciones a mover son diferentes a 0, entonces
        while(posicion > 0){
            Escribir_comandoLCD(0b00010100);                    // Se desplaza 1 posici�n (0001) a la derecha (****01)
            posicion--;                                         // Posiciones disminuye en 1
        }

        __delay_ms(40);                                         // Delay para permitir que la LCD finalice operaciones
    }
    
}
