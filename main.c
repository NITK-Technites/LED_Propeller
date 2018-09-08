#include <msp430g2553.h>
#include <stdint.h>
#include <intrinsics.h>
uint16_t prev = 0;
uint16_t present;
uint16_t duration;
static uint8_t i; // increments through column
uint8_t a[256];// for P1OUT
uint8_t b[256];// for P2OUT
void pinsetup(void)
{
    P1DIR = 0XFF;
    P2DIR = 0xFF & ~(BIT3);
    P2SEL = BIT3;
    P1OUT = 0x00;
    P2OUT = 0x00;
}
void timerB_setup(void)
{
    //Rising edge | CCI0B as input | Sync Capture| Capture Mode | Interrupt enabled
    TA1CCTL0 = CM_1 | CCIS_1 | SCS | CAP | CCIE ;
    //Start timer : ACLK , no prescale , continuous mode , no ints , clear
    TA1CTL = TASSEL_1 | ID_0 | MC_2 | TACLR ;
}
void timerA_setup(void)
{
    //TA1CCTL1 = CM_0 | SCS | CCIE;
    // Start timer: ACLK , no prescale , continuous mode , no ints , clear
    //TA1CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TAIE;
}
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT
    pinsetup();
    timerA_setup();
    timerB_setup();
    for(; ;)
    {
        __low_power_mode_3();
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    present = TA0CCR0;
    duration = present - prev;
    prev = present;
    TA0CCR0=duration>>7;
    
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void) // Flag cleared automatically
{
 
  P1OUT=a[i];
  P2OUT=b[i];
  i++;
}
