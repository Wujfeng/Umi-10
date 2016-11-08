/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    SCC2000.c
 * Date:         2016.03.22
 * Description:  SCC2000 Driver, SPI Communication Interface
*****************************************************************/


#include "SCC2000.h"
#include "SPI2.h"

SCC2000 scc2000;

/****************************************************
 * Function:    SCC2000_Reset
 * Description: Reset SCC2000.
 * Input:
 * Output:
 * Return:
*****************************************************/
void SCC2000_Reset(void)
{
  SCC2230_ReadReg(REQ_HARD_RESET);
}

/****************************************************
 * Function:    SCC2000_Init
 * Description: Initialization SCC2000.
 * Input:
 * Output:
 * Return:
*****************************************************/
void SCC2000_Init(void)
{
  /* Configurable Filter WB 60Hz */
  SCC2230_ReadReg(REQ_WRITE_FLT_60);
}

/****************************************************
 * Function:    SCC2000_Status
 * Description: Detection SCC2000 State.
 * Input:
 * Output:
 * Return:      1: Normal, 0: Unusual
*****************************************************/
uint8_t SCC2000_Status(void)
{
  uint16_t data, i;
  
  for(i=0; i<65535; i++)
  {
    data = SCC2230_ReadReg(REQ_READ_RATE_STAT1);
    if( (data&0xC03F) != 0xC03F )
    {
      continue;
    }
    
    data = SCC2230_ReadReg(REQ_READ_RATE_STAT2);
    if( (data&0x01FF) != 0x01FF )
    {
      continue;
    }
    
    data = SCC2230_ReadReg(REQ_READ_ACC_STAT);
    if( (data&0x004F) != 0x004F )
    {
      continue;
    }
    else
    {
      return 1;
    }
  }
  
  return 0;
}

/****************************************************
 * Function:    SCC2230_ReadReg
 * Description: Read data from SCC2230 register.
 * Input:       ADDR: SCC2230 Register Address
 * Output:
 * Return:      Data register
*****************************************************/
int16_t SCC2230_ReadReg(uint32_t ADDR)
{
  uint32_t data, i;
  
  for(i=0; i<0x05; i++);

  SCC2000_GYRO_Z_CS_LOW();

  SCC2000_GYRO_Z_SendHalfWord(ADDR>>16);
  SCC2000_GYRO_Z_SendHalfWord(ADDR&0x0000ffff);

  SCC2000_GYRO_Z_CS_HIGH();

  for(i=0; i<0x05; i++);

  SCC2000_GYRO_Z_CS_LOW();

  data = SCC2000_GYRO_Z_SendHalfWord(ADDR>>16);
  data <<= 16;
  data |= SCC2000_GYRO_Z_SendHalfWord(ADDR&0x0000ffff);

  SCC2000_GYRO_Z_CS_HIGH();
  
  for(i=0; i<0x05; i++);

  data = (data&0x00ffff00)>>8;

  return(data);
}

/****************************************************
 * Function:    SCC2000_Read
 * Description: Read SCC2000 ACC GYRO Data.
 * Input:       &rm3100
 * Output:
 * Return:
*****************************************************/
void SCC2000_Read(SCC2000* scc2000)
{
  scc2000->ACC_X = SCC2230_ReadReg(REQ_READ_ACC_X);
  scc2000->ACC_Y = SCC2230_ReadReg(REQ_READ_ACC_Y);
  scc2000->ACC_Z = SCC2230_ReadReg(REQ_READ_ACC_Z);

  scc2000->GYRO_Z = SCC2230_ReadReg(REQ_READ_RATE);
}

/****************************************************
 * Function:    CalculateCRC
 * Description: Data Check.
 * Input:       Data
 * Output:
 * Return:      CRc
*****************************************************/
uint8_t CalculateCRC(uint32_t Data)
{
  uint8_t BitIndex;
  uint8_t BitValue;
  uint8_t CRc;
  
  CRc = 0xFF;
  for (BitIndex = 31; BitIndex > 7; BitIndex--)
  {
    BitValue = (uint8_t)((Data >> BitIndex) & 0x01);
    CRc = CRc8(BitValue, CRc);
  }
  CRc = (uint8_t)~CRc;
  return CRc;
}

/****************************************************
 * Function:    CRc8
 * Description: Data Check.
 * Input:       BitValue
 *              CRc
 * Output:
 * Return:      CRc
*****************************************************/
uint8_t CRc8(uint8_t BitValue, uint8_t CRc)
{
  uint8_t Temp;
  
  Temp = (uint8_t)(CRc & 0x80);
  if (BitValue == 0x01)
  {
    Temp ^= 0x80;
  }
  CRc <<= 1;
  if (Temp > 0)
  {
    CRc ^= 0x1D;
  }
  return CRc;
}

