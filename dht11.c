#include "io.h"
#include "timer.h"
#include "system.h"

// DHT11 Data Pin (kullanaca��n�z pine g�re ayarlay�n)
#define DHT11_PIN               IOP_IC // DHT11 data pinini buraya atay�n
#define TIMEOUT_US              1000000

void DHT11_Init(void){
IO_Write(DHT11_PIN, 1); // DHT11 data pinini LOW yap
IO_Init(DHT11_PIN, IO_MODE_OUTPUT_OD); // Open-drain output moduna al
}


// DHT11'e ba�lang�� sinyali g�nderme
void DHT11_Start(void) {
    //DelayMs(300);
    
    IO_Write(DHT11_PIN, 0); // DHT11 data pinini LOW yap
    Sys_DelayMs(18); // 18ms boyunca d���k sinyal g�nder
    IO_Write(DHT11_PIN, 1); // Pin'i high yap
    Sys_DelayUs(30); // 20-40 us bekle
    //IO_Init(DHT11_PIN, IO_MODE_INPUT);
}

int DHT11_CheckSignalPolarity(void){
  clock_t t0,t1; 
  clock_t startTime = clock();  // Zaman a��m� i�in ba�lang�� zaman�n� al
  uint32_t timeout = TIMEOUT_US; // Zaman a��m� s�resi
   while(!IO_Read(DHT11_PIN)){
        if ((clock() - startTime) > timeout) {
            return -1; // Timeout durumunda hata d�nd�r
        }
    }
   
   t0=clock();
   startTime = clock();  // Y�ksek sinyalin ba�lang�c�n� tekrar ba�lat
   while(IO_Read(DHT11_PIN)){
        if ((clock() - startTime) > timeout) {
            return -1; // Timeout durumunda hata d�nd�r
        }
    }
   t1=clock()-t0;
   if(t1>15 && t1<40)
     return 0;
   if(t1>45 && t1<85)
     return 1;
   return -1;
}

// DHT11 verilerini okuma
unsigned char DHT11_Read(uint8_t *humidity, uint8_t *temperature) {
   unsigned char data[5] = {0};  // 40 bitlik veri (8 bit nem, 8 bit s�cakl�k, CRC vs.)
   
   // Zaman a��m� i�in ba�lang�� zaman�n� al
    clock_t startTime = clock();
    uint32_t timeout = TIMEOUT_US; // Timeout s�resi
    
    DHT11_Start();  // Ba�lang�� sinyali g�nder
   
    // DHT11 sens�r�nden kaynakl� veri gelmeme durumu i�in timeout
    if ((clock() - startTime) > timeout) {
        return 1;  // Timeout durumunda hata d�nd�r
    }
    
    while(!IO_Read(DHT11_PIN)){
        if ((clock() - startTime) > timeout) {
            return 1;  // Timeout durumunda hata d�nd�r
        }
    }
    
    while(IO_Read(DHT11_PIN)){
        if ((clock() - startTime) > timeout) {
            return 1;  // Timeout durumunda hata d�nd�r
        }
    }
    
    for(int j=0;j<5;++j){
    for (uint8_t i = 0; i < 8; i++) {
      data[j]<<=1; 
      data[j] |= DHT11_CheckSignalPolarity();
    }
    }

    
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        return 1;  // Hatal� okuma
    }

    // Verileri d�nd�r
    *humidity = data[0];
    *temperature = data[2];
    
    return 0;  // Ba�ar�l� okuma
}

