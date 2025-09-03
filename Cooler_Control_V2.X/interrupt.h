/* 
 * File:   ADCMeasure.h
 * Author: Ziatdinov
 *
 * Created on 25 ??????? 2024 ?., 12:18
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#include <stdint.h>

#define ERROR_OK               0
#define ERROR_NOK              1

#define READY     1
#define NOT_READY 0

#define STATE_MEASURE_VOLTAGE     0
#define STATE_MEASURE_TEMPERATURE 1

#define SAMPLING_SIZE 10

typedef struct {
    unsigned errorUnderVoltage    : 1;
    unsigned errorOverVoltage     : 1;
    unsigned errorHighTemperature : 1;
    unsigned alarmState           : 1;
    unsigned playLong             : 1;
} ParameterFlags;

extern volatile ParameterFlags prmFlags;

void MuxVoltage(void);

void MuxTemp(void);

void __interrupt() ISR(void);

void ADCProcessing(void);

uint16_t MedianFilter(uint16_t singleMeasure, uint16_t *medianBuf);

 void init(uint16_t singleMeasure, uint16_t *medianBuf);

#endif	/* INTERRUPT_H */

