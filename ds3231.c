#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "io.h"
#include "system.h"
#include "si2c.h"
#include "ds3231.h"

enum { FALSE, TRUE };

#define I2C_ADR_DS3231  0xD0

void DS3231_Init(void)
{
  SI2C_Init();
}

void DS3231_GetDate(DS_DATE *pdt)
{
  SI2C_ReadB(I2C_ADR_DS3231, 0, pdt, 7);
}

void DS3231_SetDate(DS_DATE *pdt)
{
  SI2C_WriteB(I2C_ADR_DS3231, 0, pdt, 7);
}
