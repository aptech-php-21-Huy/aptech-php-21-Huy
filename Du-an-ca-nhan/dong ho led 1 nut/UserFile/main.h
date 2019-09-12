#ifndef __MAIN__H
#define __MAIN__H

#include "stm8s.h"
#include "stdlib.h"

/////////////dinh nghia/////////////////////

#define LED_PORT	GPIOB
#define LED_PIN		GPIO_PIN_5


///khai bao led 7 doan////////

#define A_PIN   GPIO_PIN_5      //PD5     
#define A_PORT  GPIOD           //PD5

#define B_PIN   GPIO_PIN_3      //PA3
#define B_PORT  GPIOA           //PA3

#define C_PIN   GPIO_PIN_7      //PC7
#define C_PORT  GPIOC           //PC7

#define D_PIN   GPIO_PIN_2      //PD2
#define D_PORT  GPIOD           //PD2

#define E_PIN   GPIO_PIN_3      //PD3 
#define E_PORT  GPIOD           //PD3

#define F_PIN   GPIO_PIN_6      //PD6
#define F_PORT  GPIOD           //PD6

#define G_PIN   GPIO_PIN_6      //PC6
#define G_PORT  GPIOC           //PC6

#define H_PIN   GPIO_PIN_1      //PD1
#define H_PORT  GPIOD           //PD1

#define D1_PIN   GPIO_PIN_4     //PD4
#define D1_PORT  GPIOD           //PD4

#define D2_PIN   GPIO_PIN_4     //PC4
#define D2_PORT  GPIOC           //PC4

#define D3_PIN   GPIO_PIN_3     //PC3
#define D3_PORT  GPIOC           //PC4

#define D4_PIN   GPIO_PIN_5     //PC5
#define D4_PORT  GPIOC          //PC5


#define SW1_PIN   GPIO_PIN_1    //PA1
#define SW1_PORT  GPIOA          //PA1


/////////////khai bao bien///////////////////////
uint8_t sys,test,SW_time;
uint16_t ms;
uint8_t seconds,minute,hours;   //khai bao bien thoi gian
////Seven-segment display///
uint8_t  LED_DATA[4];           // khai bao bien gia tri le 7 doan
uint8_t Code7Seg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,       0x3F|0x80, 0x06|0x80, 0x5B|0x80, 0x4F|0x80, 0x66|0x80, 0x6D|0x80, 0x7D|0x80, 0x07|0x80, 0x7F|0x80, 0x6F|0x80};

/////////////////khai bao cac chuong trinh///////////////
void main(void);

void delay_us(uint16_t x);
void delay_ms(uint16_t x);
void clk_config_16MHz_hsi(void);         //clk noi 16MHz
void GPIO_Config(void);                 // khai bao chan GPIO
void UART_Config(void);                  // khai bao UART 
void print(unsigned char *string);       //gui chuoi qua uart
void println(unsigned char *string);       // gui chuoi qua uart \r \n
void print_number(int number);
void TIM2_Config(void);                   //khoi tao timer2
void ngat_timer2(void);                   //chuong trinh ngat timer2

void TIM4_Config(void);                   //khoi tao timer4
void ngat_timer4(void);                   //chuong trinh ngat timer4
////Seven-segment display///
void LED_DD(uint8_t num);               // chon led D1~D4
void LED_Dec(uint16_t number);         // nhap du lieu so thap phan vao LED_DATA[]
void LED_Time(uint8_t Mode);         // nhap du lieu thoi gian thuc dong ho vao LED_DATA[]
void LED_7SEG(uint8_t b);               //xuat du lieu ra port led 7 doan



#endif

