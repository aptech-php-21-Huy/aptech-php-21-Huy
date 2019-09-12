#ifndef __MYWIFI__H
#define __MYWIFI__H

#include "stm8s.h"
#include "stdlib.h"
#include "string.h"

#define LED_PORT	GPIOB
#define LED_PIN		GPIO_PIN_5
/****************/
void Led(bool b);
void delay_us(uint16_t x);
void delay_ms(uint16_t x);
/****************************/
int JAS_IndexOf(const char *text, const char *key);      //tim chuoi trong chuoi

void put_char(int c);    //gui 1 ky tu
void print(unsigned char *string);       //gui chuoi
void print_number(int number);   //gui so

void UART_Rx_Call(void);
bool Check_ip(void);    //kiem tra ip
void khoi_tao_wifi(void);       // setup wifi
void Cipsend(uint8_t id,uint8_t *data); //gui data qua Wifi
void Cmd_IPD(void);     //xu ly cau lenh


#endif

