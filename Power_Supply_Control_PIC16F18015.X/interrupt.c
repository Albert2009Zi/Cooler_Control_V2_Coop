/** 
 *  ===================================================
 *  DIY PROJECT NAME: POWER SUPPLY CONTROL
 *  ===================================================
 * 
 *  @Filename           :   interrupt.c
 *  @Date               :   29.08.2025
 *  @Reference          :   PIC16F18015 Datasheet
 *                          ADC conversion and Timer0 operation
 *                          Voltage and temperature monitoring
 *                          Median filter implementation
 *  @HW Compatibility   :   Microchip PIC16F18015, 8bit, 8 Pin
 *  @Code reference     :   Microchip XC8 Compiler, PIC16F1xxx Libraries
 *  @SW Owner           :   Private author
 *                          Albert Ziatdinov  - hardware, firmware, documentation
 *  @Authors            :   Albert Ziatdinov
 * 
 *
 * @section Introduction
 * =====================
 * This unit provides the following functionalities:
 *  - ADC channel selection and conversion triggering (MuxADCChannel)
 *  - Global interrupt service routine for Timer0 overflow (ISR)
 *  - ADC result processing including median filtering and error detection
 *    (ADCProcessing)
 *  - Median filter implementation for smoothing sensor measurements 
 *    (MedianFilter)
 *
 * Important Notes:
 * =====================
 *  - System clock is configured to 32 MHz (_XTAL_FREQ)
 *  - Timer0 is configured to generate 1 ms overflow interrupts
 *  - ADC channels are alternately read for voltage and temperature
 *  - Voltage and temperature thresholds are used to update error flags
 *    and control GPIO outputs (e.g., LATA5)
 *  - TEMP_ERROR macro can alter temperature error thresholds
 *  - Median filter buffer size is defined by SAMPLING_SIZE
 *  - Global counter cnt0 is incremented by Timer0 ISR
 *  - All functions are compatible with PIC16F18015
 *
 */

#include <xc.h>
#include <stdint.h>
#include "interrupt.h"
 
#define _XTAL_FREQ 32000000 

#define TEMP_ERROR  // this detected 13.09.2025

extern volatile uint16_t cnt0;

volatile ParameterFlags prmFlags = {0};

uint16_t medianVoltage[SAMPLING_SIZE];
uint16_t medianTemperature[SAMPLING_SIZE];

/**
 *  ===========================================================================
 *  @Function Name      :   MuxADCChannel
 *  @Date               :   29.08.2025
 *  @Description        :   Selects and enables the specified ADC channel 
 *                          for conversion on the PIC16F18015.
 *
 *  @Parameters         :   adc_channel_t channelNumber
 *                          - The ADC channel to be selected
 *
 *  @Return             :   void
 *
 *  @Notes              :   - ADC is temporarily shut down before switching channels
 *                          - ADC result is right-justified
 *                          - Conversion is started immediately after selection
 *                          - ADC interrupt flag is cleared before start
 *
 *  @HW Compatibility   :   Microchip PIC16F18015
 *  @Author             :   Albert Ziatdinov
 *  ===========================================================================
 */
void MuxADCChannel(adc_channel_t channelNumber){  
    ADCON0        = 0x00;                         // ADC shut down
    ADCON0bits.FM = 1;                            // Right justify
    ADCON0bits.CS = 1;                            // Clock ADCRCR
    ADCON0bits.ON = 1;                            // ADC is ON
    ADPCH         = (unsigned char)channelNumber; // Channel select
    ADIF          = 0;                            // Set in 0 to new conversion
    ADCON0bits.GO = 1;                            // Start conversion
}

/**
 *  ===========================================================================
 *  @Function Name      :   ISR
 *  @Date               :   29.08.2025
 *  @Description        :   Global interrupt service routine for handling 
 *                          peripheral interrupts.
 *
 *  @Parameters         :   None
 *
 *  @Return             :   void
 *
 *  @Notes              :   - Currently handles Timer0 overflow only
 *                          - Timer0 overflow occurs every 1 ms
 *                          - Increments the global counter cnt0
 *                          - Timer0 flag (TMR0IF) must be manually cleared
 *                          - Timer0 interrupt remains enabled after service
 *
 *  @HW Compatibility   :   Microchip PIC16F18015
 *  @Author             :   Albert Ziatdinov
 *  ===========================================================================
 */
void __interrupt() ISR(void)
{       
    if (TMR0IF == 1){                 // Timer0 is overload      
            TMR0     = 5;             // To obtain a 1 ms inter single Measure that increments cnt0, 250 timer ticks are required. 255 - 5 = 250
                                      // Cause 250000Hz/250 = 1000Hz or Tone_tick = 1/1000 = 1ms     
            ++cnt0;
	        TMR0IF   = 0;             // needs necessary set this flag to 0    
            TMR0IE   = 1;             // Timer0 Interuupt is enabled  
    }    
}

/**
 *  ===========================================================================
 *  @Function Name      :   ADCProcessing
 *  @Date               :   29.08.2025
 *  @Description        :   Handles ADC conversion results, applies median 
 *                          filtering, and manages voltage and temperature 
 *                          error flags.
 *
 *  @Parameters         :   None
 *
 *  @Return             :   void
 *
 *  @Notes              :   - Uses static state variable adcState to alternate 
 *                            between voltage and temperature measurements
 *                          - MedianFilter() is applied to smooth measurements
 *                          - Updates GPIO outputs (e.g., LATA5) based on 
 *                            voltage and temperature thresholds
 *                          - Updates error flags: errorUnderVoltage, 
 *                            errorOverVoltage, errorHighTemperature
 *                          - Switches ADC channel using MuxADCChannel()
 *
 *  @HW Compatibility   :   Microchip PIC16F18015
 *  @Author             :   Albert Ziatdinov
 *  ===========================================================================
 */ 
