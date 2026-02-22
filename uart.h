void TX(unsigned char);
void UART1_CONFIG(void);
void TXSTR(unsigned char *);

void UART1_CONFIG()
{
	PINSEL0 |= 0x50000;
	U1LCR    = 0x83;
	U1DLL    = 97;
	U1DLM    = 0;
	U1LCR    = 0x03;
}

void TXSTR(unsigned char *str)
{
	while(*str)
		TX(*str++);
}

void TX(unsigned char txbyte)
{
	while((U1LSR&(1<<5))==0);
	U1THR = txbyte;
}

