// Ball Potting at Target Basket using PIC18F452 Microcontroller (Line Following Robot)

#include "p18f452.h"
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOR = OFF

#define TRIG PORTCbits.RC7
#define ECHO PORTDbits.RD0

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
void send_pulse();
unsigned int read_distance();
void move_forward();
void stop_motors();
void place_ball();
void init();

void main() {
    unsigned int distance;
    init();
    while (1) {
        distance = read_distance();
        if (distance < 15) {
            stop_motors();
            place_ball();
            delay_ms(2000);
        } else {
            move_forward();
        }
    }
}

void init() {
    TRISB = 0xFF;
    TRISC = 0x00;
    TRISD = 0xFF;
    T0CON = 0b00000111;
    T0CONbits.TMR0ON = 0;
}

void send_pulse() {
    TRIG = 1;
    delay_us(10);
    TRIG = 0;
}

unsigned int read_distance() {
    unsigned int time = 0;
    send_pulse();
    while (!ECHO);
    while (ECHO) {
        delay_us(10);
        time++;
    }
    return time / 58;
}

void move_forward() {
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 0;
    PORTCbits.RC3 = 1;
    PORTCbits.RC4 = 0;
    PORTCbits.RC5 = 1;
    PORTCbits.RC6 = 1;
}

void stop_motors() {
    PORTCbits.RC5 = 0;
    PORTCbits.RC6 = 0;
}

void place_ball() {
    int i;
    for (i = 0; i < 25; i++) {
        PORTCbits.RC2 = 1;
        delay_us(2000);
        PORTCbits.RC2 = 0;
        delay_ms(18);
    }
    delay_ms(700);
    for (i = 0; i < 25; i++) {
        PORTCbits.RC2 = 1;
        delay_us(1500);
        PORTCbits.RC2 = 0;
        delay_ms(18);
    }
    delay_ms(700);
}

void delay_us(unsigned int us) {
    while (us--) {
        _asm
            NOP
            NOP
            NOP
            NOP
            NOP
        _endasm
    }
}

void delay_ms(unsigned int ms) {
    unsigned int i;
    for (i = 0; i < ms; i++) {
        T0CONbits.TMR0ON = 0;
        TMR0H = 0xF8;
        TMR0L = 0x2F;
        INTCONbits.TMR0IF = 0;
        T0CONbits.TMR0ON = 1;
        while (!INTCONbits.TMR0IF);
        T0CONbits.TMR0ON = 0;
    }
}