void ADCProcessing(void){

   uint16_t adcValue         = 0;
   uint16_t newValue         = 0;
    static uint8_t  adcState = 0;
    
    if (ADIF == 1){                        // ADC conversion is complete
     newValue = ADRES;                     // ADC conversion result   
    
     switch (adcState){
        case STATE_MEASURE_VOLTAGE: 

      adcValue = MedianFilter(newValue, medianVoltage);      
            
	  if ((adcValue > 96) && (adcValue < 148)){
	     if(prmFlags.errorHighTemperature == ERROR_OK) LATAbits.LATA5 = 0;
	      else LATAbits.LATA5 = 1;
	      prmFlags.errorUnderVoltage = ERROR_OK;
          prmFlags.errorOverVoltage  = ERROR_OK;
           }  	
        else if (adcValue <= 96) { 
          LATAbits.LATA5 = 1; 
	      prmFlags.errorUnderVoltage = ERROR_NOK;
	 }
	 
	 
	 else if (adcValue >= 148){
       LATAbits.LATA5 = 1;
	   prmFlags.errorOverVoltage = ERROR_NOK;  
	  }
	 
      MuxADCChannel(ADC_CHANNEL0);
	  adcState = STATE_MEASURE_TEMPERATURE;
      break;
	
	
	case STATE_MEASURE_TEMPERATURE:
	//Warning!!! GP4 on real device (not simulation) controls a fun
    //From this commes Error on real Hardware    
        adcValue = MedianFilter(newValue, medianTemperature); 
#ifndef TEMP_ERROR        
	     if (adcValue < 200){
		      LATAbits.LATA5       = 1;  
		    //  GP4       = 0;
           	 prmFlags.errorHighTemperature = ERROR_NOK;
		  }
                
             else if ((adcValue > 200) && (adcValue < 930)){ 
#else        
              if (adcValue < 930){
#endif                  
	             if((prmFlags.errorUnderVoltage == ERROR_OK) && (prmFlags.errorOverVoltage == ERROR_OK)) LATAbits.LATA5 = 0;
		        else LATAbits.LATA5 = 1; 
		    //    GP4 = 0;
			    prmFlags.errorHighTemperature = ERROR_OK;
                       }
	       	       
	     else if ((adcValue > 930) && (adcValue < 970)){ 
	            if((prmFlags.errorUnderVoltage == ERROR_OK) && (prmFlags.errorOverVoltage == ERROR_OK)) LATAbits.LATA5 = 0;
		        else LATAbits.LATA5 = 1;
		    //    GP4 = 1;
			    prmFlags.errorHighTemperature = ERROR_OK;
                       }	
             else if (adcValue > 970) {
		      //  GP4       = 1;
	            LATAbits.LATA5       = 1;
                prmFlags.errorHighTemperature = ERROR_NOK; 
				 	
		   }
		   		
	    MuxADCChannel(ADC_CHANNEL1);
	    adcState = STATE_MEASURE_VOLTAGE;	    
	break;
	
	default:
	break;
	}  
  } 
}
   
/**
 *  ===========================================================================
 *  @Function Name      :   MedianFilter
 *  @Date               :   29.08.2025
 *  @Description        :   Implements a simple median filter for smoothing 
 *                          ADC or sensor measurements.
 *
 *  @Parameters         :   uint16_t singleMeasure
 *                              - New single measurement to be filtered
 *                          uint16_t *medianBuf
 *                              - Pointer to buffer storing the last 
 *                                SAMPLING_SIZE measurements
 *
 *  @Return             :   uint16_t
 *                              - The median value of the buffer after 
 *                                inserting the new measurement
 *
 *  @Notes              :   - Uses a static index to keep track of buffer position
 *                          - Performs partial insertion sort for efficiency
 *                          - Buffer size is defined by SAMPLING_SIZE
 *
 *  @HW Compatibility   :   Any MCU supporting 16-bit integers
 *  @Author             :   Albert Ziatdinov
 *  ===========================================================================
 */    
 uint16_t MedianFilter(uint16_t singleMeasure, uint16_t *medianBuf) {
       
     static uint8_t numberOfElement = 0;
     uint16_t temp = 0;
     
        medianBuf[numberOfElement] = singleMeasure;
        
        if ((numberOfElement < SAMPLING_SIZE - 1) && (medianBuf[numberOfElement] > medianBuf[numberOfElement + 1])) {
            for (uint8_t cnt = numberOfElement; cnt < SAMPLING_SIZE - 1; cnt++) {
                if (medianBuf[cnt] > medianBuf[cnt + 1]) {
                    temp = medianBuf[cnt];
                    medianBuf[cnt] = medianBuf[cnt + 1];
                    medianBuf[cnt + 1] = temp;
                }
            }
        } else {
            if ((numberOfElement > 0) && (medianBuf[numberOfElement - 1] > medianBuf[numberOfElement])) {
                for (uint8_t cnt = numberOfElement; cnt > 0; cnt--) {
                    if (medianBuf[cnt] < medianBuf[cnt - 1]) {
                        temp = medianBuf[cnt];
                        medianBuf[cnt] = medianBuf[cnt - 1];
                        medianBuf[cnt - 1] = temp;
                    }
                }
            }
        }
        if (++numberOfElement >= SAMPLING_SIZE) numberOfElement = 0;
   // Return the median     
   #if (SAMPLING_SIZE % 2 == 0) 
     return (medianBuf[SAMPLING_SIZE / 2 - 1] + medianBuf[SAMPLING_SIZE / 2]) / 2;
   #else 
     return medianBuf[SAMPLING_SIZE / 2];
   #endif
 }

