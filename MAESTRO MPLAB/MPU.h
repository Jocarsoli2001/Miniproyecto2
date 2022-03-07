
#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Frecuencia para delay
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

// Definiciones de configuración
#define PWR_MGMT_1      0x6B
#define GYRO_CONFIG     0x1B
#define ACCEL_CONFIG    0x1C
#define CONFIG          0x1A
#define SMPRT_DIV       0x19

// Definiciones de lectura de datos
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48

// Valores de Calibración
#define GYRO_OFF_X      -354
#define GYRO_OFF_Y      140
#define GYRO_OFF_Z      210
#define ACCEL_X_MAX     18107
#define ACCEL_X_MIN     -14513
#define ACCEL_Y_MAX     16173
#define ACCEL_Y_MIN     -16380
#define ACCEL_Z_MAX     16509
#define ACCEL_Z_MIN     -16546

// NOTA: Para que el IMU logre funcionar, es necesario tomar en cuenta que el pin
// AD0 debe de estar conectado a voltaje. Si no se coloca a voltaje o a tierra, 
// el sensor comenzará a mandar el valor de -1 o el valor 1111 1111 1111 1111 en binario.
// Si este error sucede, chequear conexión. 

#define IMU 0b11010010                              // Dirección de la IMU + 1 cero (LSb)

// Definición de subrutinas del sensor
void InitMPU6050();