/* 
 * File:   lcd.h
 * Author: EE475
 *
 * Created on November 2, 2017, 8:53 PM
 */

#ifndef LCD_H
#define	LCD_H

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include <string.h>

#define  _XTAL_FREQ 1000000
#define  button 
#define  RS LATBbits.LATB3
#define  RW LATBbits.LATB4
#define  E  LATBbits.LATB5
#define  DB0 LATCbits.LATC0
#define  DB1 LATCbits.LATC1
#define  DB2 LATCbits.LATC2
#define  DB3 LATCbits.LATC3
#define  DB4 LATCbits.LATC4 
#define  DB5 LATCbits.LATC5  // use for data
#define  DB6 LATCbits.LATC6  // use this for shift clock
#define  DB7 LATCbits.LATC7

void switchE();
void writeChar(unsigned char ch);
void writeCMD(unsigned char cmd);
void initialize();
void writeString(unsigned char* str);
void temperatureMeasurementC(int x);
void temperatureMeasurementF(int x);
void carbonMeasurement(int x);
void salinityMeasurement(int x);
int chanSel(int chan);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

