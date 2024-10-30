#ifndef __DS3231_H
#define __DS3231_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////
// Saat/Tarih deðerleri BCD formatýnda
typedef struct {
  unsigned char sec;
  unsigned char min;
  unsigned char hour;
  unsigned char day;    // Haftanýn günü 1..7
  
  unsigned char date;   // Ayýn günü 1..31
  unsigned char mon;    // Ay 1..12
  unsigned char year;   // Yýl 0..99
  
  unsigned char a1_sec;
  unsigned char a1_min;
  unsigned char a1_hour;
  unsigned char a1_day;
  
  unsigned char a2_min;
  unsigned char a2_hour;
  unsigned char a2_day;
  
  unsigned char ctl;
  unsigned char stat;
  unsigned char ago;
  unsigned char temp_hi;
  unsigned char temp_lo;
} DS_DATE;
  
///////////////////////////////////////////

void DS3231_Init(void);
void DS3231_GetDate(DS_DATE *pdt);
void DS3231_SetDate(DS_DATE *pdt);

#ifdef __cplusplus
}
#endif

#endif