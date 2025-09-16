/**
 *  ===================================================
 *  DIY PROJECT NAME: Sound Alerts API
 *  ===================================================
 *
 *  @Filename           :   sounds.h
 *  @Date               :   25.08.2024
 *  @Author             :   Albert Ziatdinov
 *
 *  @Description        :   Header file providing interface and definitions 
 *                          for sound alerts, beep sequences, and 
 *                          state machine control on PIC16F18015.
 *
 *  @Contents
 *  =========
 *  - Macro definitions:
 *      SHORT / LONG / WAIT_TIME / TIME_OUT
 *      Alert types (NONE, ONE, TWO, THREE)
 *      Timeout flags (NO_TIMEOUT / YES_TIMEOUT)
 *      Beep sequences (TWO_SHORT_ONE_LONG, etc.)
 *      Queue status (FREE / BUSY)
 *
 *  - Functions:
 *      EnqueueBeeps()       - add beep sequence to queue
 *      BeepsStateMachine()  - sound playback state machine
 *      PlayAlert()          - play predefined alert sequences
 *
 *  @HW Compatibility   :   Microchip PIC16F18015
 *
 */

#ifndef SOUNDS_H
#define	SOUNDS_H

/**********Simple beeps and pauses********/
#define SHORT     120
#define LONG      300  //old was 200ms
#define WAIT_TIME 80
#define TIME_OUT  1300 //old was 1000ms

/***********Alert types******************/
#define NONE   0
#define ONE    1
#define TWO    2
#define THREE  3

#define NO_TIMEOUT  0
#define YES_TIMEOUT 1

/*****Sounds and pauses*****/
#define TWO_SHORT_ONE_LONG 1
#define TWO_SHORT_TWO_LONG 2
#define THREE_SHORT        3


#define FREE 0
#define BUSY 1

void EnqueueBeeps(uint16_t duration, uint8_t allowTimeout);

void BeepsStateMachine(void);

void PlayAlert(void); 

#endif	/* SOUNDS_H */

