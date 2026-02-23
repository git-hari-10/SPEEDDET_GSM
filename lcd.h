#define D  0xff
#define RS 1<<10
#define E  1<<11

void INIT(void);
void COMMAND(unsigned char);
void DATA(unsigned char);
void str(unsigned char*);
void lcd_str(unsigned char*);
void delay(int);

void INIT(void)
{
	IODIR0|=RS|E|D;
	COMMAND(0X01);
	COMMAND(0X02);
	COMMAND(0X0C);
	COMMAND(0X38);
}
void COMMAND(unsigned char cmd)
{
	IOCLR0=D;
	IOCLR0=RS;
	IOSET0=cmd;
	IOSET0=E;
	delay(2);
	IOCLR0=E;
}
void DATA(unsigned char DATA)
{
	IOCLR0=D;
	IOSET0=RS;
	IOSET0=DATA;
	IOSET0=E;
	delay(2);
	IOCLR0=E;
}
void delay(int ms)
{
	int i;
	for(;ms>=0;ms--)
		for(i=12000;i>=0;i--);
}

void lcd_str(unsigned char *s)
{
	while(*s)
		DATA(*s++);
}



