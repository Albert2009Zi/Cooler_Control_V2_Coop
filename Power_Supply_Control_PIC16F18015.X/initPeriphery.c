/** 
 *  ===================================================
 *  DIY PROJECT NAME: POWER SUPPLY CONTROL
 *  ===================================================
 * 
 *  @Filename           :   initPeriphery.c
 *  @Date               :   29.08.2025
 *  @Reference          :   Peripheral initialization
 *                          Timer0 configuration
 *                          ADC configuration
 *                          GPIO setup
 *                          Set first ADC channel and first measurement starting  
 *  @HW Compatibility   :   Microchip PIC16F18015, 8bit, 20 Pin
 *  @Code reference     :   PIC16F180xx Family Datasheet & Microchip Libraries
 *  @SW Owner           :   Private authors
 *                          Albert Ziatdinov - hardware, firmware, documentation
 *  @Authors            :   Albert Ziatdinov
 * 
 *
 * @section Introduction
 * =====================
 * This unit provides initialization routines for:
 *  - Timer0 (8-bit, 1:32 prescaler, interrupt enabled)
 *  - GPIO (all ports digital, predefined logic levels)
 *  - Comparator & DAC shutdown
 *  - ADC configuration with channel selection
 *  - Global and peripheral interrupt enabling
 * 
 * Important Notes:
 * ================
 * - System clock is configured to 32 MHz
 * - RA0 and RA1 are set as analog inputs
 * - RA2, RA4, RA5 are predefined for external circuitry
 * - A long beep is triggered after successful initialization
 * 
 */

#include <xc.h>
#include <stdint.h>
#include "initPeriphery.h"    // own header file
#include "interrupt.h"        // includes it for use function MuxADCChannel()
                              // and long beep flag

#define _XTAL_FREQ 32000000 

void InitTimer0(void){

    T0CON0 = 0b10000000;      // TMR0 enabled, 8-bit, postscales 1:1
    T0CON1 = 0b01000101;      // TMR0 is synchronized to Fosc/4, (1:32 prescaler) 8000000/32 = 250000Hz

    TMR0H = 0;                // MSB (most significant byte) of a 16-bit timer 
    TMR0L = 0;                // LSB (least significant byte) of a 16-bit timer
    TMR0IF = 0;               // Drop flag of inerrupt Timer0
    TMR0IE = 1;               // Allow interrupt from Timer0
         
}

void Init_uC(void){    
    CM1CON0 = 0x00 ;		     // Comparator shut down
    FVRCONbits.EN  = 0;          // Vref = Vdd
    DAC1CONbits.EN = 0;          // DAC shut down
    
    /* Sets all Pins of chip as digital output  */
    LATA    = 0x00;              // Set all pins in low state (load in latch triggers 0)
    ANSELA  = 0x00;              // All pins are digital
    TRISA   = 0x00;              // All pins are outputs
    WPUA    = 0x00;              // 
    
    /* Sets RA2 (Pin 5) in nessesary conditions */
    LATAbits.LATA2     = 0;      // Low level on RA2 (Pin 5)                        */
        
    /* Sets RA5 (Pin 2) in nessesary conditions */
    LATAbits.LATA5     = 1;      // High level on RA5 (Pin 2)                       */

    /* Sets RA4 (Pin 3) in nessesary conditions */
    /*!!!For my Simulator PCB must HIGH everytime!!!*/
    LATAbits.LATA4     = 1;      // High level on RA4 (Pin 3)                       */
    
    /************************Setup ADC*****************************/    
    ADCON0bits.FM      = 1;      // Right justify
    ADCON0bits.CS      = 1;      // Clock ADCRCR
    
    TRISAbits.TRISA0   = 1;      // RA0(Pin 7) set as input
    ANSELAbits.ANSELA0 = 1;      // RA0(Pin 7) set as analog
   
    TRISAbits.TRISA1   = 1;      // RA0(Pin 6) set as input
    ANSELAbits.ANSELA1 = 1;      // RA0(Pin 6) set as analog
   
    ADCON1bits.ADDSEN  = 1;      // Only second Measure load in ADRES
    ADACQ              = 32;     // Set acquitisition time approximately 52us    
    ADCON0bits.ON      = 1;      // ADC ON
                                                        
    PEIE = 1;            
    GIE  = 1;			         // Enable global interrupts                 
    

    /************************Long Beep*****************************/    
    prmFlags.playLong = 1;       // Play the long beep. Configuration complete.
       
    MuxADCChannel(ADC_CHANNEL1); // Select channel for the first ADC measure   
}