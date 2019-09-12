
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "stm32f1xx_GPS.h"

#include <stdio.h>
#include <string.h>




/* External variables --------------------------------------------------------*/
//////////khai bao cac bien  ///////////////////////////////////////
extern uint8_t status,goi_den,sms_den,SOS;
extern uint16_t count_s;
uint16_t Second,Minute,Hour,Date,Day,Month,Year;         //bien dong ho thoi gian thuc
uint8_t Rx_data[3],ind, Transfer_cplt=0; //Khai bao cac bien de nhan du lieu
uint8_t ind_sms,SMS[100];
uint8_t Rx_plus_sta;
uint8_t Rx_server[20],ind_server;  //bien nhan du lieu tu server

uint8_t OK,RRO,server_78,CONNECT,Rx_sta,UGNSINF[100],LIP[20],CPBR[100],ind_CPBR,GSN[16]="865067022455847";//865067024139803//865067022455847
uint8_t Speed,GPS_Map[4][10],Num,DB[3],imei[2][16],GPS_sta;
uint16_t Course;
uint32_t map_N,map_E; // GPS sms



uint16_t lan_gui;
uint8_t data_server[45],temp_data[40];
uint8_t DongCo,Acquy;
uint8_t user[3][12],user_sms[3][15],sdt_ing[15],sms_sta;
uint8_t dem_0078;



uint8_t Rx_NEO[3],ind_NEO,GNRMC[100],Rx_NEO_sta;





/////////khai bao cac chuong trinh con ///////////////////////////////
void Clock_TIM1(void);                                  // chay dong ho  trong ngat TIM1  
void UART_Rx_Call(UART_HandleTypeDef *huart);           // ham nhan chuoi trong ngat uart 
void Read_GPS(void);                                    //doc gia tri GPS
void Khoi_Tao_Sim(void);                                // khoi tao module sim
void AT_CGNSURC(uint8_t t);                             // che do cap nhat GNS theo thoi gian t
void AT_CGNSINF(void);                                  // cap nhat GNS
void Read_imei(void);                                   // doc imei
void Connet_Server(void);                               // ket noi server
void TinhDataSever(uint8_t mode_tinh);                  // chuan bi du lieu se gui server (chon che do tinh)
void Send_Server(void);                                 // gui du lieu data_server len may chu
void read_Phonebook(void);                              // doc sdt trong danh ba
void save_Phonebook(void);                              // luu 2 sdt vao danh ba
void save_admin(void);                                  // luu 1 sdt admin
void read_sms(void);                                    // doc tin nhan sms, tach lay sdt
void send_sms(const char *sdt, const char *noi_dung);   // gui tin nhan sms
void cuoc_goi_den(void);                                //tach so dien thoai cuoc goi den
void ATD (const char *goi_sdt);                         // thuc hien cuoc goi
void sdt_84(void);                                      // them +84... de gui sms
//////main//////////////
void cuoc_goi(void);                                    // xu ly cuoc goi
void tin_nhan(void);                                    // xu ly tin nhan sms
void khan_cap(void);                                    // xu ly truong hop khan cap
void PWR_KEY(void);                                     // kich hoat module sim
void RELAY (uint8_t on);                                // dieu khien relay on off
void GPIO_EXTI(void);                                   // kiem tra ben ngoai


int JAS_IndexOf(const char *text, const char *key);      // tim chuoi trong chuoi tong


void UART3_Rx_Call(void);                              // nhan uart3
void Read_NEO(void);                                   // doc GPS NEO
//////////chay cac chuong trinh con o day:///////////////////////////////////////

///////////////////////uint16_t GetCrc16////////////////////////////////////////////////////
static const uint8_t Data[];
static const uint16_t crctab16[256] = { 
0X0000, 0X1189, 0X2312, 0X329B, 0X4624, 0X57AD, 0X6536, 0X74BF, 0X8C48,
0X9DC1, 0XAF5A, 0XBED3, 0XCA6C, 0XDBE5, 0XE97E, 0XF8F7, 0X1081, 0X0108,
0X3393, 0X221A, 0X56A5, 0X472C, 0X75B7, 0X643E, 0X9CC9, 0X8D40, 0XBFDB,
0XAE52, 0XDAED, 0XCB64, 0XF9FF, 0XE876, 0X2102, 0X308B, 0X0210, 0X1399, 
0X6726, 0X76AF, 0X4434, 0X55BD, 0XAD4A, 0XBCC3, 0X8E58, 0X9FD1, 0XEB6E,
0XFAE7, 0XC87C, 0XD9F5, 0X3183, 0X200A, 0X1291, 0X0318, 0X77A7, 0X662E,
0X54B5, 0X453C, 0XBDCB, 0XAC42, 0X9ED9, 0X8F50, 0XFBEF, 0XEA66, 0XD8FD,
0XC974, 0X4204, 0X538D, 0X6116, 0X709F, 0X0420, 0X15A9, 0X2732, 0X36BB, 
0XCE4C, 0XDFC5, 0XED5E, 0XFCD7, 0X8868, 0X99E1, 0XAB7A, 0XBAF3, 0X5285,
0X430C, 0X7197, 0X601E, 0X14A1, 0X0528, 0X37B3, 0X263A, 0XDECD, 0XCF44,
0XFDDF, 0XEC56, 0X98E9, 0X8960, 0XBBFB, 0XAA72, 0X6306, 0X728F, 0X4014,
0X519D, 0X2522, 0X34AB, 0X0630, 0X17B9, 0XEF4E, 0XFEC7, 0XCC5C, 0XDDD5,
0XA96A, 0XB8E3, 0X8A78, 0X9BF1, 0X7387, 0X620E, 0X5095, 0X411C, 0X35A3,
0X242A, 0X16B1, 0X0738, 0XFFCF, 0XEE46, 0XDCDD, 0XCD54, 0XB9EB, 0XA862,
0X9AF9, 0X8B70, 0X8408, 0X9581, 0XA71A, 0XB693, 0XC22C, 0XD3A5, 0XE13E,
0XF0B7, 0X0840, 0X19C9, 0X2B52, 0X3ADB, 0X4E64, 0X5FED, 0X6D76, 0X7CFF,
0X9489, 0X8500, 0XB79B, 0XA612, 0XD2AD, 0XC324, 0XF1BF, 0XE036, 0X18C1, 
0X0948, 0X3BD3, 0X2A5A, 0X5EE5, 0X4F6C, 0X7DF7, 0X6C7E, 0XA50A, 0XB483,
0X8618, 0X9791, 0XE32E, 0XF2A7, 0XC03C, 0XD1B5, 0X2942, 0X38CB, 0X0A50,
0X1BD9, 0X6F66, 0X7EEF, 0X4C74, 0X5DFD, 0XB58B, 0XA402, 0X9699, 0X8710,
0XF3AF, 0XE226, 0XD0BD, 0XC134, 0X39C3, 0X284A, 0X1AD1, 0X0B58, 0X7FE7,
0X6E6E, 0X5CF5, 0X4D7C, 0XC60C, 0XD785, 0XE51E, 0XF497, 0X8028, 0X91A1,
0XA33A, 0XB2B3, 0X4A44, 0X5BCD, 0X6956, 0X78DF, 0X0C60, 0X1DE9, 0X2F72,
0X3EFB, 0XD68D, 0XC704, 0XF59F, 0XE416, 0X90A9, 0X8120, 0XB3BB, 0XA232,
0X5AC5, 0X4B4C, 0X79D7, 0X685E, 0X1CE1, 0X0D68, 0X3FF3, 0X2E7A, 0XE70E,
0XF687, 0XC41C, 0XD595, 0XA12A, 0XB0A3, 0X8238, 0X93B1, 0X6B46, 0X7ACF,
0X4854, 0X59DD, 0X2D62, 0X3CEB, 0X0E70, 0X1FF9, 0XF78F, 0XE606, 0XD49D,
0XC514, 0XB1AB, 0XA022, 0X92B9, 0X8330, 0X7BC7, 0X6A4E, 0X58D5, 0X495C,
0X3DE3, 0X2C6A, 0X1EF1, 0X0F78, };
uint16_t GetCrc16(const uint8_t*pData, int nLength)
{
uint16_t fcs = 0xffff;
while(nLength>0)
{

  fcs = (fcs>>8)^crctab16[(fcs^*pData)&0xff]; 
  nLength--; 
  pData++; 
}
return(~fcs); 
}

