#include "stm8s.h"
#include "main.h"

///////////////////cac chuong trinh////////////////////////

void clk_config_16MHz_hsi(void)
{
	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
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
static void GPIO_Config(void)          
{
   //    OUT
   //GPIO_Init(LED_PORT, LED_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST ); 
   
  GPIO_Init(GPIOA,(GPIO_Pin_TypeDef)(GPIO_PIN_3),  GPIO_MODE_OUT_PP_LOW_FAST );
  GPIO_Init(GPIOC,(GPIO_Pin_TypeDef)( GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7), GPIO_MODE_OUT_PP_LOW_FAST );
  GPIO_Init(GPIOD,(GPIO_Pin_TypeDef)( GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6),  GPIO_MODE_OUT_PP_LOW_FAST );
   //     IN
   GPIO_Init(SW1_PORT,SW1_PIN,GPIO_MODE_IN_PU_NO_IT);
   
   //cho phep ngat ngoai GPIO
//   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_FALL_ONLY);       // port A
  
}
static void UART_Config(void)
 {
  UART1_DeInit();
  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);      // chi truyen TX
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
  UART1_Cmd(ENABLE);
  //enableInterrupts();
 
 }
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
void println(unsigned char *string)     // gui chuoi xuong dong
{
  while(*string){
    
    put_char(*string);
    string++;
  }
  put_char('\n');                        //xuong dong
  put_char('\r');                        //tro ve dau dong
}
void print_number(int number)
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
void IWDG_setup(void)
{
     IWDG_Enable();
     IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
     IWDG_SetPrescaler(IWDG_Prescaler_256);
     IWDG_SetReload(0x99);
     IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
}
static void TIM2_Config(void)   //khoi tao timer2
{
//  TIM2_TimeBaseInit(TIM2_PRESCALER_512,3124);//100ms
  TIM2_TimeBaseInit(TIM2_PRESCALER_1024,15624);
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);

  enableInterrupts();
  TIM2_Cmd(ENABLE);
}
void ngat_timer2(void)        //chuong trinh ngat timer2
{
  if(TIM2_GetITStatus(TIM2_IT_UPDATE))
  {
    seconds++;  // giay
    if(seconds>59)
    {
      minute++; seconds=0;      //phut
    }
    if(minute>59)
    {
      hours++; minute=0;        //gio
    }
    
    GPIO_WriteReverse(LED_PORT,LED_PIN);
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
  }

}
static void TIM4_Config(void)                   //khoi tao timer4
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_128,124); // 1ms
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);

  enableInterrupts();
  TIM4_Cmd(ENABLE);
}
void ngat_timer4(void)                   //chuong trinh ngat timer4
{
  if(TIM4_GetITStatus(TIM4_IT_UPDATE))
  {
    ms++;
    if(ms>999)
    {
      ms=0;
      seconds++;  // giay
      if(seconds>59)
      {
        seconds=0;
        minute++;       //phut
        if(minute>59)
        {
          hours++; minute=0;        //gio
          if(hours>23) hours=0;
        }
      }  
//      LED_Time(sys); // cap nhat thoi gian led out
    }
    ////////////////////
    if(ms%200==0) GPIO_WriteReverse(LED_PORT,LED_PIN);
    // out led 
    LED_Time(sys); // cap nhat thoi gian led out
    LED_7SEG(Code7Seg[LED_DATA[ms%4]]);
    LED_DD(ms%4);
    
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
  }
}

