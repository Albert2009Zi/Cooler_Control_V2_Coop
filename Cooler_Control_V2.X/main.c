/** 
 *  ===================================================
 *  DIY PROJECT NAME: COOLER CONTROL
 *  ===================================================
 * 
 *  @Filename           :   main.c 
 *  @Date               :   29.08.2025
 *  @Reference          :   Power supply/converter 12V DC to 220V AC
 *                          Temperature control
 *                          Over and under Voltage control
 *                          Cooler control
 *  @HW Compatibility   :   Microchip PIC12F675, 8bit, 8 Pin
 *  @Code reference     :   PIC16F18015 Stack by Microchip 
 *  @SW Owner           :   Privat authors
 *                          Albert Ziatdinov ziatdinovar@gmail.com
 *                          Alex Talko  
 *  @Authors            :   Albert Ziatdinov
 *                          Alex Talko
 * 
 *
 * @section Introduction
 * =====================
 * This application makes for Power supply/converter 12V DC to 220V AC to 
 * control:
 *  - over and under voltage
 *  - temperature
 *  - instied temperture turn on the cooler and generate PWM signal
 * 
 * Important User-Inputs and Variables:
 * ====================================
 *  
 * ***********************************
 * System clock is configured to 32MHz
 * ***********************************
 *
 *
 */

#include <xc.h>
#include <stdint.h>
#include "interrupt.h"
#include "initPeriphery.h"
#include "sounds.h"

#define _XTAL_FREQ 32000000 

//#define SIMULATION

#ifndef SIMULATION
// CONFIG1
#pragma config FEXTOSC  = OFF            // External Oscillator Selection bits (Oscillator not enabled)
#pragma config RSTOSC   = HFINTOSC_32MHz // Reset Oscillator Selection bits (HFINTOSC (32MHz))
#pragma config CLKOUTEN = OFF            // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config VDDAR    = HI             // VDD Range Analog Calibration Selection bit (Internal analog systems are calibrated for operation between VDD = 2.3 - 5.5V)

// CONFIG2
#pragma config WDTE  = OFF      // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTS = PWRT_OFF // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = EXTMCLR  // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP    = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD   = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#else 
// Config word
__CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_ON & MCLRE_OFF & BOREN_ON & CP_OFF & CPD_OFF);

#endif

// Main function
int main()
{   	
    InitTimer0();
    Init_uC();
    
    while(1){  
     ADCProcessing();  
     PlayAlert();
     BeepsStateMachine(); 
    }
    
   return 0;   
}