#ifndef __TM1637_H
#define __TM1637_H

#ifdef __cplusplus
extern "C" {
#endif
///////////////////////////////////////////

 #include <stdint.h> 
  
  /*
       A
       -
   F  |  | B
       -
       G
   E  |  | C
       -
       D

 XGFEDCBA s�ralama �rnk 0b00111111 = 0x3F yani 0 say�s� yanar 
 iki nokta i�in c1'in msb'sini 1 yapmak gerek
   �rnk C harfi i�in 0b00111001 = 0x39
     �rnk 5 rakam�= 0b0110 1101
       P= 0b0111 0011
  */
  
  
  enum displaycharacters{
display_0=0x3F,
display_1=0x06,
display_2=0x5B,
display_3=0x4F,
display_4=0x66,
display_5=0x6D,
display_6=0x7D,
display_7=0x07,
display_8=0x7F,
display_9=0x6F,
};


void TM1637_Init(void);
int TM1637_WriteData(unsigned char Addr, unsigned char *data, int size);
int TM1637_WriteData2(unsigned char Addr, unsigned char data);
void TM1637_Clear(void);

///////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif