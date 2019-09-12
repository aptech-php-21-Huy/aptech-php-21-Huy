#include "stm8s.h"
#include "mywifi.h"


/* Declaring Global Variables */



/* Declaring Function Prototype */

void GPIO_Config(void)         
{
  GPIO_Init(LED_PORT, LED_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST );
}
void UART_Config(void)
 {
  UART1_DeInit();
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
  UART1_Cmd(ENABLE);
  enableInterrupts();
 
 }
void clk_config_16MHz_hsi(void)
{
	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}
void main(void)
{
  clk_config_16MHz_hsi();
  UART_Config();
  GPIO_Config();
  
  khoi_tao_wifi();
  while (1)
  {
      Cmd_IPD();
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
