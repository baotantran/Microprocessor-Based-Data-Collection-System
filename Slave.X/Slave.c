/*
 * File:   Master.c
 * Author: baota
 *
 * Created on October 26, 2017, 4:39 PM
 */



#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include <string.h>

#define  _XTAL_FREQ 1000000

int z;

void createSlave(short address){
    SSP1STAT=0x80;
    SSP1ADD=address;
    SSP1CON1=0b00110110;
    SSP1CON2=0x01;
    TRISCbits.RC3=1;
    TRISCbits.RC4=1;
    ANSELCbits.ANSC3=0;
    ANSELCbits.ANSC4=0;
    PIE1bits.SSP1IE=1;  //peripheral interrupt enable
    INTCONbits.GIE=1;   //global interrupt enable
    PIR1bits.SSP1IF=0;  //clear peripheral flag
    INTCONbits.PEIE=1;
}
 
void interrupt Slave_read(){
    if(PIR1bits.SSPIF==1){
        SSP1CON1bits.CKP=0;             // Hold the clock (SCL) while slave is processing
        if ((SSP1CON1bits.WCOL==1) || (SSP1CON1bits.SSPOV==1)) {
            z=SSP1BUF;                  //  Read the previous value;
            SSP1CON1bits.SSPOV=0;       //  Clear the overflow(byte receive while last byte still in buff, don't care in transmit mode)
            SSP1CON1bits.WCOL=0;        //  Clear the collision ()
            SSP1CON1bits.CKP=0;         //  Enable clock back
        }
        if (!SSP1STATbits.RW) { //  last byte was address, write
            z=SSP1BUF;
            SSP1CON1bits.CKP=1;         //  Shift out the last value in buffer
            while(SSP1STATbits.BF==0);  //  Wait till the buffer is full           
            z=SSP1BUF;              //  Write the data from buffer out                              //  Enable the clock back
        }
        /*else if (SSP1STATbits.RW==1) { // last byte was address, read
            z=SSP1BUF;                  //  Shift out the last value in buffer
            SSP1STATbits.BF=0;          //  Tell the MCU the buffer is empty (0:empty, 1: full)
            SSP1BUF=PORTB;              //  Read data from source to buffer -> buffer full
            SSP1CON1bits.CKP=1;         //  Release the clock to transfer data
            while(SSP1STATbits.BF==1);  //  Wait till the buffer is empty
        }*/
        PIR1bits.SSPIF=0; // Clear the flag
    }
}    
       

void main() { 
    //  Clock setup-------------------------------------------------------------
    //OSCCONbits.IRCF=0b110;      //  set the clock to 8MHz
    //  Weak pull up setup------------------------------------------------------
    INTCON2bits.RBPU=0;         //  Port B pull up enable
    WPUB=0xff;                  //  Enable weak pull up on all b port    
    //  Set input, output port
    TRISB=0xff;                 //  Set b as input, data send to master
    ANSELB=0x00;
    TRISA=0x00;                 //  Set a as output receive data from master
    ANSELA=0x00;
   
    // setup SSP1CON1 enable serial---------------------------------------------
    /*SSP1CON1bits.SSPEN=1;       //  enable serial port
    SSP1CON1bits.SSPM=0b1000;   //  I2C master mode clock=Fosc/(4*(SSPxADD+1))
    SSP1CON1bits.SSPOV=0;       //  Clear overflow indicator bit
    // setup SSP1ADD set the SCL frequency
    SSP1ADD;         // Set the frequency to 18
    // setup SSP1CON2 initiate communication, control data transfer
    SSP1CON2bits.SEN;           // In master mode bit initiate start condition SCL & SDA, in slave enable clock stretch (auto clear in hardware)
    SSP1CON2bits.RSEN;          // Only in master mode initiate repeated start conditionSCL & SDA (auto clear in hardware)
    SSP1CON2bits.PEN;           // Only in master mode initiate stop condition SCL & SDA (auto clear in hardware)
    SSP1CON2bits.RCEN;          // Only in master mode enable receive mode for I2C
    SSP1CON2bits.ACKEN;         // Only in master receive mode setting this bit initiates acknowledge sequence on SCL & SDA and send ACKDT(auto clear)
    SSP1CON2bits.ACKSTAT;       // 1 indicates that acknowledge was not received from the slave and vice versa. This bit has application in master transmit mode only.
    SSP1CON2bits.GCEN;          // General call enable bit. Setting this bit enables interrupt when a general call address is received in the register SSPSR.
    */
    createSlave(0x80);
     
       while(1){
      
           if(z==15){
               LATA=0xff;
           }
       }
}


                                                                                          