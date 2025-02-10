/* 
 * File:   ADCMeasure.h
 * Author: Ziatdinov
 *
 * Created on 25 ??????? 2024 ?., 12:18
 */

#ifndef ADCMEASURE_H
#define	ADCMEASURE_H

#define VOLTAGE     1
#define TEMPERATURE 2

#define NUM_OF_SAMPLES 10 //Bright window of measures, slow reaction of signal changing

uint16_t     ADCConversion(uint8_t channel);
void     sort(int *numOfSamples);
void     VoltageCheck(void);
void     TemperatureCheck(void);

#endif	/* ADCMEASURE_H */

