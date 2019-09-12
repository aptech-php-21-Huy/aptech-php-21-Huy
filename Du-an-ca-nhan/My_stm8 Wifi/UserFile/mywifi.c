#include "mywifi.h"

/************khai bao bien***************/
uint8_t Wifi_Pass[]="\"Van Hoang\",\"1656898119\"";
uint8_t IP[30],IPD[50];
uint8_t Rx_temp[30];
uint8_t Rx_data[6],Rx_sta,ind=0,ind_Text; ////Khai bao cac bien de nhan du lieu 

bool Ok=0,Error=0,Link=0,info_IPD=0;

uint8_t Client;

/************cac chuong trinh*********/
void Led(bool b)
{

  if(b)         GPIO_WriteHigh(LED_PORT,LED_PIN);
    else        GPIO_WriteLow(LED_PORT,LED_PIN);
}
void delay_us(uint16_t x)
{
     while(x--)
     {
           //nop();
           //nop();
           //nop();
           //nop();
           //nop();
           //nop();
           nop();
           nop();
           nop();
           nop();
           nop();
     }
}
void delay_ms(uint16_t x)
{
     while(x--)
     {
        delay_us(1000);
     }
}

int JAS_IndexOf(const char *text, const char *key)      //tim chuoi trong chuoi
{
	int index = -1;
	char *found = strstr( text, key);
	if (found != NULL) index = found - text;
	return index;
}
/*********gui chuoi qua RXTX*******/
void put_char(int c)    //gui 1 ky tu
{
  /* Write a character to the UART1 */
  UART1_SendData8(c);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET); //cho cho ki tu gui xong
}

void print(unsigned char *string)       //gui chuoi
{
  while(*string)
  {    
    put_char(*string); 
    string++;
  }
}

void print_number(int number)   //gui so
{
 int num,n=0,e; 
  do
  {
    n++;
    e=1;
    for(int j=0;j<n;j++)  e=e*10;
  }
  while(number/e>0); //dem bao nhieu chu so
  
  while(n>0)
  {
    n--;
    e=1;
    for(int j=0;j<n;j++)  e=e*10;    //thay the pow(10,n) bi sai so 
    num=number/e;
    put_char(num+48);
    number=number-(num*e);
  } 
}

//***********ngat uart nhan du lieu***********//
void UART_Rx_Call(void)
{
    switch (Rx_sta)
  {
  case 0:
    {
      
      break;
    }
  case 1: // nhan IP
    {
      IP[ind]=Rx_data[4]; ind++;
      break;
    }
  case 2: // nhan data qua Wifi
    {
      IPD[ind]=Rx_data[4]; ind++;
      break;
    }
  }
  
  for(uint8_t j=0;j<5;j++) Rx_data[j]=Rx_data[j+1]; // sap xep lai du lieu
  //kiem tra cac dieu kien
  if(strcmp(Rx_data,"OK\r\n")==0) //OK , ket thuc lenh
  {    
    if(Rx_sta==2) info_IPD=1; // bat thong bao nhan lenh +IPD
    Rx_sta=0;Ok=1;    
  }
  else if(strcmp(Rx_data,"ROR\r")==0) Error=1; //ERROR
  else if(strcmp(Rx_data,"Link")==0) Link=1; //Link
  else if(strcmp(Rx_data,"FSR\r")==0) {Rx_sta=1;ind=0;} //AT+CIFSR nhan ip
  else if(strcmp(Rx_data,"+IPD")==0) {Rx_sta=2;ind=0;} //+IPD nhan data qua Wifi  

}

/////////gui data qua Wifi////////////
void Cipsend(uint8_t id,uint8_t *data)

{
  print("AT+CIPSEND="); put_char(id); print(","); print_number(strlen(data)); print("\r\n");
  delay_ms(100);
  print(data);

}

bool Check_ip(void)
{
  print("AT\r\n");  delay_ms(1000);
  print("AT+CIFSR\r\n"); delay_ms(1000); // xem ip
  if(JAS_IndexOf(IP,"0.0.0.0")==-1) {return TRUE;} // da ket noi , co ip
  else {return FALSE;} //chua ket noi
}
void khoi_tao_wifi(void)
{
  delay_ms(10000); // cho cho esp8266 khoi dong
  if(Check_ip()) //neu da ket noi wifi
  {
    print("AT+CWMODE=1\r\n"); delay_ms(1000); // chon chuc nang client cho esp
    print("AT+CIPMUX=1\r\n"); delay_ms(1000); // configure for multiple connections
    print("AT+CIPSERVER=1,80\r\n"); delay_ms(1000); // mo port 80  
  }
  else
  {
    print("AT+CWMODE=3\r\n"); delay_ms(1000); // chon chuc nang client+Access Point cho esp
    print("AT+CIPMUX=1\r\n"); delay_ms(1000); // configure for multiple connections
    print("AT+CIPSERVER=1,80\r\n"); delay_ms(1000); // mo port 80 
  }

}

void Cmd_IPD(void)
{
  if(info_IPD) // neu nhan duoc lenh IPD
  {
    info_IPD=0; //tat thong bao
    Client=IPD[1];
    if(JAS_IndexOf(IPD,"HELLO\r\n")!=-1) Cipsend(Client,"OK\r\n");
    else if(JAS_IndexOf(IPD,"LED ON\r\n")!=-1)
      { 
        Led(0);
        Cipsend(Client,"LED ON OK\r\n");      
      }
    else if(JAS_IndexOf(IPD,"LED OFF\r\n")!=-1)
      { 
        Led(1);
        Cipsend(Client,"LED OFF OK\r\n");      
      }
  
  }

}