/*
 * File:   master.c
 * Author: baota
 *
 * Created on November 2, 2017, 2:11 PM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include <string.h>

unsigned char temp; 

void master_wait(){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
void master_write(unsigned short addr, unsigned short data){
            SSP1CON2bits.SEN=1; 
            master_wait();           
            PIR1bits.SSP1IF=0;
            SSP1BUF=addr;                   // Write address + read/write bit to buffer
            while(PIR1bits.SSP1IF==0);      // Wait till the flag is set(after 9th clock, address done transferring)
            if(SSP1CON2bits.ACKSTAT==1){    // Whether slave acknowledge    
                SSP1CON2bits.PEN=1;         // Send stop process if no acknowledge
                while(SSP1CON2bits.PEN);    // Wait till stop process is done
                return;                     // Exit program
            }
            PIR1bits.SSP1IF=0;              //  Clear the flag           
            SSP1BUF=data;             //  Write data to buffer 
            while(PIR1bits.SSP1IF==0);      //  Wait till flag is on (after 9th clock,data done transferring)
            PIR1bits.SSP1IF=0;              //  Clear the flag
            
            SSP1CON2bits.PEN=1;             //  Send stop process
            master_wait();        //  Wait till stop process is done
}
unsigned short master_read(unsigned short addr){
    unsigned short temp;
    SSP1CON2bits.SEN=1;                     //  Send the start condition
    master_wait();                          //  Wait till start condition is finished
    PIR1bits.SSP1IF=0;                      //  Clear the flag
    temp=SSP1BUF;                           //  Clear the buffer (just in case)
    SSP1BUF=addr;                           //  Write address and read bit
    while(PIR1bits.SSP1IF==0);              //  Wait for the flag, wait for address transfer is done
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1;
        master_wait();
        return 0;
    }
    PIR1bits.SSP1IF=0;                      //  Clear the flag
    SSP1CON2bits.RCEN=1;                    //  Configure master as receive
    master_wait();                          //  Wait till the RCEN is clear, buffer is full
    temp=SSP1BUF;                           //  Load the data out
    PIR1bits.SSP1IF=0;
    //SSP1CON2bits.ACKDT=0;                   //  Acknowledge bit
    //SSP1CON2bits.ACKEN=1;                   //  Send acknowledge bit
    SSP1CON2bits.PEN=1;             //  Send stop process
    while(SSP1CON2bits.PEN);        //  Wait till stop process is done//  Clear the flag
    return temp;
}

void interrupt isr(){
    if (INTCONbits.INT0IF){   
        for(int i=0;i<=1000;i++){
        master_write(0x80, 2);
        }
        INTCONbits.INT0IF=0;
    }   else if (INTCON3bits.INT1IF){
        temp=master_read(0x81);
        INTCON3bits.INT1IF=0;
    }
    SSP1CON2bits.PEN=1;
    while(SSP1CON2bits.PEN);
}
void main() {   
    OSCCONbits.IRCF=0b110;          //  Configure clock to 8MHz
    // Enable the MSSP module--------------------------------------------------- 
    SSP1CON1=0b00101000;
    ANSELCbits.ANSC3=0;             //  Set C3 digital
    ANSELCbits.ANSC4=0;             //  Set C4 digital
    TRISCbits.RC3=1;                //  Set input
    TRISCbits.RC4=1;                //  Set input      
    SSP1CON2bits.RCEN=0;
    SSP1CON2=0;                     //  Clear all initiation to 0
    SSP1ADD=(_XTAL_FREQ/(4*100000))-1;       // Clock = Fosc/(4*(SPP1ADD + 1))
    SSP1STAT=0x80;                     //  Clear all stats
    //  Interrupt setting-------------------------------------------------------
    INTCONbits.GIE=1;       //  Enable global interrupt
    INTCONbits.INT0IE=1;    //  Enable interrupt INT0 for INT0
    INTCON3bits.INT1IE=1;   //  Enable interrupt INT1 for INT1
    INTCONbits.INT0IF=0;    //  Clear flag bit (inFlag) for INT0
    INTCON3bits.INT1IF=0;    //  Clear flag bit (inFlag) for INT1
    INTCON2bits.INTEDG0=1;  //  Set interrupt on rising edge for INT0
    INTCON2bits.INTEDG1=1;  //  Set interrupt on rising edge for INT1
    //  Input Output setting----------------------------------------------------
    TRISA=0x00;                 
    ANSELA=0x00;
    TRISB=0xff; 
    ANSELB=0x00; 
    
            while(1){
                //master_write(0x80, 2);
                
            }
}
