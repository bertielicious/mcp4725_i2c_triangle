/*
 * File:   main.c
 * Author: Phil Glazzard
 *
 * Created on 27 November 2018, 14:08
 */
#include <math.h>
#include "config.h"
#include <xc.h>
#include "stdio.h"
#include "main.h"
#include "init_ports.h"
#include "config_osc.h"
#include "set_usart.h"
#include "putch.h"
#include "dac_write.h"

void main(void) 
{
    int x, y = 0;
    init_ports();
    config_osc();
    set_usart();
    printf("MCP4725 12-bit DAC controlled over i2c\n");
    
    TRISBbits.TRISB4 = 1;   // SDA is an input 
    TRISBbits.TRISB6 = 1;   // SCL is an input
    SDA = 1;                // bus idle condition
    SCL = 1;
    
    RED_LED = 1;            // RED LED lit shows that code is running
    
    /*Enable i2c master mode, SCL = FOSC/(4 * (SPPADD + 1))*/
    SSPCON1bits.SSPEN = 1;       // enables i2c SDA and SCL pins
    SSPCON1bits.SSP1M3 = 1;
    SSPCON1bits.SSP1M2 = 0;
    SSPCON1bits.SSP1M1 = 0;
    SSPCON1bits.SSP1M0 = 0;
    SSPADD = 0x05;              // SCL = 100kHz, Fosc = 4MHz
                                // SSPADD = (Fosc/(4 * SCL)) - 1
    
    PIR1bits.SSP1IF = 0;    // clear the SSP interrupt flag 
    
    unsigned char higher = 0xff;    //255
    unsigned char lower = 0x0f;     //15
    while(1)
    {
        RED_LED = ~RED_LED;                 // diagnostic LED to show code is running
        printf("higher = %x\n", higher);
        printf("lower = %x\n", lower);
        for(x = 0; x <=higher; x++)
        {
            for(y = 0; y <= lower; y++)
            {
                dac_write(0xC0,0x40,x,y<<4);

               // printf("x= %x y = %x\n", x,y<<4);
            }
        }
        for(x = higher; x >=0; x--)
        {
            for(y = lower; y >= 0; y--)
            {
                dac_write(0xC0,0x40,x,y<<4);

               // printf("x= %x y = %x\n", x,y<<4);
            }
        }
     
    }
}
    
    
