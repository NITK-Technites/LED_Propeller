#include <msp430.h>

volatile unsigned long int ms = 0;
volatile unsigned long int duration = 0;
volatile unsigned long int count = 0;
volatile unsigned short int led1=0x01;
volatile unsigned short int i=0;
void pin_setup()
{
    P1DIR = 0XFF;
    P2DIR = 0xF7;
    P1OUT = 0x00;
    P2OUT = 0x00;
    P2SEL &= ~(BIT6+BIT7);
    P2REN |= BIT3;
    P2IE  |= BIT3;
    P2IES &= ~BIT3;
    P2IFG &= ~BIT3;
    P1OUT = 0xFF;
    P2OUT = 0xF7;

}
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  pin_setup();

  DCOCTL = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 1000;
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode

//  __enable_interrupt();
//  while(1)
//  {
//
//      //__delay_cycles(100);
//  }
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    duration = ms>>7;
    count = 0;
    ms = 50;
    //P1OUT = 0x00;
    P2IFG &= ~BIT3;
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  ms+=1;
  count+=1;
  if(duration==count)
  {
       if(i==0x01)
       {
           P1OUT = 0b10101010;
           
       }
       else
       {
           P1OUT = 0b01010101;
       }
       i^=0x01;
       count = 0;
  }
  CCR0+=1000;
}
