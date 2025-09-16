/**
 *  ===================================================
 *  DIY PROJECT NAME: POWER SUPPLY CONTROL
 *  ===================================================
 *
 *  @Filename           :   interrupt.h
 *  @Date               :   25.08.2024
 *  @Author             :   Albert Ziatdinov
 *
 *  @Description        :   Header file providing interface for ADC 
 *                          measurement, median filtering, and global 
 *                          interrupt handling on PIC16F18015.
 *
 *  @Contents
 *  =========
 *  - Macro definitions:
 *      ERROR_OK / ERROR_NOK
 *      READY / NOT_READY
 *      STATE_MEASURE_VOLTAGE / STATE_MEASURE_TEMPERATURE
 *      SAMPLING_SIZE
 *
 *  - Data types:
 *      ParameterFlags (bitfield flags structure)
 *      adc_channel_t  (ADC channel enum)
 *
 *  - Global variables:
 *      prmFlags (parameter status flags)
 *
 *  - Functions:
 *      MuxADCChannel()    - select ADC channel
 *      ISR()              - global interrupt handler
 *      ADCProcessing()    - ADC result handling
 *      MedianFilter()     - signal filtering
 *      init()             - initialization function
 *
 *  @HW Compatibility   :   Microchip PIC16F18015
 *
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

typedef enum {
    ADC_CHANNEL0 = 0,
    ADC_CHANNEL1 = 1
} adc_channel_t;


extern volatile ParameterFlags prmFlags;

void MuxADCChannel(adc_channel_t channelNumber);

void __interrupt() ISR(void);

void ADCProcessing(void);

uint16_t MedianFilter(uint16_t singleMeasure, uint16_t *medianBuf);

 void init(uint16_t singleMeasure, uint16_t *medianBuf);

#endif	/* INTERRUPT_H */

