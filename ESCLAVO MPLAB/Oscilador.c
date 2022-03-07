/*
 * File:   Oscilador.c
 * Author: José Sanoli
 *
 * Created on 7 de febrero de 2022, 18:00
 */


#include <xc.h>
#include "Oscilador.h"

void initOsc(uint8_t Valor){
    
    if(Valor == 8){            // 8 MHz
        OSCCONbits.IRCF0 = 1;
        OSCCONbits.IRCF1 = 1;
        OSCCONbits.IRCF2 = 1;
    }
    if(Valor == 4){            // 4 MHz
        OSCCONbits.IRCF0 = 0;
        OSCCONbits.IRCF1 = 1;
        OSCCONbits.IRCF2 = 1;
    }
    if(Valor == 2){            // 2 MHz
        OSCCONbits.IRCF0 = 1;
        OSCCONbits.IRCF1 = 0;
        OSCCONbits.IRCF2 = 1;
    }
    if(Valor == 1){            // 1 MHz
        OSCCONbits.IRCF0 = 0;
        OSCCONbits.IRCF1 = 0;
        OSCCONbits.IRCF2 = 1;
    }
    
    
    OSCCONbits.SCS = 1;         // Reloj Interno

}
