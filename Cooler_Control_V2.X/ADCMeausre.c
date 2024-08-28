#include <xc.h>
#include <stdint.h>
#include "ADCMeasure.h"
#include "sounds.h"

#define _XTAL_FREQ   4000000 

#define ERROR_OK  0
#define ERROR_NOK 1

uint16_t ADCConversion(uint8_t channel){

       ADCON0      = 0;                   /* must after every new switch be*/ 
       ADON        = 1;                   /* ADC is ON                                       */
       ADFM        = 1;                   /* ADC results is left justified                          */
       
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
       __delay_ms(2);
       GO_nDONE = 1;                      /* Start conversion */
       while(GO_nDONE);                   /* Wait of conversion ending */
       return ((ADRESH << 8) + ADRESL);   /* return 10bit result */ 
}

void VoltageCheck(void){ 
       
       uint16_t adcValue = 0;
       adcValue = ADCConversion(VOLTAGE);
       uint8_t errorCode;

   /* if ((adcValue > 90) && (adcValue < 140)){    
       GP5 = 0;                                      //Device ON
	   GP2 = 0;                                      //No sound
       }  	   
        else */
        if (adcValue <= 80) { 
           GP5 = 1; 
	       TwoShortOneLong();
           errorCode = ERROR_NOK;
           }
	      else if (adcValue >= 140){
           GP5 = 1;
	       TwoShortTwoLong();
           errorCode = ERROR_NOK;
           } 
            else {
              
                  GP5 = 0;                                      //Device ON
	              GP2 = 0;                                      //No sound
                  
                  if (errorCode == ERROR_NOK){
                  __delay_ms(2500);
                  errorCode = ERROR_OK;
                  }
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
