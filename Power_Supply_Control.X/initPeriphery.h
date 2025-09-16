/**
 *  ===================================================
 *  DIY PROJECT NAME: POWER SUPPLY CONTROL
 *  ===================================================
 *
 *  @Filename           :   init_periphery.h
 *  @Date Created       :   29.08.2024
 *  @Author             :   Albert Ziatdinov
 *
 *  @Description        :   Header file with initialization 
 *                          routines for PIC16F18015 peripherals.
 *
 *  @Contents
 *  =========
 *  - Functions:
 *      InitTimer0()    - configure Timer0 for system timing
 *      Init_uC()       - initialize core microcontroller settings
 *
 *  @HW Compatibility   :   Microchip PIC16F18015
 *
 */
#ifndef INIT_PERIPHERY_H
#define	INIT_PERIPHERY_H

void InitTimer0(void);

void Init_uC(void);

#endif	/* INIT_PERIPHERY_H */

