#include <reg51.h>

sbit SOS = P1^0;
sbit BUZZER = P1^1;

void delay(unsigned int ms);
void uart_init();
void uart_tx(char ch);
void gsm_cmd(char *str);
void send_sms(char *num);

void main()
{
    SOS = 1;      // input
    BUZZER = 0;

    uart_init();
	
    delay(500);
    gsm_cmd("AT\r\n");
    delay(200);
    gsm_cmd("AT+CMGF=1\r\n");  // Set ONCE here
    delay(200);

    while(1)
    {
			
        if(SOS == 0)      // button pressed
        {
            
                BUZZER = 1;

                send_sms("+919876543210");
                send_sms("+919876543211");
                send_sms("+919876543212");

                BUZZER = 0;

                while(SOS == 0);
            
        }
    }
}


void uart_init()
{
    TMOD = 0x20;
	  PCON = 0x00;
    TH1 = 0xFD;
    TL1 = 0xFD;

    SCON = 0x50;

    TR1 = 1;
	  TI=1;
}
void uart_tx(char ch)
{
	  TI=0;
    SBUF = ch;

    while(TI == 0);

    TI = 0;
}
void gsm_cmd(char *str)
{
    while(*str)
    {
        uart_tx(*str++);
    }
}
void send_sms(char *num)
{
    gsm_cmd("AT\r\n");
    delay(1000);

    gsm_cmd("AT+CMGF=1\r\n");
    delay(1000);

    gsm_cmd("AT+CMGS=\"");
    gsm_cmd(num);
    gsm_cmd("\"\r\n");

    delay(1000);

    gsm_cmd("EMERGENCY ALERT! HELP NEEDED.\r\n");

    uart_tx(26);      // CTRL+Z

    delay(5000);
}
void delay(unsigned int ms)
{
    unsigned int i,j;

    for(i=0;i<ms;i++)
    {
        for(j=0;j<1275;j++);
    }
}