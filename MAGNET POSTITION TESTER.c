#include <msp430g2553.h>
#include <stdint.h>
#include <intrinsics.h>

volatile unsigned int ms = 0;
volatile unsigned duration = 0;
volatile uint8_t led1=0x01;

//const uint8_t a[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};// for P1OUT
//const uint8_t b[256]= {128,0,1,1,2,4,8,64,0,0,32,32,32,32,64,128,64,8,4,2,0,1,1,1,1,1,0,2,0,32,128,128,64,0,128,128,64,0,0,32,4,2,0,1,0,128,128,0,1,0,2,4,32,128,64,0,32,32,32,0,64,128,32,0,2,0,1,1,1,1,1,1,2,4,8,64,128,0,64,128,128,64,64,128,192,8,4,2,1,1,0,128,128,1,1,2,4,8,64,0,0,32,32,32,32,64,128,64,8,4,2,0,1,1,1,1,1,0,2,4,32,128,128,64,0,128,128,64};// for P2OUT
void pinsetup(void)
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
void timer1_setup(void)
{
    CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TACTL = TASSEL_2 + MC_1;
    CCR0 =  1000;
}
void set_clk_to_1MHz(void)
{
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}
void do_stuff()
{
    led1 = led1<<1;
}
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT
    pinsetup();
    set_clk_to_1MHz();
    timer1_setup();

    __enable_interrupt();
    while(1)
    {
        if(led1==0x00 || led1>0x80)
            {
                  led1=0x01;
            }
            P1OUT = 0xFF & (~led1);
       if(duration==ms-1)
           do_stuff();
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    duration = ms>>7;
    ms = 0;
    P2IFG &= ~BIT3;
    //__bic_SR_register_on_exit(LPM0_bits + GIE);
}
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A_ISR (void)
{
  ms +=1;
}