///////////////dong ho thoi gian thuc TIM1//////////////////////
void Clock_TIM1(void)           
{
  count_s++;
  Second++;
  if(count_s>0xfff0)
  {
    count_s=0;
  }
  if(Second>=59)
  {
    Second=0;
    Minute++;
  }
  if(Minute>=59)
  {
    Minute=0;
    Hour++;
  }
  if(Hour>23)
  {
    Hour=Hour%24;
  }
  HAL_GPIO_TogglePin(LED_PORT,LED3_PIN);
  HAL_UART_Receive_IT(&huart1,Rx_data, 1);	//Kich hoat UART1 nhan du lieu ngat moi luc
  HAL_UART_Receive_IT(&huart3, Rx_NEO, 1);

}
////////////////////////ngat uart nhan du lieu////////////////////////////////////////////
void UART_Rx_Call(UART_HandleTypeDef *huart)
{
  if(Rx_data[0]==0x0d){ Rx_sta=0; ind=0;}
  switch (Rx_sta)
  {
      case 0:   //kiem tra cac truong hop
      {
        /////////Rx_plus_sta nhan nhung cau dai vo han////////////////////////////
        if(Rx_data[2]==0x0a&Rx_data[1]==0x78&Rx_data[0]==0x78) {server_78=1; Rx_plus_sta=1; ind_server=0;}   // server tra loi (1)
        if(Rx_data[2]=='C'&Rx_data[1]=='M'&Rx_data[0]=='T'){Rx_plus_sta=2; ind_sms=0; sms_den=1; } // nhan tin nhan sms (2) 

        /////////// nhan biet trang thai ///////////////////
        if(Rx_data[2]=='O'&Rx_data[1]=='K'&Rx_data[0]==0x0d) {OK=1;}    // OK
        if(Rx_data[2]==0x0a&Rx_data[1]=='E'&Rx_data[0]=='R') {RRO=1;}   // ERROR
        if(Rx_data[2]=='N'&Rx_data[1]=='E'&Rx_data[0]=='C') {CONNECT=1;}   // CONNECT OK 
        
        //////////// Rx_sta nhan nhung cau ngan ...0x0d///////////////////
        if(Rx_data[0]=='8'&Rx_data[1]==0x0a){Rx_sta=1; GSN[ind]=Rx_data[0]; ind++;} //imei      (1)
        if(Rx_data[2]=='G'&Rx_data[1]=='N'&Rx_data[0]=='S'){Rx_sta=2;} //+CGNSINF:|+UGNSINF:    (2) 
        if(Rx_data[2]=='L'&Rx_data[1]=='I'&Rx_data[0]=='P'){Rx_sta=3;} // cuoc goi den          (3)
        if(Rx_data[2]=='P'&Rx_data[1]=='B'&Rx_data[0]=='R'){Rx_sta=4;} // doc danh ba sim       (4)
        
          break;
      }
      case 1:   //nhap imei
      {
          if(ind<15) //GSN[16]
          {
            GSN[ind]=Rx_data[0];
            ind++;
          }          
          break;
      }
      case 2:   //nhap GPS
      {
          if(ind<99) //UGNSINF[100]
          {
            UGNSINF[ind]=Rx_data[0];
            ind++;
          }   
          break;
      }
      case 3:   //sdt goi den
      {
          if(ind<19) //LIP[20]
          {
            LIP[ind]=Rx_data[0];
            ind++;
          }
          else { goi_den=1; }
            
          break;
      }
      case 4:   //doc danh ba sim
      {
          if(ind_CPBR<99) //CPBR[100]
          {
            CPBR[ind_CPBR]=Rx_data[0];
            ind_CPBR++;
          }   
          break;
      }

  }
  switch (Rx_plus_sta)
  {
    case 1: // nhan du lieu tu server 7878...0d0a
    {
      if(ind_server<19) //Rx_server[20]
      {
        Rx_server[ind_server]=Rx_data[0];
        ind_server++;
      }    
      break;
    }
    case 2: //nhan sms
    {
      if(ind_sms<99) //SMS[100]
      {
        SMS[ind_sms]=Rx_data[0];
        ind_sms++;
      }   
    
      break;
    }
  }
  

  Rx_data[2]=Rx_data[1];
  Rx_data[1]=Rx_data[0];
  
  
}

