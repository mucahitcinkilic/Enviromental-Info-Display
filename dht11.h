#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
extern "C" {
#endif
  
void DHT11_Init(void);

unsigned char DHT11_Read(uint8_t *humidity, uint8_t *temperature);

#ifdef __cplusplus
}
#endif

#endif
