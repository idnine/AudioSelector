//******************************************************************************
// Audio Input Selector
// Audio Multiplexer : TS3A5017
// Controller : MSP430-G2-553 (20pin)
// Code Composer Studio
// Code by Koo Jonghoe (idnine@gmail.com, AudioCookie)
// Feb. 5, 2014
//******************************************************************************
#include <msp430.h>

	const	int		LED1 = 0x08;	// P1.3
	const	int		LED2 = 0x10;	// P1.4
	const	int		LED3 = 0x20;	// P1.5
	const	int		SEL  = 0x40;	// P1.6
	const	int		MUTE = 0x80;	// P1.7
	const	int		EN   = 0x01;	// P2.0
	const	int		IN1  = 0x02;	// P2.1
	const	int		IN2  = 0x04;	// P2.2

	const	int 	maxCh = 3;		// Audio Input Ch. Count

volatile	int 	ledCh = 0;
volatile	int 	selectedCh = 1;
volatile	int 	isMute = 0;

void setLedCh(int ch)
{
	switch(ch)
	{
		default :
		case 1 : ledCh = LED1; break;
		case 2 : ledCh = LED2; break;
		case 3 : ledCh = LED3; break;
	}
}

int main(void)
{
	// Watchdoc OFF, Internal Clock 1 MHz Set
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	// IO Setup
	P1DIR |= (LED1 + LED2 + LED3);	// Output Setup
	P2DIR |= (EN + IN1 + IN2);		// Output Setup
	P1DIR &= ~(SEL + MUTE);			// Input Switch Setup
	P1REN |= (SEL + MUTE);			// Internal Pull-Up Enable
	P1OUT |= (SEL + MUTE);			// Internal Pull-Up

	// Interrupt Setup
	P1IES = (SEL + MUTE);			// Interrupt Edge Selector, P1.3 High -> Low
	P1IFG &= ~(SEL + MUTE);			// Interrupt Flag Clear
	P1IE  = (SEL + MUTE);			// Interrupt Enable

	// Initial Value Output
	P1OUT &= ~(LED1 + LED2 + LED3);
	P1OUT |= LED1;
	P2OUT &= ~(EN + IN1 + IN2);		// Default Ch1 Selected

	// Global Interrupt Enable, START
	_EINT();
	//_BIS_SR(LPM4_bits + GIE); // This is Power Save Mode

	volatile int i;
	volatile unsigned int j;

	while(1)
	{
		for(i=maxCh; i > 0; i--)
		{
			setLedCh(i);

			if(i == selectedCh)
			{
				if(isMute)
				{
					// 해당 채널 Blink
					P1OUT ^= ledCh;
					for(j=20000; j>0; j--);
				}
				else
				{
					// 해당 채널 ON
					P1OUT |= ledCh;
				}
			}
			else
			{
				// 현재 채널 OFF
				P1OUT &= ~ledCh;
			}
		}
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void swAction(void)
{
	if(P1IFG & SEL)
	{
		// Channel Change
		selectedCh++;
		if(selectedCh > maxCh) selectedCh = 1;
		switch(selectedCh)
		{
			default:
			case 1 :
				P2OUT &= ~(IN1 + IN2);
				break;
			case 2 :
				P2OUT &= ~IN2;
				P2OUT |= IN1;
				break;
			case 3 :
				P2OUT &= ~IN1;
				P2OUT |= IN2;
				break;
		}
		P1IFG &= ~SEL;
	}
	else if(P1IFG & MUTE)
	{
		// Mute Toggle
		if(isMute)
		{
			isMute = 0;
			P2OUT &= ~EN;
		}
		else
		{
			isMute = 1;
			P2OUT |= EN;
		}
		P1IFG &= ~MUTE;
	}
	else
		P1IFG = 0x00;
}
