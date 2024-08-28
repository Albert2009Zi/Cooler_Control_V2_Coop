#include <xc.h>
#include "sounds.h"

#define _XTAL_FREQ   4000000

void ShortSound(){
    GP2 = 1;
    __delay_ms(120);
    GP2 = 0;
    __delay_ms(100);  
}

void LongSound(){
       GP2 = 1;
    __delay_ms(300);
       GP2 = 0;
    __delay_ms(100);    
}

void TimeOut(){
    GP2 = 0;
    __delay_ms(1000);  
}


void TwoShortOneLong(){
   ShortSound();
   ShortSound();       
   LongSound();    
   TimeOut();
}

void TwoShortTwoLong(){

   ShortSound();
   ShortSound();       
   LongSound();       
   LongSound(); 
   TimeOut();  
}

void ThreeShort(){
   ShortSound();
   ShortSound();       
   ShortSound();   
   TimeOut(); 
}

/*void FiveShort(){
   ShortSound();
   ShortSound();
   ShortSound();
   ShortSound();       
   ShortSound();   
   TimeOut();
}*/ /*use only for Debug*/
