/* 
 * File:   sounds.h
 * Author: Ziatdinov
 *
 * Created on 25 ??????? 2024 ?., 12:21
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

