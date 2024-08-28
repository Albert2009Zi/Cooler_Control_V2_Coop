/**                                                  
 *  ==============================================================================
 *  PROJECT NAME: Cooler Control Hobby Project
 *  ==============================================================================
 * 
 *  @Filename           :   main.c 
 *  @Date               :   19.11.2021
 *  @Reference          :   No reference
 *  @HW Compatibility   :   Microchip PIC12F675, 8bit, 8 Pin
 *  @Code reference     :   No reference
 *  @Specification      :   Technical Task from Alex Talko
 *  @SW Owner           :   Albert Ziatdinov/Alex Talko, Open Non-commercial use 
 *                          only  
 *  @Author             :   Albert Ziatdinov
 * 
 * ================================================================================
 * System clock is configured to 4MHz
 * ================================================================================
 *
 *
 */

#include <xc.h>
#include <stdint.h>
#include "ADCMeasure.h"
#include "init_periphery.h"
#include "sounds.h"

//#define SIMULATION

#ifndef SIMULATION
// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#else 
// Config word
__CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_ON & MCLRE_OFF & BOREN_ON & CP_OFF & CPD_OFF);

#endif

// Main function
void main()        
{	
    Init_uC();
 
    while(1){  
     VoltageCheck(); 
  //   TemperatureCheck(); 
    }  
}