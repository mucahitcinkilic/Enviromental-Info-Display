#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"
#include "oled.h"
#include "button.h"
#include "timer.h"
#include "tm1637.h"
#include "ds3231.h"
#include "dht11.h"

uint8_t humidity = 0;
uint8_t temperature = 0;
int success,fail;
DS_DATE dt;
int CaseDisplay=0;
int count=0;
int Task=0; 

typedef enum{
C=0x39,
degreeSymbol=0x63,
P=0x73,
}tm1637Char;

unsigned char numTab[10]={
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
};

unsigned char numTab_WithColon[10]={
0xBF,0x86, 0xDB,0xCF,0xE6,0xED,0xFD, 0x87, 0xFF, 0xEF,
};




void init(void)
{
  // I/O portlarý baþlangýç
  Sys_IoInit();
  
  // System Clock Tick baþlangýç
  Sys_TickInit();

  // Button fonksiyonlarý baþlangýç
  BTN_Init();
    
  // Konsol baþlangýç
  Sys_ConsoleInit();
  
  // LED baþlangýç
  IO_Write(IOP_LED, 1);
  IO_Init(IOP_LED, IO_MODE_OUTPUT_OD);  
  
  IO_Init(DlyTst, IO_MODE_OUTPUT);  
  
  // DS3231 baþlangýç
  DS3231_Init();
  
  TM1637_Init();
  
  DHT11_Init();
  
}



