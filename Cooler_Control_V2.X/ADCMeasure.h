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

uint16_t ADCConversion(uint8_t channel);
void     VoltageCheck(void);
void     TemperatureCheck(void);

#endif	/* ADCMEASURE_H */

