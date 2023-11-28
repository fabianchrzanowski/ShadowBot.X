 /* File:   ADtest.c
 * Author: Craig Bacon     Created on 02 November 2021
 * Reads the left hand IR distance sensor connected to analogue input AN0(Pin2)      
 * If the value of the IR sensor is greater than setpoint_distance, turn on LED3.
 */

#include <xc.h>
#pragma config OSC = HS     //High speed resonator
#pragma config WDT = OFF    //Watchdog timer off
#pragma config LVP = OFF    //Low voltage programmer disabled
#pragma config PWRT = ON    //Power up timer on
#define LED3 LATBbits.LATB4    //Define LED3
unsigned int setpoint_distance = 400;   //Distance set point
unsigned int readleftADC(void);    //Read left ADC
unsigned int readrightADC(void);    //read right adc


int main(void)
{
TRISA=0b11111111;   //Port A all inputs
TRISB=0b00000000;   //Port B all outputs
ADCON1=0b00001101;  //Set voltage reference and port A0 as analogue input
ADCON2 = 0b10000010; // Fosc/32, A/D result right justified
LATB=0;             //Turn all Leds off
while(1){
    if(readleftADC() >= setpoint_distance || readrightADC() >= setpoint_distance)  //If left hand sensor detects an object
        LED3=1;                       //equal or greater than setpoint_distance
    else                              //turn on LED3
        LED3=0;
 }
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