//////////////uart3 NEO////////////////////
void UART3_Rx_Call(void)
{
  if(Rx_NEO[0]==0x0d){ Rx_NEO_sta=0; ind_NEO=0;}
  switch (Rx_NEO_sta)
  {
      case 0:   //kiem tra cac truong hop
      {
        if(Rx_NEO[2]=='R'&Rx_NEO[1]=='M'&Rx_NEO[0]=='C')  Rx_NEO_sta=1;
          break;
      }
      case 1:   
      {
        if(ind_NEO<99)
          {
            GNRMC[ind_NEO]=Rx_NEO[0];
            ind_NEO++;
          }       
                   
          break;
      }
      case 2:   
      {
            
          break;
      }

  }

  Rx_NEO[2]=Rx_NEO[1];
  Rx_NEO[1]=Rx_NEO[0];
}
///////////doc toa do GPS NEO////////////
void Read_NEO(void)
{
  uint8_t dem=0,vitri[20],course_temp[10];
  uint8_t Speed_temp[2][10];
  uint32_t temp_GPS[2];
  
  for(uint8_t ps=0;ps<100;ps++)
      {
        if(GNRMC[ps]==',')
        {
          dem++;  //dem dau ,
          vitri[dem]=ps+1;
        }
      }
  //doc gia tri van toc//
    for(uint8_t j=0;j<(vitri[8]-vitri[7]-1);j++)
      {
        Speed_temp[0][j]=GNRMC[vitri[7]+j];     // nhap gia tri van toc Speed[0]
        if(Speed_temp[0][j]==0x2e) vitri[19]=j;         // tim dau cham .
        Speed_temp[1][j]= Speed_temp[0][j]-48;                // dich sang kieu Decima dang so 
      }
    switch(vitri[19])              // tinh gia tri theo vi tri dau . -> Speed[2][0]
        {
        case 1: 
          Speed=Speed_temp[1][0];
          break;
        case 2: 
          Speed=Speed_temp[1][0]*10+Speed_temp[1][1];
          break;
        case 3: 
          Speed=Speed_temp[1][0]*100+Speed_temp[1][1]*10+Speed_temp[1][2];
          break;
        }
    if(Speed<2) Speed=0;
    Speed=2*Speed;
  
  // nhap gia tri thoi gian 
        Second=0+(GNRMC[vitri[1]+4]-48)*10+(GNRMC[vitri[1]+5]-48);
        Minute=(GNRMC[vitri[1]+2]-48)*10+(GNRMC[vitri[1]+3]-48);
        Hour  =7+(GNRMC[vitri[1]]-48)*10+(GNRMC[vitri[1]+1]-48); if(Hour>24) Hour=Hour%24;
        Day   =(GNRMC[vitri[9]]-48)*10+(GNRMC[vitri[9]+1]-48); 
        Month =(GNRMC[vitri[9]+2]-48)*10+(GNRMC[vitri[9]+3]-48);
        Year  =2000+(GNRMC[vitri[9]+4]-48)*10+(GNRMC[vitri[9]+5]-48);
        
        //GPS_Map
      if((vitri[4]-vitri[3])>1) // khi co toa do GPS
      {
        for(uint8_t j=0;j<((vitri[4]-vitri[3]-1));j++) GPS_Map[0][j]=GNRMC[vitri[3]+j];  //nhap gia tri vao GPS_Map[0]   N
        for(uint8_t j=0;j<(vitri[6]-vitri[5]-1);j++) GPS_Map[1][j]=GNRMC[vitri[5]+j];    //nhap gia tri vao GPS_Map[1]   E
      
        for(uint8_t j=0;j<10;j++) // dem theo so ki tu GPS_Map[0]
        {      
          GPS_Map[0][j]=GPS_Map[0][j]-48; // GPS_Map[0] dich sang kieu Decima dang so
          GPS_Map[1][j]=GPS_Map[1][j]-48; // GPS_Map[1] dich sang kieu Decima dang so
        }
        GPS_sta=1;      // trang thai co toa do
      }
      else
      {
        GPS_sta=0;      // trang thai ko toa do
      }
      if(Minute>59|Day>31|Month>12)
      {
        GPS_sta=0;
      }
      //tinh theo cong thuc GPS_Map[0] 
      temp_GPS[0]=(GPS_Map[0][0]*1000000+GPS_Map[0][1]*100000)*18+(GPS_Map[0][2]*10000+GPS_Map[0][3]*1000+GPS_Map[0][5]*100+GPS_Map[0][6]*10+GPS_Map[0][7])*30;      //22o32.7658=(22X60+32.7658)X30000=40582974          
      
      //tinh theo cong thuc GPS_Map[1] 
      temp_GPS[1]=(GPS_Map[1][0]*10000000+GPS_Map[1][1]*1000000+GPS_Map[1][2]*100000)*18+(GPS_Map[1][3]*10000+GPS_Map[1][4]*1000+GPS_Map[1][6]*100+GPS_Map[1][7]*10+GPS_Map[1][8])*30;      //22o32.7658=(22X60+32.7658)X30000=40582974
      
      //chuyen sang hex: GPS_Map[0] -> temp_GPS[0]-> GPS_Map[2]//
      GPS_Map[2][0]=temp_GPS[0]/(0xffffff+1);       
      GPS_Map[2][1]=(temp_GPS[0]%(0xffffff+1))/(0xffff+1);
      GPS_Map[2][2]=(temp_GPS[0]%(0xffff+1))/(0xff+1);
      GPS_Map[2][3]=temp_GPS[0]%(0xff+1);    
      
      //chuyen sang hex: GPS_Map[1] -> temp_GPS[1]-> GPS_Map[3]//
      GPS_Map[3][0]=temp_GPS[1]/(0xffffff+1);       
      GPS_Map[3][1]=(temp_GPS[1]%(0xffffff+1))/(0xffff+1);
      GPS_Map[3][2]=(temp_GPS[1]%(0xffff+1))/(0xff+1);
      GPS_Map[3][3]=temp_GPS[1]%(0xff+1);
      
      //doc gia tri course//
      vitri[19]=0;
      for(uint8_t j=0;j<10;j++)
      {
        course_temp[j]=GNRMC[vitri[8]+j];     // nhap gia tri course[]
        if(course_temp[j]==0x2e) vitri[19]=j;         // tim dau cham .
        course_temp[j]= course_temp[j]-48;         // dich sang kieu Decima dang so 
      }
      switch(vitri[19])              // tinh gia tri theo vi tri dau . -> Course
        {
        case 1: 
          Course=course_temp[0];
          break;
        case 2: 
          Course=course_temp[0]*10+course_temp[1];
          break;
        case 3: 
          Course=course_temp[0]*100+course_temp[1]*10+course_temp[2];
          break;
        }
      Num=0x0b;
      map_N=temp_GPS[0];
      map_E=temp_GPS[1];

}
///////////////////////////// doc toa do gps ////////////////////////////////////
void Read_GPS(void)
{  
  
  uint8_t dem=0,vitri[20];
  uint8_t Speed_temp[2][10],course_temp[10];
  uint32_t temp_GPS[2];
  
  
  for(uint8_t ps=0;ps<100;ps++)
      {
        if(UGNSINF[ps]==',')
        {
          dem++;  //dem dau ,
          if(dem==0) vitri[0]=ps+1;
          if(dem==1) vitri[1]=ps+1; 
          if(dem==2) vitri[2]=ps+1;
          if(dem==3) vitri[3]=ps+1;
          if(dem==4) vitri[4]=ps+1;
          if(dem==5) vitri[5]=ps+1;
          if(dem==6) vitri[6]=ps+1;
          if(dem==7) vitri[7]=ps+1;
          if(dem==8) vitri[8]=ps+1;
          if(dem==9) vitri[9]=ps+1;
          if(dem==10) vitri[10]=ps+1;
          if(dem==11) vitri[11]=ps+1;
          if(dem==12) vitri[12]=ps+1;
          if(dem==13) vitri[13]=ps+1;
          if(dem==14) vitri[14]=ps+1;
          if(dem==15) vitri[15]=ps+1;
          if(dem==16) vitri[16]=ps+1;
          if(dem==17) vitri[17]=ps+1; 
          if(dem==18) vitri[18]=ps+1;
        }
      }
  if(dem>14)
  {
    //doc gia tri van toc//
    for(uint8_t j=0;j<(vitri[7]-vitri[6]-1);j++)
      {
        Speed_temp[0][j]=UGNSINF[vitri[6]+j];     // nhap gia tri van toc Speed[0]
        if(Speed_temp[0][j]==0x2e) vitri[19]=j;         // tim dau cham .
        Speed_temp[1][j]= Speed_temp[0][j]-48;                // dich sang kieu Decima dang so 
      }
    switch(vitri[19])              // tinh gia tri theo vi tri dau . -> Speed[2][0]
        {
        case 1: 
          Speed=Speed_temp[1][0];
          break;
        case 2: 
          Speed=Speed_temp[1][0]*10+Speed_temp[1][1];
          break;
        case 3: 
          Speed=Speed_temp[1][0]*100+Speed_temp[1][1]*10+Speed_temp[1][2];
          break;
        }
    if(Speed<4) Speed=0;
    // nhap gia tri thoi gian 
        Second=0+(UGNSINF[vitri[2]+12]-48)*10+(UGNSINF[vitri[2]+13]-48);
        Minute=(UGNSINF[vitri[2]+10]-48)*10+(UGNSINF[vitri[2]+11]-48);
        Hour  =7+(UGNSINF[vitri[2]+8]-48)*10+(UGNSINF[vitri[2]+9]-48);
        Day   =(UGNSINF[vitri[2]+6]-48)*10+(UGNSINF[vitri[2]+7]-48); 
        Month =(UGNSINF[vitri[2]+4]-48)*10+(UGNSINF[vitri[2]+5]-48);
        Year  =(UGNSINF[vitri[2]]-48)*1000+(UGNSINF[vitri[2]+1]-48)*100+(UGNSINF[vitri[2]+2]-48)*10+(UGNSINF[vitri[2]+3]-48);
    //GPS_Map
      if((vitri[4]-vitri[3])>1) // khi co toa do GPS
      {
        for(uint8_t j=0;j<((vitri[4]-vitri[3]-1));j++) GPS_Map[0][j]=UGNSINF[vitri[3]+j];  //nhap gia tri vao GPS_Map[0]   N
        for(uint8_t j=0;j<(vitri[5]-vitri[4]-1);j++) GPS_Map[1][j]=UGNSINF[vitri[4]+j];    //nhap gia tri vao GPS_Map[1]   E
      
        for(uint8_t j=0;j<10;j++) // dem theo so ki tu GPS_Map[0]
        {      
          GPS_Map[0][j]=GPS_Map[0][j]-48; // GPS_Map[0] dich sang kieu Decima dang so
          GPS_Map[1][j]=GPS_Map[1][j]-48; // GPS_Map[1] dich sang kieu Decima dang so
        }
        GPS_sta=1;      // trang thai co toa do
      }
      else
      {
        GPS_sta=0;      // trang thai ko toa do
      }
      //tinh theo cong thuc GPS_Map[0] 
      temp_GPS[0]=(GPS_Map[0][0]*1000000+GPS_Map[0][1]*100000+GPS_Map[0][3]*10000+GPS_Map[0][4]*1000+GPS_Map[0][5]*100+GPS_Map[0][6]*10+GPS_Map[0][7])*18;      //22o32.7658=(22X60+32.7658)X30000=40582974          
      
      //tinh theo cong thuc GPS_Map[1] 
      temp_GPS[1]=(GPS_Map[1][0]*10000000+GPS_Map[1][1]*1000000+GPS_Map[1][2]*100000+GPS_Map[1][4]*10000+GPS_Map[1][5]*1000+GPS_Map[1][6]*100+GPS_Map[1][7]*10+GPS_Map[1][8])*18;      //22o32.7658=(22X60+32.7658)X30000=40582974
      
      //chuyen sang hex: GPS_Map[0] -> temp_GPS[0]-> GPS_Map[2]//
      GPS_Map[2][0]=temp_GPS[0]/(0xffffff+1);       
      GPS_Map[2][1]=(temp_GPS[0]%(0xffffff+1))/(0xffff+1);
      GPS_Map[2][2]=(temp_GPS[0]%(0xffff+1))/(0xff+1);
      GPS_Map[2][3]=temp_GPS[0]%(0xff+1);    
      
      //chuyen sang hex: GPS_Map[1] -> temp_GPS[1]-> GPS_Map[3]//
      GPS_Map[3][0]=temp_GPS[1]/(0xffffff+1);       
      GPS_Map[3][1]=(temp_GPS[1]%(0xffffff+1))/(0xffff+1);
      GPS_Map[3][2]=(temp_GPS[1]%(0xffff+1))/(0xff+1);
      GPS_Map[3][3]=temp_GPS[1]%(0xff+1);
      
      //doc gia tri course//
      vitri[19]=0;
      for(uint8_t j=0;j<10;j++)
      {
        course_temp[j]=UGNSINF[vitri[7]+j];     // nhap gia tri course[]
        if(course_temp[j]==0x2e) vitri[19]=j;         // tim dau cham .
        course_temp[j]= course_temp[j]-48;         // dich sang kieu Decima dang so 
      }
      switch(vitri[19])              // tinh gia tri theo vi tri dau . -> Course
        {
        case 1: 
          Course=course_temp[0];
          break;
        case 2: 
          Course=course_temp[0]*10+course_temp[1];
          break;
        case 3: 
          Course=course_temp[0]*100+course_temp[1]*10+course_temp[2];
          break;
        }
      
      //doc gia tri Num, tai vitri[5]//

      if((vitri[15]-vitri[14])==3)    // neu gia tri 2 chu so
      {
        Num=(UGNSINF[vitri[14]]-48)*10 + UGNSINF[vitri[14]+1]-48;         // chuyen char sang so va tinh gia tri -> dang 0F 
      }
      if((vitri[15]-vitri[14])==2)    // neu gia tri 1 chu so
      {
        Num=(UGNSINF[vitri[14]]-48);                                          // chuyen char sang so va tinh gia tri -> dang 0F 
      }
      
      //doc gia tri song DB//
      DB[0]=UGNSINF[vitri[18]];
      DB[1]=UGNSINF[vitri[18]+1];
      DB[2]=(DB[0]-0x30)*0x10+(DB[1]-0x30);
  }  
}
////////////khoi tao sim/////////////
void Khoi_Tao_Sim(void)
{

    printf("AT\r\n"); ////
    HAL_Delay(100);
    printf("ATE0\r\n");         // ATE0 ko hien thi gia tri gui xuong
    HAL_Delay(100);
    printf("AT+CLIP=1\r\n");
    HAL_Delay(100);    
    printf("AT+CMGF=1\r\n");    //bat che do truyen nhan sms
    HAL_Delay(100);
    //printf("AT+CGPSPWR=1\r\n"); // bat GPS
    //HAL_Delay(100);
    printf("AT+CGNSPWR=0\r\n"); // bat GNS
    HAL_Delay(100);
    printf("AT+CNMI=2,2\r\n");  // hien thi ngay noi dung khi co tin nhan den
    HAL_Delay(100);
    printf("AT&W\r\n");         //luu lai cac thiet lap
    HAL_Delay(500);
}
///////// tu cap nhat GNS theo chu ky /////////////
void AT_CGNSURC(uint8_t t)
{
  printf("AT + CGNSURC =%d \r\n", t );
  HAL_Delay(300); 
}
void Read_imei(void)
{
  //printf("AT+GSN\r\n");
  //HAL_Delay(1000);
  for(int j=1;j<16;j++) // nhap imei
  {
    imei[0][j]=GSN[j-1];  
  }
  imei[0][0]='0';          // them so 0 vao vi tri dau tien
  for(int j=0;j<8;j++)  // chuyen sang dang hex luu vao imei[1]
  {
    imei[1][j]=((imei[0][j*2])-48)*16+imei[0][j*2+1]-48;  
  }

}
////////cap nhat GNS tu module sim//////////
void AT_CGNSINF(void)
{
  printf("AT+CGNSINF\r\n");
  HAL_Delay(1000);
}
//////////ket noi serve////////////
void Connet_Server(void)
{
  //char IP[26]={34,'T','C','P',34,44,34,'1','1','2',46,'7','8',46,'1',46,'8','5',34,44,34,'5','0','2','3',34}; // chuoi dia chi ket noi server
  
  OK=0; 
  printf("AT+CIPSHUT\r\n");
  for(uint8_t j=0;j<30;j++)     // cho SHUT OK, toi da 3000ms
  {
    HAL_Delay(100);     
    if(OK==1)
    {
      j=30;     // thoat khi SHUT OK    
    }
  }
  CONNECT=0;
  //AT+CIPSTART="TCP","112.78.3.172","5023"//
  printf("AT+CIPSTART=\"TCP\"\,\"112.78.3.172\"\,\"5023\"\r\n");

  for(uint8_t j=0;j<70;j++)     // cho CONNECT OK, toi da 7000ms
  {
    HAL_Delay(100);     
    if(CONNECT==1)
    {
      j=70;     // CONNECT OK    
    }
  }
}
///////////////bien dich du lieu chan bi gui server////////////////////
void TinhDataSever(uint8_t mode_tinh)
{
  //uint8_t send_0[4]={0x78,0x78,0x0d,01},  send_end[3]={0x0d,0x0a,0x1a}; //0x1a la ctrl+z ket thuc 
  uint8_t send_start[2]={0x78,0x78}, send_stop[2]={0x0d,0x0a},dem_lan_gui;
  uint16_t temp;
  switch (mode_tinh) 
  {
    case 1:             //mode tinh imei
        lan_gui++;
        temp_data[0]=0x0d;        // so luong byte gui ( Packet Length ) 
        temp_data[1]=0x01;        //kieu du lieu ( Protocol Number )
        for(uint8_t j=2;j<10;j++)  // nhap imei[1] dang hex
        {
          temp_data[j]=imei[1][j-2];
        }
        temp_data[10]=lan_gui/256;            // nhap byte thu 1 bien lan_gui 
        temp_data[11]=lan_gui%256;           // nhap byte thu 2 bien lan_gui
        temp=GetCrc16(temp_data,12);          // tinh CRC
        temp_data[12]=temp/256;               // nhap byte CRC thu 1
        temp_data[13]=temp%256;               // nhap byte CRC thu 2
        
       for(int j=0;j<2;j++) data_server[j]=send_start[j];         // nhap tu vi tri 0 toi 1: 7878
       for(int j=2;j<16;j++) data_server[j]=temp_data[j-2];       // nhap tu vi tri 2  toi 15: temp_data
       for(int j=16;j<18;j++) data_server[j]=send_stop[j-16];     // nhap tu vi tri 16  toi 17: 0d0a
        
        
      break;
    case 2 :            // tinh toa do, thoi gian, van toc...
     lan_gui++;
     temp_data[0]=0x1f;        // so luong byte gui ( Packet Length ) 
     temp_data[1]=0x12;        //kieu du lieu ( Protocol Number )
      // data time //
     temp_data[2]= Year%100;
     temp_data[3]= Month;
     temp_data[4]= Day;
     temp_data[5]= Hour;
     temp_data[6]= Minute;
     temp_data[7]= Second;     
     temp_data[8]=0xC0+Num;     // Length of GPS information, quantity of positioning satellites
     // tinh toa do //
     for(uint8_t j=9;j<13;j++){ temp_data[j]=GPS_Map[2][j-9]; }      // nhap kinh do
     for(uint8_t j=13;j<17;j++){ temp_data[j]=GPS_Map[3][j-13]; }      // nhap vi do
     // tinh van toc//
     temp_data[17]=Speed;
     temp_data[18]=0x14+(Course/(0xff+1)); temp_data[19]=Course%(0xff+1);  //Status,Course       
     temp_data[20]=0x01; temp_data[21]=0xc4;                    // ma quoc gia MCC 01c4
     temp_data[22]=0x04;                                        // ma nha mang MNC 04
     temp_data[23]=0x01; temp_data[24]=0xc4;                    // ma LAC
     temp_data[25]=0x01; temp_data[26]=0xc4; temp_data[27]=0xc4; // Cell ID
     temp_data[28]=lan_gui/256;            // nhap byte thu 1 bien lan_gui 
     temp_data[29]=lan_gui%256;           // nhap byte thu 2 bien lan_gui
     temp=GetCrc16(temp_data,30);          // tinh CRC
     temp_data[30]=temp/256;               // nhap byte CRC thu 1
     temp_data[31]=temp%256;               // nhap byte CRC thu 2
     
     for(uint8_t j=0;j<2;j++) data_server[j]=send_start[j];         // nhap tu vi tri 0 toi 1: 7878
     for(uint8_t j=2;j<34;j++) data_server[j]=temp_data[j-2];       // nhap tu vi tri 2  toi 33: temp_data
     for(uint8_t j=34;j<36;j++) data_server[j]=send_stop[j-34];     // nhap tu vi tri 34  toi 35: 0d0a      
       
      break;
      
    case 3:               //Alarm packet trang thai
    lan_gui++;
    
    temp_data[0]=0x25;        // so luong byte gui ( Packet Length ) 
    temp_data[1]=0x16;        //kieu du lieu ( Protocol Number )
      // data time //
     temp_data[2]= Year%100;
     temp_data[3]= Month;
     temp_data[4]= Day;
     temp_data[5]= Hour;
     temp_data[6]= Minute;
     temp_data[7]= Second;
     temp_data[8]=0xC0+Num;     // Length of GPS information, quantity of positioning satellites
     // tinh toa do //
     for(uint8_t j=9;j<13;j++){ temp_data[j]=GPS_Map[2][j-9]; }      // nhap kinh do
     for(uint8_t j=13;j<17;j++){ temp_data[j]=GPS_Map[3][j-13]; }      // nhap vi do
     // tinh van toc//
     temp_data[17]=Speed;
     temp_data[18]=0x14+(Course/(0xff+1)); temp_data[19]=Course%(0xff+1);  //Status,Course     
     //LBS Information//
     temp_data[20]=0x09;        //LBS Length
     temp_data[21]=0x01; temp_data[22]=0xc4;                    // ma quoc gia MCC 01c4
     temp_data[23]=0x04;                                        // ma nha mang MNC 04
     temp_data[24]=0x01; temp_data[25]=0xc4;                    // ma LAC
     temp_data[26]=0x01; temp_data[27]=0xc4; temp_data[28]=0xc4; // Cell ID
     //Terminal Information//
     temp_data[29]=0xc7;  //Terminal Information Content
     temp_data[30]=0x04;       // Voltage Level 0~6
     temp_data[31]=0x03;       //GSM Signal Strength Levels 0~4
     temp_data[32]=0x00;        //Alarm 0~5
     temp_data[33]=0x02;        //Language is English
     
     temp_data[34]=lan_gui/256;            // nhap byte thu 1 bien lan_gui 
     temp_data[35]=lan_gui%256;           // nhap byte thu 2 bien lan_gui
     temp=GetCrc16(temp_data,36);          // tinh CRC
     temp_data[36]=temp/256;               // nhap byte CRC thu 1
     temp_data[37]=temp%256;               // nhap byte CRC thu 2
     
     for(uint8_t j=0;j<2;j++) data_server[j]=send_start[j];         // nhap tu vi tri 0 toi 1: 7878
     for(uint8_t j=2;j<40;j++) data_server[j]=temp_data[j-2];       // nhap tu vi tri 2  toi 39: temp_data
     for(uint8_t j=40;j<42;j++) data_server[j]=send_stop[j-40];     // nhap tu vi tri 40  toi 41: 0d0a
          
    break;
  case 4:             // Terminal Sending Heartbeat Packet to Server
    
    lan_gui++;

     temp_data[0]=0x0a;        // so luong byte gui ( Packet Length ) 
     temp_data[1]=0x01;        //kieu du lieu ( Protocol Number )
      //Terminal Information//
     temp_data[2]=0xc7;  //Terminal Information Content
     temp_data[3]=0x04;       // Voltage Level 0~6
     temp_data[4]=0x03;       //GSM Signal Strength Levels 0~4
     temp_data[5]=0x00;        //Alarm 0~5
     temp_data[6]=0x02;        //Language is English
     
     temp_data[7]=lan_gui/256;            // nhap byte thu 1 bien lan_gui 
     temp_data[8]=lan_gui%256;           // nhap byte thu 2 bien lan_gui
     temp=GetCrc16(temp_data,9);          // tinh CRC
     temp_data[9]=temp/256;               // nhap byte CRC thu 1
     temp_data[10]=temp%256;               // nhap byte CRC thu 2
     
     for(uint8_t j=0;j<2;j++) data_server[j]=send_start[j];         // nhap tu vi tri 0 toi 1: 7878
     for(uint8_t j=2;j<13;j++) data_server[j]=temp_data[j-2];       // nhap tu vi tri 2  toi 12: temp_data
     for(uint8_t j=13;j<15;j++) data_server[j]=send_stop[j-13];     // nhap tu vi tri 13  toi 14: 0d0a 
    break;
    
  case 5:       //tinh trang khi toa do GPS 00, phan ++ serve ko yeu cau
    lan_gui++;

     temp_data[0]=0x0c;        // so luong byte gui ( Packet Length ) 
     temp_data[1]=0x05;        //kieu du lieu ( Protocol Number )
      // data time //
     temp_data[2]= Year%100;
     temp_data[3]= Month;
     temp_data[4]= Day;
     temp_data[5]= Hour;
     temp_data[6]= Minute;
     temp_data[7]= Second;
     temp_data[8]=0xC0+Num;     // Length of GPS information, quantity of positioning satellites
     temp_data[9]=lan_gui/256;            // nhap byte thu 1 bien lan_gui 
     temp_data[10]=lan_gui%256;           // nhap byte thu 2 bien lan_gui
     temp=GetCrc16(temp_data,11);          // tinh CRC
     temp_data[11]=temp/256;               // nhap byte CRC thu 1
     temp_data[12]=temp%256;               // nhap byte CRC thu 2
     
     for(uint8_t j=0;j<2;j++) data_server[j]=send_start[j];         // nhap tu vi tri 0 toi 1: 7878
     for(uint8_t j=2;j<14;j++) data_server[j]=temp_data[j-2];       // nhap tu vi tri 2  toi 13: temp_data
     for(uint8_t j=14;j<16;j++) data_server[j]=send_stop[j-14];     // nhap tu vi tri 14  toi 15: 0d0a
    break;
  case 6:       //gui tinh trang...   serve ko yeu cau
    lan_gui++;
    temp_data[0]=0x17;        // so luong byte gui ( Packet Length ) 
    temp_data[1]=0x05;        //kieu du lieu ( Protocol Number )
    
    for(uint8_t j=2;j<10;j++)  // nhap imei[1] dang hex
        {
          temp_data[j]=imei[1][j-2];
        }
    // data time //
     temp_data[10]= Year%100;
     temp_data[11]= Month;
     temp_data[12]= Day;
     temp_data[13]= Hour;
     temp_data[14]= Minute;
     temp_data[15]= Second;
     temp_data[16]=0xC0+Num;     // Length of GPS information, quantity of positioning satellites
     temp_data[17]= 0xff;
     temp_data[18]= DB[2]; //cuong do song GPS
     temp_data[19]= 0xDD;
     temp_data[20]=lan_gui/256;            // nhap byte thu 1 bien lan_gui 
     temp_data[21]=lan_gui%256;           // nhap byte thu 2 bien lan_gui
     temp=GetCrc16(temp_data,22);          // tinh CRC
     temp_data[22]=temp/256;               // nhap byte CRC thu 1
     temp_data[23]=temp%256;               // nhap byte CRC thu 2
     
     for(uint8_t j=0;j<2;j++) data_server[j]=send_start[j];         // nhap tu vi tri 0 toi 1: 7878
     for(uint8_t j=2;j<26;j++) data_server[j]=temp_data[j-2];       // nhap tu vi tri 2  toi 25: temp_data
     for(uint8_t j=26;j<28;j++) data_server[j]=send_stop[j-26];     // nhap tu vi tri 26  toi 27: 0d0a
     
     break;
  }
}
////////////////////////// TRUYEN DU LIEU SERVER/////////////////////////
void Send_Server(void)
{
  uint8_t length=data_server[2]+5;
  printf("AT+CIPSEND=%d\r\n",length);
  HAL_Delay(500);
  HAL_UART_Transmit(&huart1,data_server,length, 1000);
  HAL_Delay(1500); 
  
}
//////////gui tin nhan sms////////////
void send_sms(const char *sdt, const char *noi_dung)
{
  uint8_t ket_thuc[3]={0x1a,0x0d,0x0a};
  if(JAS_IndexOf(sdt,"+84")==0)
  {
    printf("AT+CMGS=\"%s\"\r\n",sdt);
    HAL_Delay(1000);
    printf("%s",noi_dung);
    HAL_UART_Transmit(&huart1,ket_thuc,1, 1000);
    HAL_Delay(1000);
  }
  
}
/////////luu so dien thoai vao danh ba/////////
void save_Phonebook()
{
  for(int k=0;k<2;k++)
  {
    printf("AT+CPBW=%d\,\"%s\" \,129\, \r\n" ,(k+1),user[k]);   // AT+CPBW=vitri,
    HAL_Delay(500);         
  }

}
/////luu sdt admin//// 
void save_admin()
{
  printf("AT+CPBW=%d\,\"%s\" \,129\, \r\n" ,3,user[2]);   // AT+CPBW=vitri,
  HAL_Delay(500);

}

