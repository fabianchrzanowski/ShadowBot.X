/* 
 * file:        general.c 
 * authors:     James Bray
 *              Sam Davis
 *              Fabian Chrzanowski
 * project:     ShadowBots 2023/24
 * description: Navigates a two wheel shadowbot through obstacles towards
 *              a IR beacon using beacon and proximity sensors
 */

// INITIALISATION
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#pragma config OSC = HS 
#pragma config WDT = OFF 
#pragma config LVP = OFF 
#pragma config PWRT = ON 

// define clock frequency for '__delay_ms()'
#define _XTAL_FREQ 10000000 

// assign references for H-bridge pins
#define leftmotorB1 LATAbits.LATA4
#define leftmotorB2 LATAbits.LATA5
#define rightmotorA1 LATBbits.LATB0
#define rightmotorA2 LATBbits.LATB1

// assign references for LED pins
#define LED1 LATBbits.LATB2
#define LED2 LATBbits.LATB3 
#define LED3 LATBbits.LATB4
#define LED4 LATBbits.LATB5 

// assign references for beacon sensor pins 
#define beaconLeft PORTAbits.RA2
#define beaconRight PORTAbits.RA3

// assign references for encoder pins
#define encoderLeft PORTCbits.RC0
#define encoderRight PORTCbits.RC5


// VARIABLES
unsigned int setpoint_distance = 300;  // set minimum distance for obstacles
float ang2ticks = 2.6;                 // ratio of encoder ticks to degrees
int motor_speed = 125;                 // motor speed (0-255)

// FUNCTIONS
unsigned int readleftADC(void);  // read left proximity sensor
unsigned int readrightADC(void); // read right proximity sensor
void wait10ms(int del);          // delay to program for del*10 milliseconds
void turn(int dir, int ang);     // turns the robot
                                 //     dir: 1 for right, -1 for left
                                 //     ang: 0 for endless, number for angle
void acc(int dir, int dur);      // accelerate the robot
                                 //     dir: 1 forward, -1 for back, 0 for stop
void flashLEDs(int dur);         // flash all LEDs 3 times over a duration
void facebeacon(void);

// MAIN LOOP
int main(void)
{
    // set ports to input/output
    TRISC = 0b11111001;
    TRISA = 0b00001111;
    TRISB = 0b11000000;
    PR2   = 0b11111111 ;    // set period of PWM
    T2CON = 0b00000111 ;  // timer 2(TMR2) on, Prescaler = 16
    CCP1CON = (0x0c);     // 0x0c enables PWM module CCP1 & CCP2
    CCP2CON = (0x0c);
    CCPR1L = motor_speed; // load duty cycle into CCP1CON and CCP2CON
    CCPR2L = motor_speed;
    ADCON1 = 0b00001101;  // set voltage reference and port A0 as analogue input
    ADCON2 = 0b10000010;  // Fosc/32, A/D result right justified
    LATB = 0;             // turns motors and leds off
   
    // flash LEDS 3 times
    flashLEDs(300);
    
    while(1){
        facebeacon();
        if(readleftADC() >= setpoint_distance || readrightADC() >= setpoint_distance){  //If obstacle
            acc(0, 0);      //breaking
            if (beaconLeft == 1 && beaconRight == 1) { flashLEDs(300); while(1); }
            flashLEDs(100);
            wait10ms(10);
            acc(-1, 100);     //go back for 2s CHANGE to 200
            turn(-1, 60);  
            acc(1, 120);      // go left for 2s CHANGE
            turn(1, 60);
        }
        else {                    
            acc(1, 5);
        }   
    }
}

void wait10ms(int del){     	 //delay function
	int c;
	for(c = 0; c < del; c++) {
    	__delay_ms(10);
    }
	return;
}

void acc(int dir, int dur)
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
    wait10ms(dur);
    return;
}

void flashLEDs(int dur){
    for(int i=0;i<3;i++){    //flash leds after obstacle detection 
        LED1=1; LED2=1; LED3=1; LED4=1;
        wait10ms(dur/6);
        LED1=0; LED2=0; LED3=0; LED4=0;
        wait10ms(dur/6);
    }
}

// rotates the robot to face the beacon
void facebeacon()
{   
    int count = 0;
    int temp = encoderLeft;
    while(beaconLeft == 1 || beaconRight == 1){ // while beacon not detected
        turn(beaconLeft * 2 - 1, 0); // if turn towards beacon
        
        // count ticks
        if (temp != encoderLeft){ count++; } 
        temp = encoderLeft;
        
        // if it can't see the beacon, stop code
        if (count > 360 * ang2ticks) { flashLEDs(300); acc(0, 0); while(1); }
    }
}


void turn(int dir, int ang){
    int ticks = ang * ang2ticks;
    int count = 0;
    if (dir == 1){
        rightmotorA1 = 1;
        rightmotorA2 = 1;
        leftmotorB1 = 0;
        leftmotorB2 = 1;
        
        int temp = encoderLeft;
        while (count < ticks){
            if (temp != encoderLeft){ count++; } 
            temp = encoderLeft;
        }
    }
    else if (dir == -1){
        rightmotorA1 = 0;
        rightmotorA2 = 1;
        leftmotorB1 = 1;
        leftmotorB2 = 1;
        
        int temp = encoderRight;
        while (count < ticks){
            if (temp != encoderRight){ count++; } 
            temp = encoderRight;
        }
    }
    
    if (ang != 0){
        rightmotorA1 = 1;
        rightmotorA2 = 1;
        leftmotorB1 = 1;
        leftmotorB2 = 1;
    }

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

