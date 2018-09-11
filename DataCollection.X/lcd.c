/*
 * File:   lcd.c
 * Author: baota
 *
 * Created on October 26, 2017, 4:39 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"


#define  _XTAL_FREQ 12000000
#define  LCD_PORT PORTA
#define  PWR LATAbits.LA3

#define  RS LATAbits.LATA0
#define  RW LATAbits.LATA1
#define  E  LATAbits.LATA2
#define  DB0 LATCbits.LATC0
#define  DB1 LATCbits.LATC1
#define  DB2 LATCbits.LATC2
#define  DB3 LATCbits.LATC3
#define  DB4 LATCbits.LATC4
#define  DB5 LATCbits.LATC5
#define  DB6 LATCbits.LATC6
#define  DB7 LATCbits.LATC7

void switchE(){
    E=1;
    __delay_ms(10);
    E=0;
    __delay_ms(10);
}

void writeChar(unsigned char ch){
    RS=1; // select data register
    RW=0; // we are writing
    // LATB=COM;// try this if not correct character do individual
    // DB0=COM & 1;
    // DB1=(COM >> 1) & 1 
    DB7 = (ch >> 7)& 1;
    DB6 = (ch >>6)&1;
    DB5 = (ch >>5)&1;
    DB4 = (ch >>4)&1;
    DB3 = (ch >>3)&1;
    DB2 = (ch >>2)&1;
    DB1 = (ch >>1)&1;
    DB0 = (ch & 1);
    switchE();
}
void writeCMD(unsigned char cmd) {
    RS = 0;
    RW = 0;
    __delay_ms(100);
    DB7 = ((cmd >> 7)&1);
    DB6 = ((cmd >> 6)&1);
    DB5 = ((cmd >> 5)&1);
    DB4 = ((cmd >> 4)&1);
    DB3 = ((cmd >> 3)&1);
    DB2 = ((cmd >> 2)&1);
    DB1 = ((cmd >> 1)&1);
    DB0 = (cmd & 1);
    switchE();
    __delay_ms(10);
    

    
    
}
void initialize(){
   // RS=0;
    // RW=0;
    // LCD_PORT = 0;
    __delay_ms(15);
    TRISA = 0x00;
    TRISC = 0x00;
    
    //Power up LCD
    // PWR = 1;
    __delay_ms(15);
    //display initialize
    writeCMD(0x30);
    //set number of lines and fonts
    writeCMD(0x38);
    __delay_ms(15);
    // display off
    writeCMD(0x08);
    __delay_ms(15);
    
    
    //entry mode
    writeCMD(0x01);
    __delay_ms(15);
    
    writeCMD(0x06);
    __delay_ms(15);
    writeCMD(0xF);
    __delay_ms(15);
    
      
    
}

void main() {
    //TRISA=0x00; //set port a(direction register) as output port(0 for output and 1 for input)
    //TRISC=0x00; //set port c(direction register) as output port(0 for output and 1 for input)
   
    
    
    initialize();
    writeChar('f');
    writeChar('u');
    writeChar('c');
    writeChar('k');
    
    
    while (1){
        
    }
   
}
