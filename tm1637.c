#include "tm1637.h"
#include "io.h"
#include "system.h"

#define Ack_Timeout 100000000




typedef enum{false,true}bool;



void TM1637_Init(void){
  //TM1637 IOP Configuration
  
  //start koþuluna hazýr olarak baþlasýn diye ikisinide 1 olarak konfigüre edip baþlattýk.
  IO_Write(TM1637_DIO, 1);
  IO_Init(TM1637_DIO, IO_MODE_OUTPUT_OD);

  IO_Write(TM1637_Clock, 1);
  IO_Init(TM1637_Clock, IO_MODE_OUTPUT_OD);
  
  Sys_DelayMs(1);
}

static void DIO_High(void){
  
IO_Write(TM1637_DIO, 1);

}

static void DIO_Low(void){
  
IO_Write(TM1637_DIO, 0);

}

static void Clock_High(void){
  
IO_Write(TM1637_Clock, 1);

}

static void Clock_Low(void){
  
IO_Write(TM1637_Clock, 0);

}

static void TM1637_Start(void){
  
  Clock_High();
  DIO_High();  
  Sys_DelayUs(2);
  DIO_Low();

}

// Ön koþul Clock = 0 olmalý
static void TM1637_Stop(void)
{
  Clock_Low();
  Sys_DelayUs(2);
  DIO_Low();    // Clock = 0, DIO = 0
  Sys_DelayUs(2);
  Clock_High();   // Clock = 1, DIO = 0
  Sys_DelayUs(2);
  DIO_High();   // Clock = 1 => DIO 0->1
                // Stop condition Clock=1, DIO=1  
}






static int waitforAck(void)
{
	Clock_Low();
	Sys_DelayUs(5); // After the falling edge of the eighth clock delay 5us
	              // ACK signals the beginning of judgment
        int time_passed=0;
	while (IO_Read(TM1637_DIO)){ // Check the state of the Data pin
          ++time_passed;
          if(time_passed >= Ack_Timeout)
            return false;
        }  
	Clock_High();
	Sys_DelayUs(2);
	Clock_Low();
        return true;
}




// Parametredei 8-bit deðeri bus üzerinden gönderir
// 1-bit ack deðerini okur ve geri döner
static void TM1637_WriteByte(unsigned char val)
{
	int i;
	for (i = 0; i<8; i++)
	{
		Clock_Low();
		if (val & 0x01) // low front
		{
			DIO_High();
		}
		else
		{
			DIO_Low();
		}
		Sys_DelayUs(3);
		val = val >> 1;
		Clock_High();
		Sys_DelayUs(3);
	}
}

int TM1637_WriteData (unsigned char Addr, unsigned char *data, int size)
{
	TM1637_Start();
	TM1637_WriteByte(0x40);
	if(!waitforAck()){
          return false;
        }
	TM1637_Stop();
        TM1637_Start();
	TM1637_WriteByte(Addr);
        if(!waitforAck()){
          return false;
        }
	for (int i=0; i<size; i++)
	{
          TM1637_WriteByte(data[i]);
          if(!waitforAck()){
          return false;
        }
	}
	TM1637_Stop();
        
        TM1637_Start();
	TM1637_WriteByte(0x8A);
        if(!waitforAck()){
          return false;
        }
	TM1637_Stop();
        return true;
}

int TM1637_WriteData2 (unsigned char Addr, unsigned char data)
{
	TM1637_Start();
	TM1637_WriteByte(0x40);
	if(!waitforAck()){
          return false;
        }
	TM1637_Stop();
        TM1637_Start();
	TM1637_WriteByte(Addr);
        if(!waitforAck()){
          return false;
        }
	//for (int i=0; i<4; i++)
	//{
          TM1637_WriteByte(data);
          if(!waitforAck()){
          return false;
        }
	//}
	TM1637_Stop();
        
        TM1637_Start();
	TM1637_WriteByte(0x8A);
        if(!waitforAck()){
          return false;
        }
	TM1637_Stop();
        return true;
}

void TM1637_Clear(void){
TM1637_WriteData2(0xC0, 0x00);
TM1637_WriteData2(0xC1, 0x00);
TM1637_WriteData2(0xC2, 0x00);
TM1637_WriteData2(0xC3, 0x00);
}

#if 0

void TM1637_Init(void){
  //TM1637 IOP Configuration
  
  //start koþuluna hazýr olarak baþlasýn diye ikisinide 1 olarak konfigüre edip baþlattýk.
  
  IO_Write(IOP_TM1637_DIO, 1);
  IO_Init(IOP_TM1637_DIO, IO_MODE_OUTPUT_OD);

  IO_Write(IOP_TM1637_Clock, 1);
  IO_Init(IOP_TM1637_Clock, IO_MODE_OUTPUT_OD);
  
  Sys_DelayMs(1);
}

static void DATA_HIGH(void){
  
IO_Write(IOP_TM1637_DIO, 1);

}

static void DATA_LOW(void){
  
IO_Write(IOP_TM1637_DIO, 0);

}

static void CLK_HIGH(void){
  
IO_Write(IOP_TM1637_Clock, 1);

}

static void CLK_LOW(void){
  
IO_Write(IOP_TM1637_Clock, 0);

}


void start (void)
{
	CLK_HIGH();
	DATA_HIGH();
	Sys_DelayUs(2);
	DATA_LOW();
}

void stop (void)
{
	CLK_LOW();
	Sys_DelayUs (2);
	DATA_LOW();
	Sys_DelayUs(2);
	CLK_HIGH();
	Sys_DelayUs(2);
	DATA_HIGH();
}

void writeByte (uint8_t byte)
{
	int i;
	for (i = 0; i<8; i++)
	{
		CLK_LOW();
		if (byte & 0x01) // low front
		{
			DATA_HIGH();
		}
		else
		{
			DATA_LOW();
		}
		Sys_DelayUs(3);
		byte = byte >> 1;
		CLK_HIGH();
		Sys_DelayUs(3);
	}
}

void waitforAck (void)
{
	CLK_LOW();
	Sys_DelayUs(5); // After the falling edge of the eighth clock delay 5us
	              // ACK signals the beginning of judgment
//	while (dio);  // Check the state of the Data pin
	CLK_HIGH();
	Sys_DelayUs(2);
	CLK_LOW();
}






void TM1637_WriteData (uint8_t Addr, uint8_t *data, int size)
{
	start();
	writeByte(0x40);
	waitforAck();
	stop();
        
        start();
	writeByte(Addr);
	waitforAck();
	for (int i=0; i<size; i++)
	{
		writeByte(data[i]);
		waitforAck();
	}
	stop();
        
        start();
	writeByte(0x8A);
	waitforAck();
	stop();
}

#endif
