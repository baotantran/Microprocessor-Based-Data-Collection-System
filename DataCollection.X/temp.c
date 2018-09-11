/*
 * File:   temp.c
 * Author: baota
 *
 * Created on October 26, 2017, 4:39 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include <string.h>
#include "lcd.h"

#define  _XTAL_FREQ 1000000
#define  RS LATBbits.LATB3
#define  RW LATBbits.LATB4
#define  E  LATBbits.LATB5
#define  DB5 LATCbits.LATC5             // data for shift register
#define  DB6 LATCbits.LATC6             // clock for shift register
#define  inFlag INTCONbits.INT0IF
unsigned int count=0;
unsigned int CMD;
unsigned int n;
unsigned int temperC_F;
unsigned int carbon;
unsigned int salinity;
unsigned int index=0;
unsigned int random=769;
void switchE(){
    E=1;
    __delay_ms(10);
    E=0;
    __delay_ms(10);
}

void writeChar(unsigned char ch){
    RS=1; // select data register
    RW=0; // we are writing
    for (int i = 7; i>=0 ; i--)
    {
        DB5 = (ch >> i)&1;
        DB6 = 0;
        DB6 = 1;
        __delay_ms(1);
        
    }
    //DB6 = 0;
   // DB6 = 1;
    
    switchE();
}
void writeCMD(unsigned char cmd) {
    RS = 0;
    RW = 0;
    __delay_us(5);
    for (int i = 7; i>=0 ; i--)
    {
        DB5 = (cmd >> i) & 1;
        DB6 = 0;
        DB6 = 1;
        
    }   
    switchE();
    __delay_us(5);
    

    
    
}
void initialize(){
    __delay_ms(15);    
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
void writeString(unsigned char* str)
{
	for (int i =0; i < strlen(str); i++)
	{
		writeChar(str[i]);

	}
}

void temperatureMeasurementC(int x)
{   
    float Vin = ((float)x / 1023) * 4.75;
    int temp = (int)((Vin - 0.0023)/0.0394);
    char str[16];
    char str1[16];
    char str2[16];
    
    strcpy(str, "temperature     ");   
    writeString(str);
    writeCMD(0xC0);
    sprintf(str1, "%d", temp);
    writeString(str1);
    writeChar(0xDF);
    strcpy(str2, "C             ");
    writeString(str2);
    
}

void temperatureMeasurementF(int x)
{   
    float Vin = ((float)x / 1023) * 4.75;
    int temp = (int)((Vin - 0.0023)/0.0394);
    int F = temp*1.8 + 32;
    char str[16];
    char str1[16];
    char str2[16];
    
    strcpy(str, "temperature     ");  
    writeString(str);
    writeCMD(0xC0);
    sprintf(str1, "%d", F);
    writeString(str1);
    writeChar(0xDF);
    strcpy(str2, "F             ");
    writeString(str2);
}

void carbonMeasurement(int x)
{
    float Vin = ((float)x / 1023) * 4.75;
    float Vin = Vin*40 + 50;
    float carbon = ((68*Vin/49) + 150/49);
    char str[16];
    char str1[16]; 
    char str2[16];
    
    strcpy(str, "carbon level   ");
    writeString(str);
    writeCMD(0xC0);
    sprintf(str1, "%0.2f", carbon);
    writeString(str1);
    strcpy(str2, "ppm          ");
    writeString(str2);

}

void salinityMeasurement(int x)
{
    float Vin = ((float)x / 1023) * 4.75;
    float Vin = 40.0*Vin + 100.0;
    float salinity = (9*Vin/40) - 35/2;
    char str[16];
    char str1[16]; 
    char str2[16];
    
    strcpy(str, "salinity level  ");
    writeString(str);
    writeCMD(0xC0);
    sprintf(str1, "%0.2f", salinity);
    writeString(str1);
    strcpy(str2, "ppt          ");
    writeString(str2);
}


int chanSel(int chan){
    ADCON0bits.CHS=chan;
    __delay_us(10);
    ADCON0bits.GO=1;
    while (ADCON0bits.GO);
    unsigned int digital = (ADRESH << 8) + ADRESL; //put 10 bit binary into integer
    __delay_us(10);
    return digital;
}
unsigned int write_slave(){
    unsigned int temp;
            n=SSP1BUF;                  //  Clear the address just received from buffer
            //PIR1bits.SSPIF=0;          //  Clear the flag that was set after the 9th clock
            SSP1CON1bits.CKP=1;         //  Enable the clock for the data to transfer into buffer
            while(SSP1STATbits.BF==0);  //  Wait for the flag to set after the 9th clock
            //SSP1CON1bits.CKP=0;         //  Hold the clock to get the data out  
            temp=SSP1BUF;               //  Transfer data to port A-------------
            SSP1CON1bits.CKP=1;         //  Enable the clock
            return temp;
}

void read_slave(unsigned int byte1){
             SSP1CON1bits.CKP=0;        //  Hold the clock
             n=SSP1BUF;                 //  Transfer the address just received
             PIR1bits.SSPIF=0;         //  Clear the flag
             SSP1BUF=byte1;             //  Read data from port B---------------
             SSP1CON1bits.CKP=1;        //  Enable the clock for data to transfer out
             while(SSP1STATbits.BF==1); //  Wait till the data in buffer is empty
}

void read_2bytes(unsigned int data){
    if(index==0){
             read_slave(data);
             index=1;
             }else if (index==1){
                 read_slave(data>>8);
                 index=0;
}
}
void interrupt isr(){
if(INTCONbits.INT0IF){
inFlag=0;
count++;
if(count>=5){
    count=0;
}
}

    if(PIR1bits.SSPIF==1){
         SSP1CON1bits.CKP=0;             //  Hold the clock
         if((SSP1CON1bits.WCOL==1) || (SSP1CON1bits.SSPOV==1)){
             n=SSP1BUF;
             SSP1CON1bits.WCOL=0;
             SSP1CON1bits.SSPOV=0;
             SSP1CON1bits.CKP=0;
         }
         if(!SSP1STATbits.READ_WRITE){
            CMD=write_slave();
         }  else if (SSP1STATbits.READ_WRITE==1){
             if(CMD == 2)
             {
                 read_2bytes(temperC_F);   
             }
             else if(CMD == 3)
             {
                 read_2bytes(temperC_F);
             }
             else if(CMD == 4)
             {
                 read_2bytes(carbon);
             }
             else if(CMD == 5)
             {
                 read_2bytes(salinity);
             }
   
            }
         }
        //PIR1bits.SSP1IF=0;
        //PIR1bits.SSPIF=0;
        //LATAbits.LA7=PIR1bits.SSPIF;

         PIR1bits.SSPIF=0;
        }

void main() {
    //  Port setting------------------------------------------------------------
    // port A pin <0:3>: analog input <4:7>: digital output
    TRISA=0b11111111; //set output (0)input (1)
    ANSELA=0b00001111; // set digital(0) analog(1)
    // port B pin <0:2>: digital input(interrupt) <3:7>: digital output
    TRISB=0b00000111; //set output (0)input (1) 
    ANSELB=0b00000000; // set digital (0)
    // port C pin <0:7>: digital output (all used) 
    TRISC=0b00011000; //
    ANSELC=0b00000000; // 
    
    //  Analog setting----------------------------------------------------------
    ADCON0bits.ADON=1;      //  Enable ADC
    ADCON1bits.PVCFG=0;     //  Positive ref VDD (4.7V)
    ADCON1bits.NVCFG=0;     //  Negative ref VSS (0V)
    ADCON2bits.ADFM=1;      //  Right justified result
    ADCON2bits.ACQT=0b010;  //  Acquisition delay 4 TAD
    ADCON2bits.ADCS=0;      //  Conversion clock select 2us (for 1MHz)
    //  Interrupt setting-------------------------------------------------------
    INTCONbits.GIE=1;       //  Enable global interrupt
    INTCONbits.INT0IE=1;    //  Enable interrupt INT0
    INTCONbits.INT0IF=0;    //  Clear flag bit (inFlag)
    INTCON2bits.INTEDG0=1;  //  Set interrupt on rising edge
    PIE1bits.SSPIE=1;  //peripheral interrupt enable
    PIR1bits.SSPIF=0;  //clear peripheral flag
    INTCONbits.PEIE=1;  //enable all peripheral interrupt
    //  Capture setting---------------------------------------------------------    
    //  CCP1CONbits.CCP1M=0b0101;   //  Capture mode, rising edge
    //  PIR1bits.CCP1IF=0;          //  Set flag to 0
    //  PIE1bits.CCP1IE=1;          //  Enable software interrupt
    //  Timer setting-----------------------------------------------------------
    //  T1CONbits.TMR1CS=0b00;  //  Clock source is instruction clock
    //  T1CONbits.T1CKPS=0b00;  //  1:1 prescale value
    //  T1CONbits.T1RD16=1;     //  In one 16 bits operation
    //  T1CONbits.TMR1ON=1;     //  Enable timer
    //  Slave setting-----------------------------------------------------------
    //OSCCONbits.IRCF=0b110;      //  Set the clock to 8MHz
    SSP1CON1=0b00110110;
    SSP1ADD=0x80;               //  Set slave address
    TRISCbits.RC3=1;
    TRISCbits.RC4=1;
    ANSELCbits.ANSC3=0;
    ANSELCbits.ANSC4=0;
    SSP1CON2=0x01;              //  Clear all
    SSP1STAT=0x80;              //  Clear all except for slew rate
    
    initialize();           //  Initialize LCD
    writeChar('a');
    writeString("Temperature");
    while (1){ 
        /*if (CMD==1){
            count++;
            CMD=0;
            if(count>=5){
                count=0;
            }
        }*/
        temperC_F=chanSel(0);           //  Temperature from channel 0
        carbon=chanSel(1);              //  Carbon from channel 1
        salinity=chanSel(2);            //  Salinity from channel 2
        
        if(CMD==6){
            writeCMD(0x02);
            temperatureMeasurementC(temperC_F);
        } else if (CMD==7) {
            writeCMD(0x02);
            temperatureMeasurementF(temperC_F);
        } else if (CMD==8){
            writeCMD(0x02);
            carbonMeasurement(carbon);
        } else if (CMD==9){
            writeCMD(0x02);
            salinityMeasurement(salinity);
        }
    }
}

