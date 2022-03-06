/*
 * File:   MPU.c
 * Author: José Sanoli
 *
 * Created on 6 de marzo de 2022, 14:00
 */


#include "MPU.h"
#include "I2C.h"

// Inicializar la IMU
void InitMPU6050(){
    __delay_ms(150);                            // Delay para permitir que la IMU se inicialice propiamente
    
    // Clock = Giroscopio Eje X
    I2C_Master_Start();                             // Se inicia la comunicación "repetida" por I2C
    I2C_Master_Write(IMU+0);                        // Slave: IMU | Operación: Write (+0)
    I2C_Master_Write(PWR_MGMT_1);                   // CMD: Selección de registro PWR_MGMT_1
    I2C_Master_Write(0b00000001);                   // CMD: Sleep = OFF. CLKSEL = 001 (PLL con eje X de giroscopio como referencia). Este modo mejora la estabilidad de las lecturas 
    
    // Rango de giroscopio = +/- 250 °/s
    // Sensitividad = 131 LSB/°/s
    I2C_Master_RepeatedStart();
    I2C_Master_Write(IMU+0);                        // Slave: IMU | Operación: Write (+0)
    I2C_Master_Write(GYRO_CONFIG);                  // CMD: Selección de registro GYRO_CONFIG
    I2C_Master_Write(0b00000000);                   // CMD: No self-tests para los giroscopios (Bits 7, 6 y 5). Rango = 250 °/s (Bits 4 y 3)
    
    // Rango de acelerómetro = +/- 2g
    // Sensitividad = 16384 LSB/g
    I2C_Master_RepeatedStart();
    I2C_Master_Write(IMU+0);                        // Slave: IMU | Operación: Write (+0)
    I2C_Master_Write(ACCEL_CONFIG);                 // CMD: Selección de registro ACCEL_CONFIG
    I2C_Master_Write(0b00000000);                   // CMD: No self-tests para los acelerómetros (Bits 7, 6 y 5). Rango = 2g (Bits 4 y 3)
    
    // Filtro pasa baja digital (Reducir ruido)
    I2C_Master_RepeatedStart();
    I2C_Master_Write(IMU+0);                      // Slave: IMU | Operación: Write (+0)
    I2C_Master_Write(CONFIG);                     // CMD: Selección de registro CONFIG
    I2C_Master_Write(0b00000000);                 // CMD: Fs = 8kHz (Bits 0, 1 y 2) o filtro desactivado. Se implementa el Digital Low Pass Filter para reducir el ruido en mediciones
    
    // Sample Rate = Fs / (1 + SMPLRT_DIV)
    I2C_Master_RepeatedStart();
    I2C_Master_Write(IMU+0);                      // Slave: IMU | Operación: Write (+0)
    I2C_Master_Write(SMPRT_DIV);                  // CMD: Selección de registro SMPRT_DIV
    I2C_Master_Write(0b00000111);                 // CMD: SMPLRT_DIV = 7. Sample Rate = 8kHz / (1+7) = 1kHz
    
    I2C_Master_Stop();                            // Se detiene la comunicación
    
}
