#include <xc.h>
#include <stdint.h>
#include "interrupt.h"
#include "initPeriphery.h"
 
#define _XTAL_FREQ 32000000 

extern volatile uint16_t cnt0;

volatile ParameterFlags prmFlags = {0};

uint16_t medianVoltage[SAMPLING_SIZE];
uint16_t medianTemperature[SAMPLING_SIZE];

void MuxVoltage(void){  
    ADCON0        = 0x00;
    ADCON0bits.FM = 1;  //Right justify
    ADCON0bits.CS = 1;  //Clock ADCRCR
    ADCON0bits.ON = 1;
    ADPCH         = 0b00000001; //Channel 1
    ADIF          = 0;
    ADCON0bits.GO = 1;
}

void MuxTemp(void){  
    ADCON0        = 0x00;
    ADCON0bits.FM = 1;  //Right justify
    ADCON0bits.CS = 1;  //Clock ADCRCR
    ADCON0bits.ON = 1;
    ADPCH         = 0b00000000;  //Channel 0
    ADIF          = 0;
    ADCON0bits.GO = 1;
}

void __interrupt() ISR(void)
{       
//##########################Timer0#####################################// 
    if (TMR0IF == 1){                 // Timer0 is overload      
            TMR0     = 5;             // To obtain a 1 ms intersingleMeasure that increments cnt0, 250 timer ticks are required. 255 - 5 = 250
                                      // Cause 250000Hz/250 = 1000Hz or Tone_tick = 1/1000 = 1ms     
            ++cnt0;
	        TMR0IF   = 0;             // neccessary set this flag to 0    
            TMR0IE   = 1;             // Timer0 Interuupt is enabled  
    }    
}

 //##########################ADC#####################################//  

void ADCProcessing(void){

   uint16_t adcValue         = 0;
   uint16_t newValue         = 0;
    static uint8_t  adcState = 0;
    
    if (ADIF == 1){     
     newValue = ADRES; /* ADC result */   
    
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
	 
      MuxTemp();
	  adcState = STATE_MEASURE_TEMPERATURE;
      break;
	
	
	case STATE_MEASURE_TEMPERATURE:
	//Warning!!! GP4 on real device (not simulation) controls a fun
        adcValue = MedianFilter(newValue, medianTemperature); 
	     if (adcValue < 200){
		      LATAbits.LATA5       = 1;  
		    //  GP4       = 0;
           	 prmFlags.errorHighTemperature = ERROR_OK;
		  }
                
             else if ((adcValue > 200) && (adcValue < 930)){ 
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
		   		
	    MuxVoltage();
	    adcState = STATE_MEASURE_VOLTAGE;	    
	break;
	
	default:
	break;
	}  
  } 
}



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
        return medianBuf[SAMPLING_SIZE / 2];
    }

//    void init(uint16_t singleMeasure, uint16_t *medianBuf) {
//        for (uint8_t cnt = 0; cnt < SAMPLING_SIZE; cnt++){ 
//            medianBuf[cnt] = singleMeasure;
//        }
//    }

