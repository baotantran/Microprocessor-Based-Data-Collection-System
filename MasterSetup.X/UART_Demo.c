
#include "mcc_generated_files/mcc.h"
#include <string.h>

/*
                         UART_Demo.c
 */

unsigned char temp;
unsigned int data;


void UART_Demo_Initialize() 
{

    printf("\rPICDEM LAB II - Date 08/11/2015\r\n");  
    printf("UART Communications 8-bit Rx and Tx\r\n\n");
    printf("Keyboard Type H : LED ON   Type L: LED OFF \r\n\n");

}

void UART_Demo_Command_INT(void) 
{    

    if(eusart2RxCount!=0) 
    {   

    temp=EUSART2_Read();  // read a byte for RX

    EUSART2_Write(temp);  // send a byte to TX  (from Rx)

    /*switch(temp)    // check command  
    {
     case 'H':
     case 'h':
        {
            //LED_SetHigh();
            CMD =3;
            break;
        }
     case 'L':
     case 'l':
        {
            LED_SetLow();
            printf(" -> LED Off!!     \r");   
            break;
        }
     default:
        {
            printf(" -> Fail Command!! \r");            
            break;
        }       
    }
    }*/

/*    if(strcmp(temp, 'c') == 0)
    {
        printf("hello");
        CMD = 1;
    }
    else if (strcmp(temp, 'f') == 0)
    {
        printf("bye");
        CMD = 0;
    }
    else
    {
        printf("wrong command");
    }
    
    if(CMD == 1)
    {
        LED_SetHigh();
    }
    else if(CMD == 0)
    {
        LED_SetLow();
    }
   }*/
    }
}

