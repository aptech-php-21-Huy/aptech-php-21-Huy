#include "stm8s.h"


#define RELAY_PORT      GPIOD
#define RELAY_PIN	GPIO_PIN_2

#define IN_PORT		GPIOA
#define IN_PIN		GPIO_PIN_3

#define OUT_PORT	GPIOD
#define OUT_PIN		GPIO_PIN_3

#define LED_PORT	GPIOB
#define LED_PIN		GPIO_PIN_5



uint32_t dem_clk, tan_so;
//volatile int dem;
//volatile int ms,chu_ky, tb[3];
uint8_t dem;
uint32_t ms,chu_ky, tb[3];



/////////////////khai bao cac chuong trinh///////////////
void main(void);

void delay_us(uint16_t x);
void delay_ms(uint16_t x);
void clk_config_16MHz_hsi(void); //clk noi 16MHz
void GPIO_Config(void);         // khai bao chan GPIO
void UART_Config(void);         // khai bao UART 
void print(unsigned char *string);              //gui chuoi qua uart
void println(unsigned char *string);       // gui chuoi qua uart \r \n
void print_number(int number);
void TIM2_Config(void);         //khoi tao timer2
void timer4_init(void);         //khoi tao timer4
void ngat_timer4(void);         //chuong trinh ngat timer4
void ngat_timer2(void);         //chuong trinh ngat timer2
void ngat_port_A(void);         //chuong trinh ngat ngoai port A
void chon_tan_so(uint32_t hz);
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
   GPIO_Init(LED_PORT, LED_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST ); 
   GPIO_Init(RELAY_PORT, RELAY_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST );
   //GPIO_Init(OUT_PORT, OUT_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST );
   
   //     IN
   GPIO_Init(IN_PORT,IN_PIN,GPIO_MODE_IN_PU_IT);
   
   //cho phep ngat ngoai GPIO
   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_FALL_ONLY);       // port A
  
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
void timer2_init(void)
{
        TIM2_DeInit();
	TIM2_TimeBaseInit(TIM2_PRESCALER_128,124);  
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
	//enableInterrupts();
	//TIM2_Cmd(ENABLE);
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 200, TIM2_OCPOLARITY_HIGH);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);
    enableInterrupts();
    TIM2_Cmd(ENABLE);
}
void timer4_init(void)
{
	TIM4_TimeBaseInit(TIM4_PRESCALER_128,12); // 0.1ms
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);

	enableInterrupts();
	TIM4_Cmd(DISABLE);
}
void ngat_timer4(void)  
{
  if(TIM4_GetITStatus(TIM4_IT_UPDATE))
   {     
      ms++;
      if(ms>10000)
      {
       // chu_ky=0;
        tan_so=0;
        ms=0;
        TIM4_Cmd(DISABLE);
      }
      TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
   }
}
void ngat_timer2(void)  
{
  if(TIM2_GetITStatus(TIM2_IT_UPDATE))
   {
      //GPIO_WriteReverse(OUT_PORT, OUT_PIN);
      TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
   }
}
void ngat_port_A(void)
{
  if(GPIO_ReadInputPin(IN_PORT,IN_PIN)==0)
  {
    if(ms==0) TIM4_Cmd(ENABLE);
      chu_ky=ms; //100us
      ms=0; 
      if(chu_ky!=0)
      {
        tb[dem]=chu_ky;
        dem++;
        if(dem==3) dem=0;
        chu_ky=(tb[0]+tb[1]+tb[2])/3;
        
        tan_so=10000/chu_ky;
      }
      else
      {
        tan_so=0;
      }
      
  }
}
void chon_chu_ky(uint32_t T)
{
  //long int tem;
  uint32_t tem;
  if(T>8)  // <8 bi treo tim2
  {
    tem=T*4/10;
    tem=tem*9/10;
    TIM2_TimeBaseInit(TIM2_PRESCALER_4096, tem);
    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,(tem)/2, TIM2_OCPOLARITY_HIGH);
    
    TIM2_Cmd(ENABLE);  //kich hoat lai tim2
  } 

}

static void TIM2_Config(void)
{
  /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_1024,16);

  
  /* PWM1 Mode configuration: Channel3 */         
  TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,8, TIM2_OCPOLARITY_HIGH);
  TIM2_OC2PreloadConfig(ENABLE);


  TIM2_ARRPreloadConfig(ENABLE);

  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
}
void IWDG_setup(void)
{
     IWDG_Enable();
     IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
     IWDG_SetPrescaler(IWDG_Prescaler_256);
     IWDG_SetReload(0x99);
     IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
}
///////////////////////////////////////////////////////////////////////////////
void main(void)
{
  clk_config_16MHz_hsi();
  //UART_Config();
  GPIO_Config();
  timer4_init();
  TIM2_Config();
  //chon_chu_ky(0);
  //delay_ms(500);
  //GPIO_WriteLow(RELAY_PORT,RELAY_PIN);
  IWDG_setup();
 

  
  while (1)
  {
    
   IWDG_ReloadCounter();
    
    if(tan_so>0)
    {
      
      
      chon_chu_ky(chu_ky);
      delay_ms(10);
      GPIO_WriteLow(RELAY_PORT,RELAY_PIN);
      
      //tan_so=0;
    }
    else
    {
      GPIO_WriteHigh(RELAY_PORT,RELAY_PIN); 
      
      TIM2_Cmd(DISABLE);  // stop tim2
    }    
    
    GPIO_WriteReverse(LED_PORT,LED_PIN);
    delay_ms(300);
    /*
    GPIO_WriteLow(RELAY_PORT,RELAY_PIN);
    chu_ky=3837;
    chon_chu_ky(chu_ky);
    */
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
