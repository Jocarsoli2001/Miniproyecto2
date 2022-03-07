
#include "I2C.h"
// MSSP ------------------------------------------------------------------------

void InitMSSP(MSSP_Mode Modo, unsigned long Dato){
    // Habilitar pines de transmisión
    // Modo I2C: 1 = Habilita los pines SDA y SCL para transmisión
    // Modo SPI: 1 = Habilita los pines SCK, SDO, SDI y SS para transmisión
    SSPCONbits.SSPEN = 0;                               // Se deshabilita brevemente para permitir "re-inicializaciones"
    __delay_ms(10);
    SSPCONbits.SSPEN = 1;     
    
   
    if (Modo < 0b00000110){                                 // MSSP = SPI (Opciones para I2C son todas mayores a 6)
        
         // Configuración de pines de transmisión
        TRISCbits.TRISC5 = 0;                               // Serial Data Out (SDO|RC5) = Output
        TRISCbits.TRISC4 = 1;                               // Serial Data In (SDI|RC4) = Input
        
        if (Modo < 0b00000100){                             // Si SPI = Master (Opciones para Master son todas menores a 4)
            TRISCbits.TRISC3 = 0;                           // Serial Clock (RC3) = Output
        }  
        else {                                              // Si SPI = Slave
            TRISCbits.TRISC3 = 1;                           // Serial Clock (RC3) = Input   
        }   
        
        if (Modo == 0b00000100){ TRISAbits.TRISA5 = 1;}     // Si Slave Select = ON entonces SS (RA5) = Input
            
        // Polaridad de reloj
        // NOTA: Efectos de configuración varían según el modo (SPI o I2C)
        SSPCONbits.CKP = 0;                                 // Estado "Idle" del reloj = LOW
        
        // Master: Información sampleada en el medio del tiempo de  salida (0)
        // Slave: Debe colocarse en 0
        // NOTA: Colocar en 0 si es slave
        SSPSTATbits.SMP = 1;                                
        
    }
    
    else if (Modo >= 0b00000110){                           // MSSP = I2C (Opciones para SPI son todas menores a 6)
        
        TRISCbits.TRISC3 = 1;                               // Serial Clock (SCL | RC3) = Input
        TRISCbits.TRISC4 = 1;                               // Serial Data (SDA | RC4) = Input
        SSPCONbits.SSPEN = 1;                               // Se habilita el uso de los pines SDA y SCL para transmisión
        SSPSTATbits.SMP = 1;                                // Control de velocidad de rotación = OFF
        if (Modo == 0b00000111 || Modo == 0b00001111){
            UA = 0;                                         // Si se utiliza una dirección de 10 bits, se indica si se debe cambiar SSPADD (1) o no (0)
        }
        
        if (Modo == 0b00001000){                            // PIC = Master
            CKP = 0;                                        // No utilizado. Se limpia para evitar problemas
            SSPCON2 = 0;                                    // Se limpian todos los bits de status
            SSPSTAT = 0;                                    // Se limpia el registro SSPSTAT. SEN = Start condition idle
            SSPADD = (_XTAL_FREQ/(Dato*4))-1;               // Setup: Frecuencia reloj de transferencia. Dato = Frecuencia de transmisión
            PIE1bits.SSPIE = 0;                             // Interrupciones por MSSP = OFF
        } 
        else {                                              // PIC = Slave
            CKP = 1;                                        // SCL: 1 = Clock enable / 0 = Hold Clock Low
            SSPCON2bits.GCEN = 0;                           // Se deshabilita la recepción de una "General Call adress"
            SSPCON2bits.SEN = 1;                            // Se habilita el "Clock stretching" para recibir y mandar datos
            SSPADD = Dato;                                  // Dirección de Slave
            INTCONbits.GIE = 1;                             // Interrupciones globales = ON
            INTCONbits.PEIE = 1;                            // Interrupciones por periféricos = ON
            PIE1bits.SSPIE = 1;                             // Interrupciones por transmisión MSSP = ON
            SSPIF = 0;                                      // Se limpia bandera de interrupción por MSSP
        }
          
    }
    
    // Modo de funcionamiento para módulo MSSP
    // Se hace un OR con la variable "Modo" para evitar sobrescribir datos de SSPCON
    SSPCON = SSPCON |  Modo;
    
    // Configuración de flanco de reloj
    // Si CKP = 0: 1 - Transmisión en flanco positivo | 0 - Transmisión en flanco negativo
    // Si CKP = 1: Se invierte la configuración previa.
    SSPSTATbits.CKE = 0;                                    

}

// I2C =================================================
// NOTA: Para minimizar el error durante
//      - Transmisión (De Master a Slave): Se recomienda que el Slave tenga una frecuencia 
//        mayor a la del Master. Si se quiere que ambas sean iguales, entonces utilizar una
//        frecuencia de 8MHz en ambos PICS. Experimentalmente se comprobó que si las frecue-
//        ncias son iguales pero inferiores a 8MHz, I2CMasterCheck tiende a "atorarse".
//      - Recepción (De Slave a Master): Se recomienda que el Slave tenga una frecuencia igual 
//        o menor a la del Master. Esto puede no funcionar.

// Master: Chequeo de transmisión
void I2CMasterCheck(){
    while((SSPSTAT & 0x04)|| (SSPCON2 & 0x1F));     // Se espera a que no exista transmisión de datos
}


void I2C_Master_Start(Start_Type Modo_Inicio){
    I2CMasterCheck();                               // ¿Transmisión de datos?
    if (Modo_Inicio == 1) SSPCON2bits.SEN = 1;      // Se inicia la condición de "Repeated start" en SDA y SCL
    if (Modo_Inicio == 0) SSPCON2bits.RSEN = 1;     // Se inicia la condición de inicio estándar en SDA y SCL
    SSPIF = 0;
}

// Master: Detener comunicación
void I2CMasterStop(void){
    I2CMasterCheck();                               // ¿Transmisión de datos?
    SSPCON2bits.PEN = 1;                            // Se inicia la condición de STOP de los pines SDA y SCL
}

// Master: Escribir
void I2CMasterWrite(char Datos){
    I2CMasterCheck();                               // ¿Transmisión de datos?
    SSPBUF = Datos;                                 // Se escribe al Buffer        
}


char I2CMasterRead(ACK_Type ACK){
    char Datos;
    I2CMasterCheck();                               // ¿Transmisión de datos?
    SSPCON2bits.RCEN = 1;                           // Se habilita la recepción de datos
    I2CMasterCheck();                               // ¿Transmisión de datos?
    Datos = SSPBUF;                                 // Se almacenan los datos recibidos en el buffer
    I2CMasterCheck();                               // ¿Transmisión de datos?
    if (ACK == 0) SSPCON2bits.ACKDT = 0;            // 0 es el valor de "Acknowledge" 
    else          SSPCON2bits.ACKDT = 1;            // 1 es el valor de "Acknowledge" (Negative Acknowledge o NACK)
    SSPCON2bits.ACKEN = 1;                          // Se inicia la secuencia de "Acknowledge"
    return Datos;
}
