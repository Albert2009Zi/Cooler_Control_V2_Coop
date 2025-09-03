#include <xc.h>
#include <stdint.h>
#include "initPeriphery.h"
#include "interrupt.h"
#include "sounds.h"

#define _XTAL_FREQ   32000000 

void InitTimer0(void){

    T0CON0 = 0b10000000;      // TMR0 enabled, 8-bit, postscales 1:1
    T0CON1 = 0b01000101;      // TMR0 is synchronized to Fosc/4, (1:32 prescaler) 8000000/32 = 250000Hz

    TMR0H = 0;                 
    TMR0L = 0;
    TMR0IF = 0;               // Drop flag of inerrupt Timer0
    TMR0IE = 1;               // Allow interrupt from Timer0
         
}

void Init_uC(void){    
    CM1CON0 = 0x00 ;		   /* Shut down the Comparator                        */
    FVRCONbits.EN  = 0;
    DAC1CONbits.EN = 0;
    
    /* Sets all Pins of chip as digital output  */
    LATA    = 0x00;
    ANSELA  = 0x00;
    TRISA   = 0x00;
    WPUA    = 0x00;
    
    /* Sets RA2 (Pin 5) in nessesary conditions */
    LATAbits.LATA2     = 0;           /* Low level on RA2 (Pin 5)                        */
        
    /* Sets RA5 (Pin 2) in nessesary conditions */
    LATAbits.LATA5     = 1;           /* High level on RA5 (Pin 2)                       */

    /* Sets RA4 (Pin 3) in nessesary conditions */
    /*!!!For my Simulator PCB must HIGH everytime!!!*/
    LATAbits.LATA4     = 1;           /* High level on RA4 (Pin 3)                       */
    
    /************************Setup ADC*****************************/    
    ADCON0bits.FM      = 1;  //Right justify
    ADCON0bits.CS      = 1;  //Clock ADCRCR
    
    TRISAbits.TRISA0   = 1;  //RA0(PIN 7) set as input
    ANSELAbits.ANSELA0 = 1;  //RA0(PIN 7) set as analog input
   
    TRISAbits.TRISA1   = 1;  //RA0(PIN 6) set as input
    ANSELAbits.ANSELA1 = 1;  //RA0(PIN 6) set as analog input
   
    ADACQ              = 32; //Set acquitisition time approximately 52us    
    ADCON0bits.ON      = 1;
                                                        
    PEIE = 1;            
    GIE  = 1;			   /* Enable global interrupts                        */
    

    /************************Long Beep*****************************/    
    prmFlags.playLong = 1;  
       
    MuxVoltage();
    
}