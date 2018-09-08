#include <msp430.h>

volatile unsigned long int ms = 0;
volatile unsigned long int duration = 0;
volatile unsigned long int duration_total = 0;
volatile unsigned long int count = 0;
//volatile unsigned short int i=0x00;
volatile unsigned short int count_rev=0;

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
    duration_total = ms;
    duration = ms>>7;
    count = 0;
    ms = 0;
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

       // P1OUT = 0xFF & (~led1);
        if(duration==count)
        {
              count_rev++;
              count = 0;
              switch(count_rev)
                      {
                      case 0: P1OUT = ~(0x1)&0xFF;P2OUT=0xFF;break;
                      case 1: P1OUT = ~(0x2)&0xFF;P2OUT=0xFF;break;
                      case 2: P1OUT = ~(0x4)&0xFF;P2OUT=0XFF;break;
                      case 3: P1OUT = ~(0x8)&0xFF;P2OUT=0xFF;break;
                      case 4: P1OUT = ~(0x10)&0xFF;P2OUT=0xFF;break;
                      case 5: P1OUT = ~(0x20)&0xFF;P2OUT=0xFF;break;
                      case 6: P1OUT = ~(0x40)&0xFF;P2OUT=0xFF;break;
                      case 7: P1OUT = ~(0x80)&0xFF;P2OUT=0xFF;break;
                      case 8: P1OUT = 0xFF;P2OUT=~(0x1)&0xFF;break;
                      case 9: P1OUT=0xFF;P1OUT = ~(0x2)&0xFF;break;
                      case 10: P1OUT=0xFF;P1OUT = ~(0x4)&0xFF;break;
                      case 11: P1OUT=0xFF;P1OUT = ~(0xC)&0xFF;break;
                      case 12: P1OUT=0xFF;P1OUT = ~(0x10)&0xFF;break;
                      case 13: P1OUT=0xFF;P1OUT = ~(0x20)&0xFF;break;
                      case 14: P1OUT=0xFF;P1OUT = ~(0x40)&0xFF; count_rev=0;break;
                      }
        }

        CCR0+=1000;
}
