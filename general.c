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
#define LED3 LATBbits.LATB4   //Define LED3
#define LED1 LATBbits.LATB2

#define beaconLeft PORTAbits.RA2
#define beaconRight PORTAbits.RA3

unsigned int setpoint_distance = 300;   //Distance set point

unsigned int readleftADC(void);     //read left ADC
unsigned int readrightADC(void);    //Read right ADC
void wait10ms(int del);
void turnright90(int dur);
void turnleft90(int dur);
void acc(int dir);
void facebeacon(void);

int main(void)
{
       unsigned char markspace=100;     	//mark space value for 8 PWM (50% mark space ratio)
       TRISAbits.RA2 = 1;    //Port A all inputs
       TRISAbits.RA3 = 1;
       TRISAbits.RA4 = 0;
       TRISAbits.RA5 = 0;
       TRISCbits.RC2 = 0;          //set CCP1(pin13) to an output pin
       TRISCbits.RC1 = 0;
       TRISBbits.RB0 = 0;
       TRISBbits.RB1 = 0;
       
       PR2 = 0b11111111 ;    	//set period of PWM
       T2CON = 0b00000111 ;   //Timer 2(TMR2) on, Prescaler = 16
       CCP1CON = (0x0c);        //0x0c enables PWM module CCP1
       CCP2CON = (0x0c);
       CCPR1L = markspace;  //Load duty cycle into CCP1CON, PWM begins
       CCPR2L = markspace;
       ADCON1=0b00001101;  //Set voltage reference and port A0 as analogue input
       ADCON2 = 0b10000010; // Fosc/32, A/D result right justified
       LATB=0;  
       TRISB=0b11000000; //Turn all Leds off
       
       
       while(1){
           facebeacon();
            if(readleftADC() <= setpoint_distance && readrightADC() <= setpoint_distance){  //If left hand sensor detects an object
                LED3=0;                       //equal or greater than setpoint_distance
                acc(1);
                wait10ms(10);
            }
            else {   
                    acc(0);      //breaking
                    for(int i=0;i<3;i++){    //flash leds after obstacle detection 
                        LED3=1;
                        wait10ms(10);
                        LED3=0;
                        wait10ms(10);
                    }
                    wait10ms(10);
                    acc(-1);     //go back for 2s
                    wait10ms(200);
                    turnleft90(100);  
                    acc(1);      // go left for 2s
                    wait10ms(200);
                    turnright90(80);

     }            	
} 
       }
//end main()
void wait10ms(int del){     	 //delay function
	int c;
	for(c=0;c<del;c++)
    	__delay_ms(10);
	return;
}

void acc(int dir)
{
    if(dir == 1){
        rightmotorA1 = 0;     
        rightmotorA2 = 1;
        leftmotorB1 = 0;        
        leftmotorB2 = 1;
    }
    else if(dir == -1){
        rightmotorA1 = 1;
        rightmotorA2 = 0;
        leftmotorB1 = 1;
        leftmotorB2 = 0;
    }
    else if(dir == 0)
    {
        rightmotorA1 = 1;     
        rightmotorA2 = 1;
        leftmotorB1 = 1;
        leftmotorB2 = 1;
    }
}

void facebeacon(void)
{   while(beaconLeft == 1 || beaconRight == 1){ 
        if(IRLeft == 0){
            rightmotorA1 = 0;
            rightmotorA2 = 1;
            leftmotorB1 = 1;
            leftmotorB2 = 1;
        }else if(beaconRight == 0){
            rightmotorA1 = 1;
            rightmotorA2 = 1;
            leftmotorB1 = 0;
            leftmotorB2 = 1;
        }else if(beaconRight == 1 && beaconLeft == 1){
            rightmotorA1 = 1;
            rightmotorA2 = 1;
            leftmotorB1 = 0;
            leftmotorB2 = 1;
        }
    }
}

void turnright90(int dur){
    rightmotorA1 = 1;
    rightmotorA2 = 1;
    leftmotorB1 = 0;
    leftmotorB2 = 1;
    wait10ms(dur);
    return;
}

void turnleft90(int dur){
    rightmotorA1 = 0;
    rightmotorA2 = 1;
    leftmotorB1 = 1;
    leftmotorB2 = 1;
    wait10ms(dur);
    return;
}

unsigned int readleftADC(void) {
    ADCON0 = 0b00000011; //select A/D channel AN0,start conversion
    while (ADCON0bits.GO); //do nothing while conversion in progress
    return ((ADRESH << 8) + ADRESL); //Combines high and low A/D bytes into one
}                                 // value and returns this A/D value 0-1023

unsigned int readrightADC(void) {
    ADCON0 = 0b00000111; //select A/D channel AN0,start conversion
    while (ADCON0bits.GO); //do nothing while conversion in progress
    return ((ADRESH << 8) + ADRESL); //Combines high and low A/D bytes into one
}                                 // value and returns this A/D value 0-1023