void LED_DD(uint8_t num)               // chon led D1~D4
{
  // tat het led D1~D4
  GPIO_WriteHigh(D1_PORT,D1_PIN);
  GPIO_WriteHigh(D2_PORT,D2_PIN);
  GPIO_WriteHigh(D3_PORT,D3_PIN);
  GPIO_WriteHigh(D4_PORT,D4_PIN);
  
  switch(num)
  {
  case 0:
    GPIO_WriteLow(D1_PORT,D1_PIN);   
    break;
  case 1:
    GPIO_WriteLow(D2_PORT,D2_PIN);
    break;
  case 2:
    GPIO_WriteLow(D3_PORT,D3_PIN);
    break;
  case 3: 
    GPIO_WriteLow(D4_PORT,D4_PIN);
    break;
  default:
  }
}
void LED_Dec(uint16_t number)                // nhap du lieu so thap phan vao LED_DATA[]
{
  LED_DATA[0]=number/1000;
  LED_DATA[1]=(number%1000)/100;
  LED_DATA[2]=(number%100)/10;
  LED_DATA[3]=(number%10);
}
void LED_Time(uint8_t Mode)         // nhap du lieu thoi gian vao LED_DATA[]
{
    switch(Mode)
    {
    case 0:     // che do phut giay                  
      LED_DATA[0]=minute/10;
      LED_DATA[1]=minute%10;
      LED_DATA[2]=seconds/10;
      LED_DATA[3]=seconds%10;
      break;
    case 1:     // che do gio phut                  
      LED_DATA[0]=hours/10;
      LED_DATA[1]=hours%10;
      LED_DATA[2]=minute/10;
      LED_DATA[3]=minute%10;
      if(seconds%2) LED_DATA[1]=LED_DATA[1]+10;
      break;
    }
}
void LED_7SEG(uint8_t b)               //xuat du lieu ra port led 7 doan
{
  if(b&0x01) GPIO_WriteHigh(A_PORT,A_PIN); else GPIO_WriteLow(A_PORT,A_PIN);
  if(b&0x02) GPIO_WriteHigh(B_PORT,B_PIN); else GPIO_WriteLow(B_PORT,B_PIN);
  if(b&0x04) GPIO_WriteHigh(C_PORT,C_PIN); else GPIO_WriteLow(C_PORT,C_PIN);
  if(b&0x08) GPIO_WriteHigh(D_PORT,D_PIN); else GPIO_WriteLow(D_PORT,D_PIN);
  if(b&0x10) GPIO_WriteHigh(E_PORT,E_PIN); else GPIO_WriteLow(E_PORT,E_PIN);
  if(b&0x20) GPIO_WriteHigh(F_PORT,F_PIN); else GPIO_WriteLow(F_PORT,F_PIN);
  if(b&0x40) GPIO_WriteHigh(G_PORT,G_PIN); else GPIO_WriteLow(G_PORT,G_PIN);
  if(b&0x80) GPIO_WriteHigh(H_PORT,H_PIN); else GPIO_WriteLow(H_PORT,H_PIN);
}

///////////////////////////////////////////////////////////////////////////////
void main(void)
{
  clk_config_16MHz_hsi();
  //UART_Config();
  GPIO_Config();
  //TIM2_Config();
  TIM4_Config();
  //IWDG_setup();
//  sys=1;     //hien thi gio phut
  sys=0;    // hien thi phut giay
  while (1)
  {
    /*
     huong dan su dung:
        + nhan nhieu lan <2 giay de tang gia tri
        + nhan giu 2<seconds<5 de chuyen che do sys=1|sys=0 (gio|phut)
        + nhan giu seconds>5 de tang gia tri lien tuc
    */
    switch(sys)
    {
    case 0:
      if(GPIO_ReadInputPin(SW1_PORT,SW1_PIN)==0)
      {
        SW_time=0;
        while(GPIO_ReadInputPin(SW1_PORT,SW1_PIN)==0)
        {
            SW_time++;
            delay_ms(100);
            if(SW_time>50) minute++; //>5giay
        }
        if(SW_time<20) {minute++;} //<2giay 
        else
        {
          if(SW_time<50) sys=1; // 2giay<SW_time<5giay chuyen che do sys=1        
        } 
        if(minute>59) minute=0;
      }
      break;
    case 1:
       if(GPIO_ReadInputPin(SW1_PORT,SW1_PIN)==0)
      {
        SW_time=0;
        while(GPIO_ReadInputPin(SW1_PORT,SW1_PIN)==0)
        {
            SW_time++;
            delay_ms(100);
            if(SW_time>50) hours++; //>5giay
        }
        if(SW_time<20) {hours++;} //<2giay 
        else
        {
          if(SW_time<50) sys=0; // 2giay<SW_time<5giay chuyen che do sys=0       
        }            
      }
      if(hours>23) hours=0;
      break;
      
    }

    


  }
    
}

/* User Function */




#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
