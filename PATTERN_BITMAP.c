#include <msp430.h>

volatile unsigned long int ms = 0;
volatile unsigned long int duration = 0;
volatile unsigned long int count = 0;
const unsigned short int a[]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned short int b[] = {32,0,64,0,128,128,128,0,0,128,128,0,128,64,64,0,64,64,128,0,32,0,8,8,0,32,64,128,128,0,0,0,128,128,0,128,64,64,64,64,64,128,0,64,32,32,32,0,64,128,0,128,0,0,128,128,0,128,128,64,64,64,64,0,192,32,0,0,8,8,32,32,64,128,128,0,0,128,128,0,0,0,64,64,64,64,0,128,64,0,32,32,0,64,0,128,128,128,0,0,128,128,0,128,64,64,0,64,64,128,0,32,0,8,8,0,32,64,128,128,0,0,0,128,128,0,128,192};
volatile unsigned short int i =0;
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
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    duration = ms>>7;
    count = 0;
    ms = 0;
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

  if(duration == count)
  {
         i++;
         P1OUT =  0xFF &(~a[i]);
         P2OUT = (0xFF &(~b[i]))&(0xF7);
         count = 0;
         if(i==127)
               i=0;
  }

  CCR0+=1000;
}
