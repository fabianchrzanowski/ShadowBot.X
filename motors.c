 /* File:   pwm8bits.c
 * Author: Craig Bacon
 * Created on 30 October 2014, 14:11
 * PWM pin 13 right hand motor, mark space ratio is 8 bit (255) and the PWM	         
*  frequency is 610 Hz    PWM Period = (PR2+1)x4 x 1/Osc x TMR2 Prescaler
 * e.g (255+1)x4 x1/10Mhz x 16 =1.638ms or 610Hz.PR2 changes frequency.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PWRT = ON
#define _XTAL_FREQ 10000000 // define clock frequency for __delay_10ms()
#define leftmotorB1 LATAbits.LATA4
#define leftmotorB2 LATAbits.LATA5
#define rightmotorA1 LATBbits.LATB0
#define rightmotorA2 LATBbits.LATB1

#define encoderLeft PORTCbits.RC0
#define encoderRight PORTCbits.RC5

#define LED1 LATBbits.LATB2
#define LED2 LATBbits.LATB3 
#define LED3 LATBbits.LATB4
#define LED4 LATBbits.LATB5 

void wait10ms(int del);

int main(void)
{
       unsigned char markspace=127;     	//mark space value for 8 PWM (50% mark space ratio)
       TRISCbits.RC2 = 0;          //set CCP1(pin13) to an output pin
       TRISCbits.RC1 = 0;
       TRISCbits.RC0 = 1;    //Port A all inputs
       TRISCbits.RC5 = 1;
       TRISBbits.RB0 = 0;
       TRISBbits.RB1 = 0;
       TRISAbits.RA4 = 0;
       TRISB=0b11000000;
       TRISAbits.RA5 = 0;
       PR2 = 0b11111111 ;    	//set period of PWM
       T2CON = 0b00000111 ;   //Timer 2(TMR2) on, Prescaler = 16
       CCP1CON = (0x0c);        //0x0c enables PWM module CCP1
       CCP2CON = (0x0c);
       CCPR1L = markspace;  //Load duty cycle into CCP1CON, PWM begins
       CCPR2L = markspace;
       
        rightmotorA1 = 1;     
         rightmotorA2 = 1;
         leftmotorB1 = 1;
         leftmotorB2 = 1;

       LED1=0; LED2=0; LED3=0; LED4=0; 
       int temp = encoderLeft;
       int count = 0;
       while(1){
           if (temp != encoderLeft){
               count++;
           }
           if (count > 400){LED1=1;}
           if (count > 420){LED2=1;}
           if (count > 440){LED3=1;}
           if (count > 460){LED4=1;}
           temp = encoderLeft;
           printf("%d", count);
  
     }            	
} //end main()

void wait10ms(int del){     	 //delay function
	int c;
	for(c=0;c<del;c++)
    	__delay_ms(10);
	return;
}


