
/* Code to test 8 bit Shift Register LED Driver TLC5916
 * P1.2 -> Data out to 5916
 * P1.4 -> Clock to 5916
 */

#define latch BIT5
#include <msp430.h>
void latch_data()
{
    while (!(IFG2 & UCA0TXIFG));                //wait for buffer to be empty before latching
    P1OUT &=~latch;
    P1OUT |= latch;
}
void spi_setup()
{
    P1SEL = BIT1 | BIT2 | BIT4;
    P1SEL2 = BIT1 | BIT2 | BIT4;
    UCA0CTL1 = UCSWRST;
    UCA0CTL0 |= UCCKPL+UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
    /* Data is changed on the first UCLK edge and captured on the following edge,
     * clock polarity - inactive state is high,MSB first, in master mode, Synchronous mode */
    UCA0CTL1 |= UCSSEL_2;                       // SMCLK
    UCA0BR0 |= 0x02;                            // /2
    UCA0BR1 = 0;
    UCA0MCTL = 0;                               // No modulation
    UCA0CTL1 &= ~UCSWRST;                       // **Initialize USCI state machine**
}
void spi_senddata(volatile unsigned short int data)
{
    while (!(IFG2 & UCA0TXIFG));            // USCI_A0 TX buffer ready?
    UCA0TXBUF = data;
}
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT
    P1OUT = 0x00;
    P1DIR |= latch;
    spi_setup();
    volatile unsigned short int data=0x01;
    while(1)
    {
        spi_senddata(data);
        latch_data();
        data = data<<1;
        if(data==0)
            data=0x01;
        __delay_cycles(1000000);
    }
}


