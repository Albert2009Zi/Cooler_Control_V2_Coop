#include <xc.h>
#include <stdint.h>
#include "init_periphery.h"
#include "ADCMeasure.h"
#include "sounds.h"

#define _XTAL_FREQ   4000000 

void Init_uC(void){    
    CMCON  = 0x07;         /* Shut down the Comparator                         */
    VRCON  = 0x00;         /* Shut down Comparator reference Voltage           */
    
    /* Sets all Pins of chip as digital output  */
    TRISIO  = 0;
    GPIO    = 0;
    
    /* Sets GP2 (Pin 5) in nessesary conditions */
    TRISIO2 = 0;           /* Sets GP2 (Pin 5) as output                       */
    GP2     = 0;           /* Low level on GP2 (Pin 5)                         */
    
    /* Sets GP5 (Pin 2) in nessesary conditions */
    TRISIO5 = 0;           /* Sets GP5 (Pin 2) as output                       */
    GP5     = 1;           /* High level on GP5 (Pin 2), System switched down  */
    
    /* Sets GP4 (Pin 3) in nessesary conditions */
    TRISIO4 = 0;           /* Sets GP4 (Pin 3) as output                       */
    GP4     = 0;           /* Low level on GP4 (Pin 3), Cooler is OFF          */
    
    
    VCFG    = 1;           /* Sets Vref = Vpin6                                */
    TRISIO0 = 1;           /* Sets GP0 (Pin 7) as input. Temperature control   */
    TRISIO1 = 1;           /* Sets GP1 (Pin 6) as input. Button control        */
    
    ANSEL   = 0b00110011;  /* Sets Tosc = 4us (RC Generator)                   */  
    PEIE = 1;            
    GIE  = 1;	           /* Enable global interrupts                         */
    
    LongSound();           /* System initialisation complete sound             */ 
    
    GP5 = 0;
    __delay_ms(2500);
    
}