/**
 *  ===================================================
 *  DIY PROJECT NAME: POWER SUPPLY CONTROL
 *  ===================================================
 *
 *  @Filename           :   sounds.c
 *  @Date               :   29.08.2025
 *  @Description        :   Implements sound alert system using beeps queue.
 *                          Supports short and long beep sequences, alarm 
 *                          timeout, and state machine processing.
 *
 *  @HW Compatibility   :   Microchip PIC16F18015, 8bit, 8 Pin
 *  @SW Owner           :   Private author
 *                          Albert Ziatdinov
 *
 * @section Contents
 * =================
 *  - Types and structures:
 *      SoundParameters
 *  - Global variables:
 *      cnt0, beepsQueue, queueHead, queueTail
 *  - Function prototypes:
 *      EnqueueBeeps()
 *      BeepsStateMachine()
 *      PlayAlert()
 *
 */

#include <xc.h>
#include "sounds.h"
#include "interrupt.h"

#define _XTAL_FREQ 32000000

#define BEEPS_PER_ALERT 5     // size of Queue
volatile uint16_t cnt0 = 0;   // timer0 counter

//Sound parameters
typedef struct{
      uint16_t duration;      // duration of beep         
      uint8_t  allowTimeout;  // is alarm ends ans final timeout allows 
} SoundParameters;

//Queue and its variables
SoundParameters beepsQueue[BEEPS_PER_ALERT];
uint8_t queueHead = 0;                        
uint8_t queueTail = 0;                        

/**
 *  ===========================================================================
 *  @Function Name      :   EnqueueBeeps
 *  @Date               :   29.08.2025
 *  @Description        :   Adds a beep to the sound queue if there is space.
 *
 *  @Parameters         :   uint16_t duration      - duration of the beep
 *                          uint8_t  allowTimeout  - YES_TIMEOUT / NO_TIMEOUT
 *
 *  @Return             :   void
 *
 *  @Notes              :   Implements circular queue behavior.
 *  @HW Compatibility   :   Microchip PIC16F18015
 *  @Author             :   Albert Ziatdinov
 *  ===========================================================================
 */
void EnqueueBeeps(uint16_t duration, uint8_t allowTimeout) {
    
    if ((queueTail + 1) % BEEPS_PER_ALERT != queueHead) {     //Check if there is space in the circular queue
    
        beepsQueue[queueTail].duration     = duration;
        beepsQueue[queueTail].allowTimeout = allowTimeout;
        queueTail = (queueTail + 1) % BEEPS_PER_ALERT;
    } 
}

/**
 *  ===========================================================================
 *  @Function Name      :   BeepsStateMachine
 *  @Date               :   29.08.2025
 *  @Description        :   Handles playback of beeps from the queue.
 *                          Uses a simple state machine to control timing 
 *                          of beeps and timeout between sequences.
 *
 *  @Parameters         :   None
 *  @Return             :   void
 *
 *  @Notes              :   Updates GPIO (LATA2) to generate sound signal.
 *                          Modifies global counter cnt0.
 *  @HW Compatibility   :   Microchip PIC16F18015
 *  @Author             :   Albert Ziatdinov
 *  ===========================================================================
 */
void BeepsStateMachine(void){  

    static uint8_t  state                    = 0;
    static uint16_t stateMachineDuration     = 0;
    static uint8_t  stateMachineAllowTimeout = 0;

    switch(state){
       case 0:    
        if (queueHead != queueTail) {
                stateMachineDuration      = beepsQueue[queueHead].duration;     
                stateMachineAllowTimeout  = beepsQueue[queueHead].allowTimeout;
                queueHead = (queueHead + 1) % BEEPS_PER_ALERT;
                LATA2   = 1;   
                cnt0  = 0;
                state = 1;
		        prmFlags.alarmState = BUSY;
            }
	   break;
	
      case 1:
	  if (cnt0 >= stateMachineDuration){
             cnt0  = 0;
             state = 2;
             LATA2   = 0;
          } 
	break;
	
	case 2:
          if (cnt0 >= WAIT_TIME){ 
             cnt0  = 0; 
             if (stateMachineAllowTimeout == NO_TIMEOUT){ 
                 state = 0;    
	        }
            else if (stateMachineAllowTimeout == YES_TIMEOUT){
	        state = 3;
	        }
           }   
        break;
   
        case 3:
         if (cnt0 >= TIME_OUT){
           cnt0       = 0;
           queueHead  = 0;
           queueTail  = 0; 
	       state      = 0;	
	       prmFlags.alarmState = FREE; 
         }   
	break;
	 
        default:
        break;	
   }
}

/**
 *  ===========================================================================
 *  @Function Name      :   PlayAlert
 *  @Date               :   29.08.2025
 *  @Description        :   Determines which sound alert sequence to play 
 *                          based on system error flags and long beep flag.
 *
 *  @Parameters         :   None
 *  @Return             :   void
 *
 *  @Notes              :   Enqueues short and long beeps into the queue.
 *                          Checks error flags: errorUnderVoltage, 
 *                          errorOverVoltage, errorHighTemperature.
 *  @HW Compatibility   :   Microchip PIC16F18015
 *  @Author             :   Albert Ziatdinov
 *  ===========================================================================
 */
void PlayAlert() {

   uint8_t howMuchTimesPlayShort = NONE; 
   uint8_t howMuchTimesPlayLong  = NONE;
   uint8_t totalBeeps            = NONE;
   static uint8_t soundState     = 0;
   
   switch (soundState){
     case 0:
      if (prmFlags.errorUnderVoltage){ 
        howMuchTimesPlayShort = TWO;
        howMuchTimesPlayLong  = ONE;
        } 	
      if (!prmFlags.alarmState) soundState = 1; 	
     break;
     
     case 1:  
     if (prmFlags.errorOverVoltage) {
         howMuchTimesPlayShort = TWO;
         howMuchTimesPlayLong  = TWO; 
	 }  
      if (!prmFlags.alarmState) soundState = 2;	 
     break;
    
     case 2:  
     if (prmFlags.errorHighTemperature){
       howMuchTimesPlayShort = THREE;
       howMuchTimesPlayLong  = NONE; 
       }   
      if (!prmFlags.alarmState) soundState = 3; 
     break; 
     
     case 3:
      if (prmFlags.playLong){
       howMuchTimesPlayShort = NONE;
       howMuchTimesPlayLong  = ONE; 
       }   
      if (!prmFlags.alarmState) {
       soundState = 0; 
       prmFlags.playLong = 0;
       }
     break; 
     
     default:
     break;
}
           
    totalBeeps = howMuchTimesPlayShort + howMuchTimesPlayLong;
    
    // Add short beeps
    for (uint8_t i = 0; i < howMuchTimesPlayShort; i++) {
        if (i == (totalBeeps - 1)) {
            EnqueueBeeps(SHORT, YES_TIMEOUT);  // last
        } else {
            EnqueueBeeps(SHORT, NO_TIMEOUT);
        }
    }

    //  Add long beeps
    for (uint8_t i = 0; i < howMuchTimesPlayLong; i++) {
        if ((howMuchTimesPlayShort + i) == totalBeeps - 1) {
            EnqueueBeeps(LONG, YES_TIMEOUT);  // last
        } else {
            EnqueueBeeps(LONG, NO_TIMEOUT);
        }
    }
}