#include <xc.h>
#include <stdint.h>
#include "ADCMeasure.h"
#include "sounds.h"

#define _XTAL_FREQ   4000000 

volatile uint16_t adcValue = 0;

//Median sorting filter
void sort(int *numOfSamples) {
    for (int cntOut = 0; cntOut < NUM_OF_SAMPLES - 1; cntOut++) {
        for (int cntIn = 0; cntIn < NUM_OF_SAMPLES - cntOut - 1; cntIn++) {
            if (numOfSamples[cntIn] > numOfSamples[cntIn + 1]) {
                int tmpVal = numOfSamples[cntIn];
                numOfSamples[cntIn] = numOfSamples[cntIn + 1];
                numOfSamples[cntIn + 1] = tmpVal;
            }
        }
    }
}

uint16_t ADCConversion(uint8_t channel){

       ADCON0      = 0;                   /* must after every new switch be*/ 
       VCFG        = 0;
       ADON        = 1;                   /* ADC is ON                                       */
       ADFM        = 1;                   /* ADC results is left justified                   */
       
   switch(channel){
	 case(VOLTAGE):
	      CHS1        = 0;   
          CHS0        = 1;
	  break;
	 case(TEMPERATURE):
          CHS1        = 0;   
          CHS0        = 0; 
 	  break;	 
	 default:
	  break; 
	}
                   
       ADIF     = 0;     
       __delay_us(100);
       GO = 1;                      /* Start conversion */
       while(ADIF != 1); 
       
       return (ADRESH << 8) + ADRESL;
}

void VoltageCheck(void){ 

   int samples[NUM_OF_SAMPLES]; //array of median sorting values
       adcValue = 0;
              
   for (int cnt = 0; cnt < NUM_OF_SAMPLES; cnt++) {
        samples[cnt] = ADCConversion(VOLTAGE);          // 
    }
    
    sort(samples);  // Median sorting
    
    adcValue = samples[NUM_OF_SAMPLES / 2];  // Filtered value
    
       
          if((adcValue > 85) && (adcValue < 130)) {
              GP5 &= ~1;                                      //Device ON
              GP2 &= ~1;                                      //No sound   
          }   
       
       
       else  if (adcValue <= 85) { 
           GP5   |= 1; 
       	   TwoShortOneLong();
           }
	      else if (adcValue >= 130){
           GP5   |= 1;
           TwoShortTwoLong();
           } 
            
}
	
void TemperatureCheck(void){
	
	uint16_t adcValue = 0;
	adcValue = ADCConversion(TEMPERATURE);
    
	     if (adcValue < 200){
		      GP5       = 1;  
		      GP4       = 0;
		  }
                
             else if ((adcValue >= 200) && (adcValue < 930)){ 
		        GP4       = 0;
                       }
	       	       
	     else if ((adcValue >= 930) && (adcValue < 970)){ 
		        GP4 = 1;
                       }	
             else  {
		        GP4       = 1;
	            GP5       = 1;
                ThreeShort(); 	
              //  FiveShort();
                   }
    }