#include<lpc21xx.h>
#include "lcd.h"
#include "uart.h"
#define BUZZER 1<<12 

int S = 0, T = 0, Dt = 20;
unsigned char detected = 0;

void START_isr(void) __irq
{
    EXTINT = 0x01;        
    T0PR   = 15000-1;
    T0TCR  = 0x01;         
    VICVectAddr = 0;
}

void STOP_isr(void) __irq
{
    EXTINT = 0x02;        
    T = T0TC;	
		T0TCR = 0x03;
    T0TCR = 0x00;
		S = (Dt * 1000) / T;
		detected = 1;
    VICVectAddr = 0;
}

int main()
{
	IODIR0 |= BUZZER;

  INIT();               
  UART1_CONFIG();       
	
  // Interrupt Configuration
	
  PINSEL1 |= 0x00000001;       
  PINSEL0 |= 0x20000000;
	
	VICIntSelect = 0;         

  VICVectCntl0 = (0x20)|14;   
	VICVectAddr0 = (unsigned long)START_isr;

  VICVectCntl1 = (0x20)|15;  
  VICVectAddr1 = (unsigned long)STOP_isr;
	
	EXTMODE  = 0x03;          
  EXTPOLAR = 0x00; 

  VICIntEnable = (1<<14)|(1<<15);
	
	while(1)
	{
		if(detected)
		{
			detected = 0;
			COMMAND(0x01);
			if(S > 40) 
			{
				COMMAND(0x80);
				lcd_str("OVERSPEED");
				COMMAND(0xC0);
				lcd_str("DETECTED");
				IOSET0 = BUZZER;
				TXSTR("AT\r");
				delay(500);
				TXSTR("AT+CMGF=1\r");
				delay(500);
				TXSTR("AT+CMGS=\"8489593859\"\r");
				delay(500);
				TXSTR("OVERSPEED DETECTED");
				delay(500);
				TX(0x1A);
				delay(500);
				IOCLR0 = BUZZER;
			}
			else
			{
				COMMAND(0x80);
				lcd_str("SPEED NORMAL");
				IOCLR0 = BUZZER;
			}
			delay(2000);
		}
		else
		{
		  /*str("PROJECT TOPIC:");
			str("INTELLIGENT OVER");
			str("SPEED DETECTION");
			str("USING ARM & GSM");*/
			COMMAND(0x80);
			lcd_str("|  PROJ  TOPIC |");
			COMMAND(0xC0);
			lcd_str("OVERSPEED SYSTEM");
		} 
	}	
}

