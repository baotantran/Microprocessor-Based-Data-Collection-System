/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC18F25K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"
#include <xc.h>
#define _XTAL_FREQ 8000000
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*
                         Main application
 */

unsigned short dSlave;
unsigned char temp;
unsigned int ch;
unsigned int CMD;
unsigned int data;
float value;
unsigned int part1;
unsigned int part2;


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
    unsigned short x;
    SSP1CON2bits.SEN=1;                     //  Send the start condition
    master_wait();                          //  Wait till start condition is finished
    PIR1bits.SSP1IF=0;                      //  Clear the flag
    x=SSP1BUF;                           //  Clear the buffer (just in case)
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
    x=SSP1BUF;                           //  Load the data out
    PIR1bits.SSP1IF=0;
    //SSP1CON2bits.ACKDT=0;                   //  Acknowledge bit
    //SSP1CON2bits.ACKEN=1;                   //  Send acknowledge bit
    SSP1CON2bits.PEN=1;             //  Send stop process
    while(SSP1CON2bits.PEN);        //  Wait till stop process is done//  Clear the flag
    return x;
}

void checkValue()
{
    if(eusart2RxCount!=0) 
    {   

    ch=EUSART2_Read();  // read a byte for RX

    EUSART2_Write(ch);  // send a byte to TX  (from Rx)
    
    /*switch(ch)    // check command  
    {
     case '1':
        {
            CMD = 1;
            printf("\nCMD = %d\n", CMD);           
            break;
        }
     default:
        {
            printf(" -> Fail Command!! \r");            
            break;
        }       
    }*/
    
        if(strcmp(ch, '1' ) == 0)
        {
            CMD = 1;
            printf("\nCMD = %d\n", CMD);
        }
        else if(strcmp(ch, '2') == 0)
        {
            CMD = 2;
            printf("\nCMD = %d\n", CMD);
        }   
        else if(strcmp(ch, '3') == 0)
        {
            CMD = 3;
            printf("\nCMD = %d\n", CMD);
        }
        else if(strcmp(ch, '4') == 0)
        {
            CMD = 4;
            printf("\nCMD = %d\n", CMD);
        }
        else if(strcmp(ch, '5') == 0)
        {
            CMD = 5;
            printf("\nCMD = %d\n", CMD);
        }
        else if(strcmp(ch, 't') == 0)
        {
            CMD = 6;
            printf("\nCMD = %d\n", CMD);
        }
        else if(strcmp(ch, 'f') == 0)
        {
            CMD = 7;
            printf("\nCMD = %d\n", CMD);
        }
        else if(strcmp(ch, 'c') == 0)
        {
            CMD = 8;
            printf("\nCMD = %d\n", CMD);
        }
        else if(strcmp(ch, 's') == 0)
        {
            CMD = 9;
            printf("\nCMD = %d\n", CMD);
        }
        else {
            printf("\nwrong command\n");
        }
    }
}
float temperatureMeasurementC(unsigned int x)
{   
    float Vin = ((float)x / 1023) * 4.54;
    unsigned int temp = (int)((Vin - 0.0023)/0.0394);
    return temp;
}

float temperatureMeasurementF(unsigned int x)
{   
    float Vin = ((float)x / 1023) * 4.75;
    int temp = (int)((Vin - 0.0023)/0.0394);
    unsigned int F = temp*1.8 + 32;
    return F;
}

float carbonMeasurement(unsigned int x)
{
    float Vin = ((float)x / 1023) * 4.75;
    float Vin = Vin*40 + 50;
    float carbon = ((68*Vin/49) + 150/49);
    return carbon;
}

float salinityMeasurement(unsigned int x)
{
    float Vin = ((float)x / 1023) * 4.75;
    float Vin = 40.0*Vin + 100.0;
    float salinity = (9*Vin/40) - 35/2;
    return salinity;
}
/*void interrupt isr(){
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
}*/
unsigned int retrive_data(unsigned int command){
     master_write(0x80,command);
     part1=master_read(0x81);
     part2=master_read(0x81);
     data=0;
     return ((part1 | data) | (part2 << 8));
}
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    OSCCONbits.IRCF=0b110;
    // Enable the MSSP module--------------------------------------------------- 
    SSP1CON1bits.SSPEN=1;           //  I2C master mode clock=Fosc/(4*(SSPxADD+1))
    SSP1CON1bits.SSPM=0b1000;       //  Master mode
    SSP1CON1bits.WCOL=0;            //  Clear collision
    ANSELCbits.ANSC3=0;             //  Set C3 digital
    ANSELCbits.ANSC4=0;             //  Set C4 digital
    TRISCbits.RC3=1;                //  Set input
    TRISCbits.RC4=1;                //  Set input                 
    SSP1CON2=0;                     //  Clear all initiation to 0
    SSP1ADD=(_XTAL_FREQ/(4*100000))-1;       // Clock = Fosc/(4*(SPP1ADD + 1))
    SSP1STAT=0;                     //  Clear all stats
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
    TRISB=0b10000000; 
    ANSELB=0x00; 

    while (1)
    {
         checkValue(); 
         if(CMD==1){                //Switch case
             master_write(0x80,1);  
         } 
         else if(CMD == 2)          // Display in C 
         {
             data = retrive_data(CMD);
             value = temperatureMeasurementC(data);
         }
         else if(CMD == 3)          // Display in F
         {
             data = retrive_data(CMD);
             value = temperatureMeasurementF(data);
         }
         else if(CMD == 4)          // Display Carbon
         {
             data = retrive_data(CMD);
             value = carbonMeasurement(data);
         }
         else if(CMD == 5)          // Display Salinity
         {
             data = retrive_data(CMD);
             value = salinityMeasurement(data);
         }        
         else if(CMD==6){                // Celsius
             master_write(0x80,6);  
         }
         else if(CMD==7){                // Fahrenheit
             master_write(0x80,7);  
         }
         else if(CMD==8){                // Carbon
             master_write(0x80,8);  
         }
         else if(CMD==9){                // Salinity
             master_write(0x80,9);  
         }
         if(value!=0){
             printf("\nData = %0.2f\n", value);
             value=0; 
         }
         
          CMD=0;
    }
}
/**
 End of File
*/