///////////doc sdt trong danh ba sim//////////////
void read_Phonebook(void)
{
  uint8_t vitri_start,vitri_end;
  uint8_t l; //do dai chuoi
  ind_CPBR=0;
  for(uint8_t n=0;n<100; n++) // xoa du lieu truoc khi ghi
  {
    CPBR[n]=0;
  }
  ind_CPBR=0;
  
  printf("AT+CPBR=1,3\r\n");
  HAL_Delay(3000);
  for(uint8_t n=0;n<12; n++) // xoa du lieu truoc khi ghi
      {
        user[0][n]=0;
        user[1][n]=0;
        user[2][n]=0;
      }
  for(uint8_t m=0;m<3;m++) // lay 3 sdt trong danh ba
  {
    vitri_start=JAS_IndexOf(CPBR,"\,\"")+2;     // ,"
    vitri_end=JAS_IndexOf(CPBR,"\"\,129");     // ",129
    l=vitri_end-vitri_start;
    for(uint8_t n=0;n<l ; n++)
    {
      user[m][n]=CPBR[n+vitri_start];    
    }
    vitri_end=JAS_IndexOf(CPBR,"\"\:")+2; //tim vi tri het cau xuong dong
    for(uint8_t n=0;n<vitri_end ; n++)    // xoa cau da lay du lieu
    {
      if(CPBR[n]<'0'|CPBR[n]>'9') CPBR[n]=' '; //chi de lai so 0-9  
    }
  }  
  
  sdt_84(); // lay sdt gui sms  
}
///////doc sms, tach noi dung lay sdt///////////
void read_sms(void)
{
  
  uint8_t vitri_start,vitri_end;
  uint8_t l; //do dai chuoi
  ////// lay sdt vua gui//////////
  for(uint8_t n=0;n<15; n++) // xoa du lieu truoc khi ghi
      {
        sdt_ing[n]=0;
      }
    vitri_start=JAS_IndexOf(SMS,"\"+")+1; // "+
    vitri_end=JAS_IndexOf(SMS,"\"\,");    // ",
    l=vitri_end-vitri_start;
    for(uint8_t n=0;n<l ; n++) // lay sdt dang nhan tin sms
      {
        sdt_ing[n]=SMS[n+vitri_start];    
      }
  /////////////////////////// 
  if(JAS_IndexOf(SMS,"SDT")>=0) // tim SDT  > -1
  {
    sms_sta=1; //chon luu danh ba bang tin nhan
    
    for(uint8_t n=0;n<12; n++) // xoa du lieu truoc khi ghi
      {
        user[0][n]=0;
        user[1][n]=0;
        
      }    
    for(uint8_t m=0;m<2;m++) // lay 2 sdt trong SMS
    {
      vitri_start=JAS_IndexOf(SMS,"\*")+1; // *
      vitri_end=JAS_IndexOf(SMS,"\#");    // #
      l=vitri_end-vitri_start;
      for(uint8_t n=0;n<l ; n++)
      {
        user[m][n]=SMS[n+vitri_start];    
      }
      vitri_end=vitri_end+1;
      for(uint8_t n=0;n<vitri_end ; n++)    // xoa cau da lay du lieu
      {
        if(SMS[n]<'0'|SMS[n]>'9') SMS[n]=' '; //chi de lai so 0-9  
      }    
    }
  }
  ////////////////////
  if(JAS_IndexOf(SMS,"RELAY")>=0)
  {
    sms_sta=2; //chon dieu kien relay bang tin nhan  
  }
  ////////////////
  if(JAS_IndexOf(SMS,"ADMIN")>=0)
  {
    sms_sta=3; //chon luu sdt admin    
    ///tach sdt trong tin nhan///
    vitri_start=JAS_IndexOf(SMS,"\*")+1; // *
    vitri_end=JAS_IndexOf(SMS,"\#");    // #
    l=vitri_end-vitri_start;
    for(uint8_t n=0;n<l ; n++)
    {
      user[2][n]=SMS[n+vitri_start];    
    }
    
  }
  ///////////////
  if(JAS_IndexOf(SMS,"INFO")>=0)
  {
    sms_sta=4; //chon thong bao trang thai bang tin nhan
  }
  
  
}
///////tach sdt cuoc goi den /////////////
void cuoc_goi_den(void)
{
  uint8_t vitri_start,vitri_end;
  uint8_t l; //do dai chuoi
  for(uint8_t n=0;n<15; n++) // xoa du lieu truoc khi ghi
    {
      sdt_ing[n]=0;
    }
  vitri_start=JAS_IndexOf(LIP," \"")+2; // "+
  vitri_end=JAS_IndexOf(LIP,"\"\,");    // ",
  l=vitri_end-vitri_start;
  for(uint8_t n=0;n<l ; n++) // lay sdt dang nhan tin sms
    {
      sdt_ing[n]=LIP[n+vitri_start];    
    }   
}
void ATD (const char *goi_sdt)
{
  if(JAS_IndexOf(goi_sdt,"00")!=0)
  {
    printf("AT\r\n");
    HAL_Delay(500);
    HAL_IWDG_Refresh(&hiwdg); // IWDG_Refresh
    printf("ATD %s\;\r\n",goi_sdt);
    HAL_Delay(15000);
    HAL_IWDG_Refresh(&hiwdg); // IWDG_Refresh
    HAL_Delay(15000);
    printf("ATH\r\n");    
  }
 
}
///////// them +84... de gui sms///////
void sdt_84(void)
{
  
  for(uint8_t n=0;n<3;n++)
  {
    //if(JAS_IndexOf(user[n],"00")!=0 ) // khac 00...
    if(user[n][0]=='0'& user[n][1]!='0') // tu 01 den 09
    {
      sprintf(user_sms[n],"+84");
      for(uint8_t m=0;m<12;m++)
      {
          user_sms[n][m+3]=user[n][m+1];
      }    
    }      
  }

}
/////////tim chuoi trong chuoi/////
int JAS_IndexOf(const char *text, const char *key)
{
	int index = -1;
	char *found = strstr( text, key);
	if (found != NULL) index = found - text;
	return index;
}
////////kiem tra dieu kien ben ngoai////////////////
void GPIO_EXTI()
{
  uint8_t temp_Acquy; 

  temp_Acquy=!HAL_GPIO_ReadPin(IN_PORT,IN4_PIN);  
  DongCo=!HAL_GPIO_ReadPin(IN_PORT,IN2_PIN);
  if(DongCo==1) status=1; // dang mo may
    else        status=2; // dang tat may
  if(temp_Acquy==0&Acquy==1) SOS=1; // acquy vua ngat, bat khan cap SOS 3 lan

  Acquy=temp_Acquy; // luu lai gia tri acquy

}
/////////***************///////////////////
void GPS_server(void) ///gui toa do GPS len server
{
  uint8_t ket_thuc[3]={0x1a,0x0d,0x0a};
  if(server_78==0) dem_0078++; //dem so lan server ko tra loi
  else dem_0078=0;
  if(dem_0078>=3)
  {
    HAL_UART_Transmit(&huart1,ket_thuc,1, 1000); 
    HAL_Delay(500);
    Read_imei();
    TinhDataSever(1);
    Send_Server();    
  }
  /////////////////////
  Read_NEO();
  RRO=0; server_78=0;OK=0;
  if(GPS_sta==1)
  {
    TinhDataSever(2);
        
  }
  else
  {
     TinhDataSever(1);
     
  }
  Send_Server();
  for(uint8_t n=0;n<3;n++) //toi da 3 lan sua loi
  {
    if(RRO==1)    //gui bi loi
    {
      RRO=0; OK=0;
      Connet_Server();    //ket noi lai
      TinhDataSever(1);
      Send_Server();
      Read_NEO();       // gui them toa do
      TinhDataSever(2);
      Send_Server();
    }
    else
    {
      n=3; //thoat    
    }
  }
  

}
//xu ly cuoc goi
void cuoc_goi(void)
{

  if(goi_den==1)      // xu ly cuoc goi den
  {
    printf("ATH\r\n"); //ket thu cuoc goi
    HAL_Delay(500);
    cuoc_goi_den();
    HAL_Delay(3000); // cho on dinh trc khi doc danh ba
    read_Phonebook(); // doc danh ba
    if(JAS_IndexOf(CPBR,sdt_ing)>0)  // tim sdt vua goi trong danh ba
    {
      RELAY(0); //tat may
      HAL_IWDG_Refresh(&hiwdg); // IWDG_Refresh
      ATD(sdt_ing);     
    }   
    
    goi_den=0;
  }
}
// xu ly tin nhan sms
void tin_nhan(void)
{
  char noi_dung_sms[100];
  
  if(sms_den==1)      // xu ly tin nhan sms den
    {
      HAL_Delay(3000);  // cho nhan full sms      
      read_sms();
      HAL_IWDG_Refresh(&hiwdg); // IWDG_Refresh
      switch (sms_sta)
      {
        case 1:   // luu so dien thoai vao danh ba
        {
          save_Phonebook();
          HAL_Delay(2000); // cho on dinh trc khi doc danh ba
          read_Phonebook();
          sprintf(noi_dung_sms,"%s \r\n%s \r\n%s \r\n OK",user[0],user[1],user[2]);
          
          send_sms(sdt_ing,noi_dung_sms); // gui tin nhan phan hoi
          break;
        }
      case 2: // dieu kien relay
        {
          if(JAS_IndexOf(SMS,"ON")>=0) // lenh ON
          {
            RELAY(1);          
          }
          if(JAS_IndexOf(SMS,"OFF")>=0) // lenh OFF
          {
            RELAY(0);
          }
          HAL_Delay(500);
          GPIO_EXTI();  // kiem tra lai trang thai
          if(DongCo==1)
          {
            sprintf(noi_dung_sms,"trang thai: ON ");
          }
          else
          {
            sprintf(noi_dung_sms,"trang thai: OFF ");
          }
          
          send_sms(sdt_ing,noi_dung_sms); // gui tin nhan phan hoi
          break;
        } 
      case 3: // luu sdt admin
        {
          save_admin();
          HAL_Delay(2000); // cho on dinh trc khi doc danh ba
          read_Phonebook();
          sprintf(noi_dung_sms,"%s \r\n%s \r\n%s \r\n OK",user[0],user[1],user[2]);
          
          send_sms(sdt_ing,noi_dung_sms); // gui tin nhan phan hoi
          break;
        }
     case 4: // INFO
        {
          HAL_Delay(3000); // cho on dinh trc khi doc danh ba
          read_Phonebook();
          sprintf(noi_dung_sms,
                  "GNRMC: %s\r\n CPBR:%s\r\n LIP:%s\r\n DONG CO:%d\r\n ACQUY:%d\r\n https://maps.google.com/maps?q=%.5f,%.5f",GNRMC,CPBR,LIP,DongCo,Acquy,(double) map_N/1800000,(double) map_E/1800000);
          
          send_sms(sdt_ing,noi_dung_sms); // gui tin nhan phan hoi
          break;
        }
      }        
      
      HAL_IWDG_Refresh(&hiwdg); // IWDG_Refresh
      sms_den=0;
    
    }
}
// xu ly khan cap SOS
void khan_cap(void)
{
  uint8_t tg_nd[20]; //thoi gian _ noi dung
  if(SOS==1)      // xu ly truong hop khan cap
    {
      sprintf(tg_nd,"SOS\r\n %d:%d\r\nACQUY OFF",Hour,Minute);
      HAL_Delay(3000); // cho on dinh
      read_Phonebook();
      for(uint8_t n=0;n<3;n++)
      {        
        send_sms(user_sms[n],tg_nd); 
        HAL_Delay(5000);
        HAL_IWDG_Refresh(&hiwdg); // IWDG_Refresh
        ATD(user[n]);
      }
      SOS=0;    
    }

}
//dieu khien relay on off
void RELAY (uint8_t on)
{
  if(on==1)
  {
    HAL_GPIO_WritePin(OUT_PORT,OUT4_PIN,GPIO_PIN_RESET);  // on
  }
  else
  {
    Read_NEO(); // cap nhat lay van toc
    if(Speed==0) HAL_GPIO_WritePin(OUT_PORT,OUT4_PIN,GPIO_PIN_SET); //off neu dang dung V=0      
  
  } 

}
//kich hoat module sim
void PWR_KEY(void)
{
star:
    HAL_GPIO_WritePin(PWR_KEY_PORT,PWR_KEY_PIN,GPIO_PIN_RESET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(PWR_KEY_PORT,PWR_KEY_PIN,GPIO_PIN_SET);
    HAL_Delay(3000);
    OK=0;
    printf("AT\r\n");
    HAL_Delay(500);
    if(OK==0) goto star;
    HAL_Delay(3000);  
}

/**
* @brief This function handles Hard fault interrupt.
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