unsigned char BCDtoDecimal(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Bir basamaðý TM1637'ye yazmak için fonksiyon
void DisplayDigit(unsigned char position, unsigned char value) {

  switch(position){
  case 1: TM1637_WriteData2(0xC0, numTab[value]);
  break;
  case 2: TM1637_WriteData2(0xC1, numTab[value]);
  break;
  case 3: TM1637_WriteData2(0xC2, numTab[value]);
  break;
  case 4: TM1637_WriteData2(0xC3, numTab[value]);
  break;
  }
}

// Bir basamaðý TM1637'ye yazmak için fonksiyon
void DisplayDigit_WithColon(unsigned char position, unsigned char value) {

  switch(position){
  case 1: TM1637_WriteData2(0xC0, numTab_WithColon[value]);
  break;
  case 2: TM1637_WriteData2(0xC1, numTab_WithColon[value]);
  break;
  case 3: TM1637_WriteData2(0xC2, numTab_WithColon[value]);
  break;
  case 4: TM1637_WriteData2(0xC3, numTab_WithColon[value]);
  break;
  }
}

// Ýki haneli bir sayýyý TM1637'ye yazmak için fonksiyon
void DisplayTwoDigits(unsigned char position, unsigned char number) {
  unsigned char DecVal=BCDtoDecimal(number);
    if (DecVal >= 10) {
        DisplayDigit(position, DecVal / 10);   // Ýlk basamak
        DisplayDigit(position + 1, (DecVal % 10));  // Ýkinci basamak
        
    } else {
        DisplayDigit(position, 0);             // Tek haneli ise ilk basamaða 0
        DisplayDigit(position + 1, DecVal);    // Ýkinci basamaða sayýyý yaz
        
    }
}

// Ýki haneli bir sayýyý TM1637'ye yazmak için fonksiyon
void DisplayTwoDigits_WithColon(unsigned char position, unsigned char number) {
  unsigned char DecVal=BCDtoDecimal(number);
    if (DecVal >= 10) {
        DisplayDigit(position, DecVal / 10);   // Ýlk basamak
        DisplayDigit_WithColon(position + 1, (DecVal % 10));  // Ýkinci basamak
        
    } else {
        DisplayDigit(position, 0);             // Tek haneli ise ilk basamaða 0
        DisplayDigit_WithColon(position + 1, DecVal);    // Ýkinci basamaða sayýyý yaz
        
    }
}

// Zamaný TM1637'ye yazdýrma fonksiyonu
void DisplayTime(DS_DATE* dt) {

    // Saat
    DisplayTwoDigits_WithColon(1, dt->hour);
    // Dakika
    DisplayTwoDigits(3, dt->min);
    // Saniye istenirse eklenebilir, ama genellikle ekranda sürekli deðiþmesi istenmez
    // DisplayTwoDigits(0xC4, dt->sec);
}

// Ýki haneli bir sayýyý TM1637'ye yazmak için fonksiyon
void DisplayTwoDigits_NoBCD(unsigned char position, unsigned char number) {
    
    unsigned char DecNum= number;
    if (DecNum >= 10) {
        DisplayDigit(position, DecNum / 10);   // Ýlk basamak
        DisplayDigit(position + 1, DecNum % 10);  // Ýkinci basamak
    } else {
        DisplayDigit(position, 0);             // Tek haneli ise ilk basamaða 0
        DisplayDigit(position + 1, DecNum);    // Ýkinci basamaða sayýyý yaz
    }
}

void DisplayTemperature(unsigned char *temp){
TM1637_Clear();
DisplayTwoDigits_NoBCD(1,*temp);
TM1637_WriteData2(0xC2, degreeSymbol);
TM1637_WriteData2(0xC3, C);

}

void DisplayHumidity(unsigned char *hum){
TM1637_Clear();
DisplayTwoDigits_NoBCD(1,*hum);
TM1637_WriteData2(0xC3, P);

}


void task_dht(void){
  
// DHT11 sensöründen veri oku
        if (DHT11_Read(&humidity, &temperature) == 0) {
            // Baþarýlý okuma, sýcaklýk ve nemi yazdýr
           OLED_SetCursor(0, 0);
            printf("heat: %d C", temperature);
            OLED_SetCursor(1, 0);
            printf("Nem: %d",humidity);
            OLED_SetCursor(2, 0);
            printf("Suc: %d",++success);
        } else {
            // Hatalý okuma
           OLED_SetCursor(3, 0);
           printf("fail %d",++fail);
        }
       
}


void Task_ERTC(void)
{
  static unsigned char sec;

  
  DS3231_GetDate(&dt);
  

  sec = dt.sec;

  if(sec < 0x20){
   DisplayTime(&dt);
  }
  if(sec>=0x20 && sec < 0x40){
  DisplayTemperature(&temperature);
  }
  if(sec>=0x40){
  DisplayHumidity(&humidity);
  }
  
 OLED_SetCursor(4, 0);
            printf("Sec: %d",sec);
}



void Task_Print(void){
OLED_SetCursor(6,0);
printf("cnt= %d",++count);
}

void Display_Info(void){
    Task_Print();   
    task_dht();
    Sys_DelayMs(1000);
    Task_ERTC();
}


typedef enum{
CaseDisplayInfo=0,  
CaseCalibrateHour=1,
CaseCalibrateMin=2,
};



void CalibrateHour(void){
static unsigned char hour;
hour=dt.hour;

   DisplayTwoDigits_WithColon(1, dt.hour);      
   DisplayTwoDigits(3, dt.min);
   Sys_DelayMs(400);
   TM1637_Clear();
   Sys_DelayMs(100);

   
  if (g_Buttons[BTN_Enter]) {

    Task=CaseCalibrateMin;
    
    --g_Buttons[BTN_Enter];
  }  

       if (g_Buttons[BTN_Inc]) {
        
         ++hour;
         dt.hour=hour;
         DS3231_SetDate(&dt);
         
       --g_Buttons[BTN_Inc];
  } 
  
        if (g_Buttons[BTN_Dec]) {
         --hour;
         dt.hour=hour;
         DS3231_SetDate(&dt);       
        
          
        --g_Buttons[BTN_Dec];
  } 
  
}

void CalibrateMin(void){
static unsigned char min;
min=dt.min;

   DisplayTwoDigits_WithColon(1, dt.hour);      
   DisplayTwoDigits(3, dt.min);
   Sys_DelayMs(400);
   TM1637_Clear();
   Sys_DelayMs(100);

   
  if (g_Buttons[BTN_Enter]) {

    Task=CaseDisplayInfo;
    
    --g_Buttons[BTN_Enter];
  }  

       if (g_Buttons[BTN_Inc]) {
        
         ++min;
            dt.min=min;
   DS3231_SetDate(&dt);
  
       --g_Buttons[BTN_Inc];
  } 
  
        if (g_Buttons[BTN_Dec]) {
        
        --min;
                    dt.min=min;
   DS3231_SetDate(&dt);
          
        --g_Buttons[BTN_Dec];
  } 
  
}

void Task_Button(void)
{       
  
    
  if (g_Buttons[BTN_Enter] && g_Buttons[BTN_Inc]) {
    
    --g_Buttons[BTN_Enter];
    --g_Buttons[BTN_Inc];
    
    Task=CaseCalibrateHour; 
  }  
  
  switch(Task){
  case CaseDisplayInfo:   Display_Info();
  break;
  
  case CaseCalibrateHour: CalibrateHour();
  break;
  
  
  case CaseCalibrateMin: CalibrateMin();
  break;
  
  }
}



    
void main()
{
  // Baþlangýç yapýlandýrmalarý
  init();
  OLED_Rotate(1);
  OLED_SetFont(FNT_SMALL);
  
  Timer_Init(TIMER_4, 84, 1000, 1);  // Prescaler ve period ayarlarý
  Timer_IntConfig(TIMER_4, 1);
  Timer_Start(TIMER_4, 1);  // Timer baþlatýlmalý
  
  // Görev çevrimi
  while (1) 
  {
   Task_Button();
  }
}
