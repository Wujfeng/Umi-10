/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Command.c
 * Date:         2016.03.22
 * Description:  Command Protocol
*****************************************************************/


#include "Command.h"


/****************************************************
 * Function:    Data_Decoding
 * Description: Data Subcontracting Decoding.
 * Input:       &data
 * Output:
 * Return:
*****************************************************/
void Data_Decoding(uint8_t* data)
{
  uint8_t Data_Frame[100];
  uint16_t i = 0, Frame_Flag = 0, Start = 0, End = 0, geshu = 0;

  geshu = USART1_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream2);

  for(i=0; i<geshu; i++)
  {
    if(data[i]=='$')
    {
      Start = i;
      Frame_Flag = 1;
    }

    if(Frame_Flag==1 && data[i]==0x0D && data[i+1]==0x0A)
    {
      End = i+1;
      Frame_Flag = 2;
    }
    else if(Frame_Flag==1 && i==(geshu-1))
    {
      End = i;
      Frame_Flag = 2;
    }

    if(Frame_Flag==2)
    {
      memset(Data_Frame, 0, sizeof(Data_Frame));
      memcpy(Data_Frame, data+Start, End-Start+1);
      Command_Class(Data_Frame);
      while(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7)==0);

      Frame_Flag = 0;
      
      if(End != (geshu-1))
      {
        Delay_ms(10);
      }
    }
  }
}

/****************************************************
 * Function:    Command_Class
 * Description: Command Subcontracting Decoding.
 * Input:       &data
 * Output:
 * Return:
*****************************************************/
void Command_Class(uint8_t* data)
{
  if(!memcmp("$cmd,get,", data, 9))
  {
    Get_Class(data);
  }
  else if(!memcmp("$cmd,set,", data, 9))
  {
    Set_Class(data);
  }
  else if(!memcmp("$cmd,save", data, 9))
  {
    Save_Config();
  }
  else if(!memcmp("$cmd,erase", data, 10))
  {
    Factory_Reset();
  }
  else if(!memcmp("$cmd,data_upload,", data, 17))
  {
    Upload(data);
  }
  else
    Out_Command_Bad();
}

/****************************************************
 * Function:    Get_Class
 * Description: Get Command.
 * Input:       &data
 * Output:
 * Return:
*****************************************************/
void Get_Class(uint8_t* data)
{
  /********************** Gyro Scale ***************************/
  if(!memcmp("$cmd,get,gyro,kgs", data, 17))
  {
    Get_GyroScale(&(system_parameter.imu_dem));
  }
  /********************** Gyro Zero ***************************/
  else if(!memcmp("$cmd,get,gyro,kgz", data, 17))
  {
    Get_GyroZero(&(system_parameter.imu_dem));
  }
  /********************** Gyro Gyron ************************/
  else if(!memcmp("$cmd,get,gyro,gyron0", data, 20))
  {
    Get_GyroGyron(&(system_parameter.imu_dem), 0);
  }
  else if(!memcmp("$cmd,get,gyro,gyron1", data, 20))
  {
    Get_GyroGyron(&(system_parameter.imu_dem), 1);
  }
  else if(!memcmp("$cmd,get,gyro,gyron2", data, 20))
  {
    Get_GyroGyron(&(system_parameter.imu_dem), 2);
  }
  /********************** Acc Scale **************************/
  else if(!memcmp("$cmd,get,acce,kas", data, 17))
  {
    Get_AccScale(&(system_parameter.imu_dem));
  }
  /********************** Acc Zero **************************/
  else if(!memcmp("$cmd,get,acce,kaz", data, 17))
  {
    Get_AccZero(&(system_parameter.imu_dem));
  }
  /********************** Acc Accn ************************/
  else if(!memcmp("$cmd,get,acce,accn0", data, 19))
  {
    Get_AccAccn(&(system_parameter.imu_dem), 0);
  }
  else if(!memcmp("$cmd,get,acce,accn1", data, 19))
  {
    Get_AccAccn(&(system_parameter.imu_dem), 1);
  }
  else if(!memcmp("$cmd,get,acce,accn2", data, 19))
  {
    Get_AccAccn(&(system_parameter.imu_dem), 2);
  }
  
  else if(!memcmp("$cmd,get,mode", data, 13))
  {
    Get_Mode(system_parameter.MODE);
  }
  
  else if(!memcmp("$cmd,get,bmi055,range", data, 21))
  {
    Get_BMI055_Range(system_parameter.BMI055_Range);
  }
  else if(!memcmp("$cmd,get,bmi055,bw", data, 18))
  {
    Get_BMI055_BW(system_parameter.BMI055_BW);
  }

  else if(!memcmp("$cmd,get,id", data, 11))
  {
    Get_ID(system_parameter.ID);
  }

  else
    Out_Command_Bad();
}

/****************************************************
 * Function:    Set_Class
 * Description: Set Command.
 * Input:       &data
 * Output:
 * Return:
*****************************************************/
void Set_Class(uint8_t* data)
{
  /********************** Gyro Scale ***************************/
  if(!memcmp("$cmd,set,gyro,kgs,", data, 18))
  {
    Setup_GyroScale(data, &(system_parameter.imu_dem));
  }
  /********************** Gyro Zero ***************************/
  else if(!memcmp("$cmd,set,gyro,kgz,", data, 18))
  {
    Setup_GyroZero(data, &(system_parameter.imu_dem));
  }
  /********************** Gyro Gyron ************************/
  else if(!memcmp("$cmd,set,gyro,gyron0,", data, 21))
  {
    Setup_GyroGyron(data, &(system_parameter.imu_dem), 0);
  }
  else if(!memcmp("$cmd,set,gyro,gyron1,", data, 21))
  {
    Setup_GyroGyron(data, &(system_parameter.imu_dem), 1);
  }
  else if(!memcmp("$cmd,set,gyro,gyron2,", data, 21))
  {
    Setup_GyroGyron(data, &(system_parameter.imu_dem), 2);
  }
  /********************** Acc Scale **************************/
  else if(!memcmp("$cmd,set,acce,kas,", data, 18))
  {
    Setup_AccScale(data, &(system_parameter.imu_dem));
  }
  /********************** Acc Zero **************************/
  else if(!memcmp("$cmd,set,acce,kaz,", data, 18))
  {
    Setup_AccZero(data, &(system_parameter.imu_dem));
  }
  /********************** Acc Accn ************************/
  else if(!memcmp("$cmd,set,acce,accn0,", data, 20))
  {
    Setup_AccAccn(data, &(system_parameter.imu_dem), 0);
  }
  else if(!memcmp("$cmd,set,acce,accn1,", data, 20))
  {
    Setup_AccAccn(data, &(system_parameter.imu_dem), 1);
  }
  else if(!memcmp("$cmd,set,acce,accn2,",data, 20))
  {
    Setup_AccAccn(data, &(system_parameter.imu_dem), 2);
  }
  
  else if(!memcmp("$cmd,set,mode,",data, 14))
  {
    Setup_Mode(data, &(system_parameter.MODE));
  }
  
  else if(!memcmp("$cmd,set,bmi055,range,",data, 22))
  {
    Setup_BMI055_Range(data, &(system_parameter.BMI055_Range));
  }
  else if(!memcmp("$cmd,set,bmi055,bw,",data, 19))
  {
    Setup_BMI055_BW(data, &(system_parameter.BMI055_BW));
  }

  else if(!memcmp("$cmd,set,id,",data, 12))
  {
    Setup_ID(data, &(system_parameter.ID));
  }

  else
    Out_Command_Bad();
}

/****************************************************
 * Function:    Upload
 * Description: Data Upload Command.
 * Input:       &data
 * Output:
 * Return:
*****************************************************/
void Upload(uint8_t* data)
{
  if(!memcmp("$cmd,data_upload,on", data, 19))
  {
    Out_Config_OK();
    system_parameter.Data_Upload = 0;
  }
  else if(!memcmp("$cmd,data_upload,off", data, 20))
  {
    system_parameter.Data_Upload = 1;
    Out_Config_OK();
  }
  else
    Out_Command_Bad();
}


/****************************************************
 * Function:    Save_Config
 * Description: Save system parameters.
 * Input:
 * Output:
 * Return:
*****************************************************/
void Save_Config(void)
{
  Flash_Write(&system_parameter);
  Out_Config_OK();
}

/****************************************************
 * Function:    Factory_Reset
 * Description: Restore factory system parameters.
 * Input:
 * Output:
 * Return:
*****************************************************/
void Factory_Reset(void)
{
  Flash_Init(&system_parameter);
  Flash_Write(&system_parameter);
  Out_Config_OK();
}

/****************************************************
 * Function:    Out_Command_Bad
 * Description: Output error command prompt statement.
 * Input:
 * Output:
 * Return:
*****************************************************/
void Out_Command_Bad(void)
{
  USART1_OUT((uint8_t*)"$cmd,Bad,Command*ff\r\n", 21);
}

/****************************************************
 * Function:    Out_Config_OK
 * Description: Output Settings success precautionary statements.
 * Input:
 * Output:
 * Return:
*****************************************************/
void Out_Config_OK(void)
{
  USART1_OUT((uint8_t*)"$cmd,Config,OK*ff\r\n", 19);
}

/****************************************************
 * Function:    packsum
 * Description: Parity ASCII data.
 * Input:       &Buffer: ASCII data
 *              lenght: Data lenght
 * Output:
 * Return:
*****************************************************/
void packsum(uint8_t* Buffer ,uint16_t lenght)
{
  uint8_t sum1 = 0;
  uint8_t sum2 = 0;
  uint8_t cmpsum = 0;
  uint16_t i;
  for(i=1; i<(lenght-1); i++)
  {
    cmpsum ^= Buffer[i];
  }
  sum1 = cmpsum / 16;
  if(sum1<10)
  {
    sum1 = sum1 + 48;
  }
  else
  {
    sum1 = sum1 + 55;
  }
  sum2 = cmpsum % 16;
  if(sum2<10)
  {
    sum2 = sum2 + 48;
  }
  else
  {
    sum2 = sum2 + 55;
  }
  Buffer[lenght] = sum1;
  Buffer[lenght+1] = sum2;
}

/****************************************************
 * Function:    Get_GyroScale
 * Description: Get Gyro Scale.
 * Input:       &imu_dem
 * Output:
 * Return:
*****************************************************/
void Get_GyroScale(IMU_Dem* imu_dem)
{
  uint8_t SendNum = 0;
  uint8_t i = 0;
  char buff[30] = {0};
  uint8_t numlong = 0;
  uint8_t OutNum = 0;
  uint8_t data[100] = {0};
  float OutData[3] = {0.0f};

  memcpy(data, "$cmd,set,gyro,kgs", 17);
  SendNum = 17;
  OutData[0] = imu_dem->GYRO_Scale[0];
  OutData[1] = imu_dem->GYRO_Scale[1];
  OutData[2] = imu_dem->GYRO_Scale[2];
  OutNum = 3;

  for(i=0; i<OutNum; i++)
  {
    data[SendNum++] = ',';
    numlong = sprintf(buff, "%.7f", OutData[i]);
    memcpy((data+SendNum), buff, numlong);
    SendNum += numlong;
  }
  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_GyroZero
 * Description: Get Gyro Zero.
 * Input:       &imu_dem
 * Output:
 * Return:
*****************************************************/
void Get_GyroZero(IMU_Dem* imu_dem)
{
  uint8_t SendNum = 0;
  uint8_t i = 0;
  char buff[30] = {0};
  uint8_t numlong = 0;
  uint8_t OutNum = 0;
  uint8_t data[100] = {0};
  float OutData[3]= {0.0f};

  memcpy(data, "$cmd,set,gyro,kgz", 17);
  SendNum = 17;
  OutData[0] = imu_dem->GYRO_Zero[0];
  OutData[1] = imu_dem->GYRO_Zero[1];
  OutData[2] = imu_dem->GYRO_Zero[2];
  OutNum = 3;

  for(i=0; i<OutNum; i++)
  {
    data[SendNum++] = ',';
    numlong = sprintf(buff, "%.7f", OutData[i]);
    memcpy((data+SendNum), buff,numlong);
    SendNum += numlong;
  }
  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_GyroGyron
 * Description: Get Gyro Cross-coupling.
 * Input:       &imu_dem
 *              Index
 * Output:
 * Return:
*****************************************************/
void Get_GyroGyron(IMU_Dem* imu_dem, uint8_t Index)
{
  uint8_t SendNum = 0;
  uint8_t i = 0;
  char buff[30] = {0};
  uint8_t numlong = 0;
  uint8_t OutNum = 0;
  uint8_t data[100] = {0};
  float OutData[3]= {0.0f};

  switch(Index)
  {
    case 0:
    {
      memcpy(data, "$cmd,set,gyro,gyron0", 20);
      SendNum = 20;
      OutData[0] = imu_dem->GYRO_Coupling[0][0];
      OutData[1] = imu_dem->GYRO_Coupling[0][1];
      OutData[2] = imu_dem->GYRO_Coupling[0][2];
      OutNum = 3;
      break;
    }
    case 1:
    {
      memcpy(data,"$cmd,set,gyro,gyron1",20);
      SendNum = 20;
      OutData[0] = imu_dem->GYRO_Coupling[1][0];
      OutData[1] = imu_dem->GYRO_Coupling[1][1];
      OutData[2] = imu_dem->GYRO_Coupling[1][2];
      OutNum = 3;
      break;
    }
    case 2:
    {
      memcpy(data, "$cmd,set,gyro,gyron2", 20);
      SendNum = 20;
      OutData[0] = imu_dem->GYRO_Coupling[2][0];
      OutData[1] = imu_dem->GYRO_Coupling[2][1];
      OutData[2] = imu_dem->GYRO_Coupling[2][2];
      OutNum = 3;
      break;
    }
    default:
    {
      Out_Command_Bad();
      return;
    }
  }

  for(i=0; i<OutNum; i++)
  {
    data[SendNum++] = ',';
    numlong = sprintf(buff, "%.7f", OutData[i]);
    memcpy((data+SendNum), buff,numlong);
    SendNum += numlong;
  }
  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_AccScale
 * Description: Get Acc Scale.
 * Input:       &imu_dem
 * Output:
 * Return:
*****************************************************/
void Get_AccScale(IMU_Dem* imu_dem)
{
  uint8_t SendNum = 0;
  uint8_t i = 0;
  char buff[30] = {0};
  uint8_t numlong = 0;
  uint8_t OutNum = 0;
  uint8_t data[100] = {0};
  float OutData[3] = {0.0f};

  memcpy(data, "$cmd,set,acce,kas", 17);
  SendNum = 17;
  OutData[0] = imu_dem->ACC_Scale[0];
  OutData[1] = imu_dem->ACC_Scale[1];
  OutData[2] = imu_dem->ACC_Scale[2];
  OutNum = 3;

  for(i=0; i<OutNum; i++)
  {
    data[SendNum++] = ',';
    numlong = sprintf(buff, "%.7f", OutData[i]);
    memcpy((data+SendNum), buff, numlong);
    SendNum += numlong;
  }
  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_AccZero
 * Description: Get Acc Zero.
 * Input:       &imu_dem
 * Output:
 * Return:
*****************************************************/
void Get_AccZero(IMU_Dem* imu_dem)
{
  uint8_t SendNum = 0;
  uint8_t i = 0;
  char buff[30] = {0};
  uint8_t numlong = 0;
  uint8_t OutNum = 0;
  uint8_t data[100] = {0};
  float OutData[3] = {0.0f};

  memcpy(data, "$cmd,set,acce,kaz", 17);
  SendNum = 17;
  OutData[0] = imu_dem->ACC_Zero[0];
  OutData[1] = imu_dem->ACC_Zero[1];
  OutData[2] = imu_dem->ACC_Zero[2];
  OutNum = 3;

  for(i=0; i<OutNum; i++)
  {
    data[SendNum++] = ',';
    numlong = sprintf(buff, "%.7f", OutData[i]);
    memcpy((data+SendNum), buff,numlong);
    SendNum += numlong;
  }
  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_AccAccn
 * Description: Get Acc Cross-coupling.
 * Input:       &imu_dem
 *              Index
 * Output:
 * Return:
*****************************************************/
void Get_AccAccn(IMU_Dem* imu_dem, uint8_t Index)
{
  uint8_t SendNum = 0;
  uint8_t i = 0;
  char buff[30] = {0};
  uint8_t numlong = 0;
  uint8_t OutNum = 0;
  uint8_t data[100] = {0};
  float OutData[3] = {0.0f};

  switch(Index)
  {
    case 0:
    {
      memcpy(data, "$cmd,set,acce,accn0", 19);
      SendNum = 19;
      OutData[0] = imu_dem->ACC_Coupling[0][0];
      OutData[1] = imu_dem->ACC_Coupling[0][1];
      OutData[2] = imu_dem->ACC_Coupling[0][2];
      OutNum = 3;
      break;
    }
    case 1:
    {
      memcpy(data, "$cmd,set,acce,accn1", 19);
      SendNum = 19;
      OutData[0] = imu_dem->ACC_Coupling[1][0];
      OutData[1] = imu_dem->ACC_Coupling[1][1];
      OutData[2] = imu_dem->ACC_Coupling[1][2];
      OutNum = 3;
      break;
    }
    case 2:
    {
      memcpy(data, "$cmd,set,acce,accn2", 19);
      SendNum = 19;
      OutData[0] = imu_dem->ACC_Coupling[2][0];
      OutData[1] = imu_dem->ACC_Coupling[2][1];
      OutData[2] = imu_dem->ACC_Coupling[2][2];
      OutNum = 3;
      break;
    }
    default:
    {
      Out_Command_Bad();
      return;
    }
  }

  for(i=0; i<OutNum; i++)
  {
    data[SendNum++] = ',';
    numlong = sprintf(buff, "%.7f", OutData[i]);
    memcpy((data+SendNum), buff, numlong);
    SendNum += numlong;
  }
  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_Mode
 * Description: Get Mode.
 * Input:       mode
 * Output:
 * Return:
*****************************************************/
void Get_Mode(uint32_t mode)
{
  uint8_t SendNum = 0;
  uint8_t data[30] = {0};

  if(mode == 0)
  {
    memcpy(data, "$cmd,set,mode,nav", 17);
    SendNum = 17;
  }
  else
  {
    memcpy(data, "$cmd,set,mode,imubd", 19);
    SendNum = 19;
  }

  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_BMI055_Range
 * Description: Get BMI055 Range.
 * Input:       Range
 * Output:
 * Return:
*****************************************************/
void Get_BMI055_Range(uint32_t Range)
{
  uint8_t SendNum = 0;
  uint8_t data[50] = {0};

  if(Range == GYRO_125dps)
  {
    memcpy(data, "$cmd,set,bmi055,range,125dps", 28);
    SendNum = 28;
  }
  else if(Range == GYRO_250dps)
  {
    memcpy(data, "$cmd,set,bmi055,range,250dps", 28);
    SendNum = 28;
  }
  else if(Range == GYRO_500dps)
  {
    memcpy(data, "$cmd,set,bmi055,range,500dps", 28);
    SendNum = 28;
  }
  else if(Range == GYRO_1000dps)
  {
    memcpy(data, "$cmd,set,bmi055,range,1000dps", 29);
    SendNum = 29;
  }
  else
  {
    memcpy(data, "$cmd,set,bmi055,range,2000dps", 29);
    SendNum = 29;
  }

  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_BMI055_BW
 * Description: Get BMI055 Filter Bandwidth.
 * Input:       BW
 * Output:
 * Return:
*****************************************************/
void Get_BMI055_BW(uint32_t BW)
{
  uint8_t SendNum = 0;
  uint8_t data[50] = {0};

  if(BW == GYRO_BW_12Hz)
  {
    memcpy(data, "$cmd,set,bmi055,bw,12Hz", 23);
    SendNum = 23;
  }
  else if(BW == GYRO_BW_32Hz)
  {
    memcpy(data, "$cmd,set,bmi055,bw,32Hz", 23);
    SendNum = 23;
  }
  else if(BW == GYRO_BW_23Hz)
  {
    memcpy(data, "$cmd,set,bmi055,bw,23Hz", 23);
    SendNum = 23;
  }
  else if(BW == GYRO_BW_64Hz)
  {
    memcpy(data, "$cmd,set,bmi055,bw,64Hz", 23);
    SendNum = 23;
  }
  else if(BW == GYRO_BW_47Hz)
  {
    memcpy(data, "$cmd,set,bmi055,bw,47Hz", 23);
    SendNum = 23;
  }
  else if(BW == GYRO_BW_116Hz)
  {
    memcpy(data, "$cmd,set,bmi055,bw,116Hz", 24);
    SendNum = 24;
  }
  else if(BW == GYRO_BW_230Hz)
  {
    memcpy(data, "$cmd,set,bmi055,bw,230Hz", 24);
    SendNum = 24;
  }
  else
  {
    memcpy(data, "$cmd,set,bmi055,bw,523Hz", 24);
    SendNum = 24;
  }

  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Get_ID
 * Description: Get Product ID.
 * Input:       id
 * Output:
 * Return:
*****************************************************/
void Get_ID(uint32_t id)
{
  uint8_t SendNum = 0;
  uint8_t data[30] = {0};

  memcpy(data, "$cmd,set,id", 11);
  SendNum = 11;
  data[SendNum++] = ',';

  data[SendNum++] = (uint8_t)(id>>24);
  data[SendNum++] = (uint8_t)((id>>16)&0x000000FF);
  data[SendNum++] = (uint8_t)((id>>8)&0x000000FF);
  data[SendNum++] = (uint8_t)(id&0x000000FF);

  data[SendNum++] = '*';
  packsum(data, SendNum);
  SendNum = SendNum + 2;
  data[SendNum++] = 0x0d;
  data[SendNum++] = 0x0a;

  USART1_OUT(data, SendNum);
}

/****************************************************
 * Function:    Setup_GyroScale
 * Description: Setup Gyro Scale.
 * Input:       data
 *              &imu_dem
 * Output:
 * Return:
*****************************************************/
void Setup_GyroScale(uint8_t* data, IMU_Dem* imu_dem)
{
  char Temp_Buffer[3][30] = {0};
  uint8_t Buffer_P1 = 0, Buffer_P2 = 0;
  uint8_t P_Offset = 18;
  while((data[P_Offset] != '*'))
  {
    if(data[P_Offset] != ',')
      Temp_Buffer[Buffer_P1][Buffer_P2++] = data[P_Offset];
    else
    {
      Buffer_P1++;
      Buffer_P2 = 0;
    }
    P_Offset++;
  }

  imu_dem->GYRO_Scale[0] = atof(Temp_Buffer[0]);
  imu_dem->GYRO_Scale[1] = atof(Temp_Buffer[1]);
  imu_dem->GYRO_Scale[2] = atof(Temp_Buffer[2]);
      
  Out_Config_OK();
}

/****************************************************
 * Function:    Setup_GyroZero
 * Description: Setup Gyro Zero.
 * Input:       data
 *              &imu_dem
 * Output:
 * Return:
*****************************************************/
void Setup_GyroZero(uint8_t* data, IMU_Dem* imu_dem)
{
  char Temp_Buffer[3][30] = {0};
  uint8_t Buffer_P1 = 0, Buffer_P2 = 0;
  uint8_t P_Offset = 18;
  while((data[P_Offset] != '*'))
  {
    if(data[P_Offset] != ',')
      Temp_Buffer[Buffer_P1][Buffer_P2++] = data[P_Offset];
    else
    {
      Buffer_P1++;
      Buffer_P2 = 0;
    }
    P_Offset++;
  }

  imu_dem->GYRO_Zero[0] = atof(Temp_Buffer[0]);
  imu_dem->GYRO_Zero[1] = atof(Temp_Buffer[1]);
  imu_dem->GYRO_Zero[2] = atof(Temp_Buffer[2]);
  
  Out_Config_OK();
}

/****************************************************
 * Function:    Setup_GyroGyron
 * Description: Setup Gyro Cross-coupling.
 * Input:       data
 *              &imu_dem
 *              Index
 * Output:
 * Return:
*****************************************************/
void Setup_GyroGyron(uint8_t* data, IMU_Dem* imu_dem, uint8_t Index)
{
  char Temp_Buffer[3][30] = {0};
  uint8_t Buffer_P1 = 0, Buffer_P2 = 0;
  uint8_t P_Offset = 21;
  while((data[P_Offset] != '*'))
  {
    if(data[P_Offset] != ',')
      Temp_Buffer[Buffer_P1][Buffer_P2++] = data[P_Offset];
    else
    {
      Buffer_P1++;
      Buffer_P2 = 0;
    }
    P_Offset++;
  }

  switch(Index)
  {
    /****************** GyroX  ******************/
    case 0:
    {
      imu_dem->GYRO_Coupling[0][0] = atof(Temp_Buffer[0]);
      imu_dem->GYRO_Coupling[0][1] = atof(Temp_Buffer[1]);
      imu_dem->GYRO_Coupling[0][2] = atof(Temp_Buffer[2]);
      break;
    }
    /****************** GyroY  ******************/
    case 1:
    {
      imu_dem->GYRO_Coupling[1][0] = atof(Temp_Buffer[0]);
      imu_dem->GYRO_Coupling[1][1] = atof(Temp_Buffer[1]);
      imu_dem->GYRO_Coupling[1][2] = atof(Temp_Buffer[2]);
      break;
    }
    /****************** GyroZ  ******************/
    case 2:
    {
      imu_dem->GYRO_Coupling[2][0] = atof(Temp_Buffer[0]);
      imu_dem->GYRO_Coupling[2][1] = atof(Temp_Buffer[1]);
      imu_dem->GYRO_Coupling[2][2] = atof(Temp_Buffer[2]);
      break;
    }
    default:
    {
      Out_Command_Bad();
      return;
    }
  }
  Out_Config_OK();
}

/****************************************************
 * Function:    Setup_AccScale
 * Description: Setup Acc Scale.
 * Input:       data
 *              &imu_dem
 * Output:
 * Return:
*****************************************************/
void Setup_AccScale(uint8_t* data, IMU_Dem* imu_dem)
{
  char Temp_Buffer[3][30] = {0};
  uint8_t Buffer_P1 = 0, Buffer_P2 = 0;
  uint8_t P_Offset = 18;
  while((data[P_Offset] != '*'))
  {
    if(data[P_Offset] != ',')
      Temp_Buffer[Buffer_P1][Buffer_P2++] = data[P_Offset];
    else
    {
      Buffer_P1++;
      Buffer_P2 = 0;
    }
    P_Offset++;
  }

  imu_dem->ACC_Scale[0] = atof(Temp_Buffer[0]);
  imu_dem->ACC_Scale[1] = atof(Temp_Buffer[1]);
  imu_dem->ACC_Scale[2] = atof(Temp_Buffer[2]);
  
  Out_Config_OK();
}

/****************************************************
 * Function:    Setup_AccZero
 * Description: Setup Acc Zero.
 * Input:       data
 *              &imu_dem
 * Output:
 * Return:
*****************************************************/
void Setup_AccZero(uint8_t* data, IMU_Dem* imu_dem)
{
  char Temp_Buffer[3][30] = {0};
  uint8_t Buffer_P1 = 0, Buffer_P2 = 0;
  uint8_t P_Offset = 18;
  while((data[P_Offset] != '*'))
  {
    if(data[P_Offset] != ',')
      Temp_Buffer[Buffer_P1][Buffer_P2++] = data[P_Offset];
    else
    {
      Buffer_P1++;
      Buffer_P2 = 0;
    }
    P_Offset++;
  }

  imu_dem->ACC_Zero[0] = atof(Temp_Buffer[0]);
  imu_dem->ACC_Zero[1] = atof(Temp_Buffer[1]);
  imu_dem->ACC_Zero[2] = atof(Temp_Buffer[2]);
  
  Out_Config_OK();
}

/****************************************************
 * Function:    Setup_AccAccn
 * Description: Setup Acc Cross-coupling.
 * Input:       data
 *              &imu_dem
 *              Index
 * Output:
 * Return:
*****************************************************/
void Setup_AccAccn(uint8_t* data, IMU_Dem* imu_dem, uint8_t Index)
{
  char Temp_Buffer[3][30] = {0};
  uint8_t Buffer_P1 = 0, Buffer_P2 = 0;
  uint8_t P_Offset = 20;
  while((data[P_Offset] != '*'))
  {
    if(data[P_Offset] != ',')
      Temp_Buffer[Buffer_P1][Buffer_P2++] = data[P_Offset];
    else
    {
      Buffer_P1++;
      Buffer_P2 = 0;
    }
    P_Offset++;
  }

  switch(Index)
  {
    case 0:
    {
      imu_dem->ACC_Coupling[0][0] = atof(Temp_Buffer[0]);
      imu_dem->ACC_Coupling[0][1] = atof(Temp_Buffer[1]);
      imu_dem->ACC_Coupling[0][2] = atof(Temp_Buffer[2]);
      break;
    }
    case 1:
    {
      imu_dem->ACC_Coupling[1][0] = atof(Temp_Buffer[0]);
      imu_dem->ACC_Coupling[1][1] = atof(Temp_Buffer[1]);
      imu_dem->ACC_Coupling[1][2] = atof(Temp_Buffer[2]);
      break;
    }
    case 2:
    {
      imu_dem->ACC_Coupling[2][0] = atof(Temp_Buffer[0]);
      imu_dem->ACC_Coupling[2][1] = atof(Temp_Buffer[1]);
      imu_dem->ACC_Coupling[2][2] = atof(Temp_Buffer[2]);
      break;
    }
    default:
    {
      Out_Command_Bad();
      return;
    }
  }
  Out_Config_OK();
}

/****************************************************
 * Function:    Setup_Mode
 * Description: Setup Mode.
 * Input:       &data
                &mode
 * Output:
 * Return:
*****************************************************/
void Setup_Mode(uint8_t* data, uint32_t* mode)
{
  if(!memcmp("$cmd,set,mode,nav", data, 17))
  {
    Out_Config_OK();
    *mode = 0;
  }
  else if(!memcmp("$cmd,set,mode,imubd", data, 19))
  {
    Out_Config_OK();
    *mode = 1;
  }
  else
    Out_Command_Bad();
}

/****************************************************
 * Function:    Setup_BMI055_Range
 * Description: Setup BMI055 Range.
 * Input:       &data
                &range
 * Output:
 * Return:
*****************************************************/
void Setup_BMI055_Range(uint8_t* data, uint32_t* range)
{
  if(!memcmp("$cmd,set,bmi055,range,125dps", data, 28))
  {
    Out_Config_OK();
    *range = GYRO_125dps;
  }
  else if(!memcmp("$cmd,set,bmi055,range,250dps", data, 28))
  {
    Out_Config_OK();
    *range = GYRO_250dps;
  }
  else if(!memcmp("$cmd,set,bmi055,range,500dps", data, 28))
  {
    Out_Config_OK();
    *range = GYRO_500dps;
  }
  else if(!memcmp("$cmd,set,bmi055,range,1000dps", data, 29))
  {
    Out_Config_OK();
    *range = GYRO_1000dps;
  }
  else if(!memcmp("$cmd,set,bmi055,range,2000dps", data, 29))
  {
    Out_Config_OK();
    *range = GYRO_2000dps;
  }
  else
    Out_Command_Bad();
}

/****************************************************
 * Function:    Setup_BMI055_BW
 * Description: Setup BMI055 Filter Bandwidth.
 * Input:       &data
                &bw
 * Output:
 * Return:
*****************************************************/
void Setup_BMI055_BW(uint8_t* data, uint32_t* bw)
{
  if(!memcmp("$cmd,set,bmi055,bw,12Hz", data, 23))
  {
    Out_Config_OK();
    *bw = GYRO_BW_12Hz;
  }
  else if(!memcmp("$cmd,set,bmi055,bw,32Hz", data, 23))
  {
    Out_Config_OK();
    *bw = GYRO_BW_32Hz;
  }
  else if(!memcmp("$cmd,set,bmi055,bw,23Hz", data, 23))
  {
    Out_Config_OK();
    *bw = GYRO_BW_23Hz;
  }
  else if(!memcmp("$cmd,set,bmi055,bw,64Hz", data, 23))
  {
    Out_Config_OK();
    *bw = GYRO_BW_64Hz;
  }
  else if(!memcmp("$cmd,set,bmi055,bw,47Hz", data, 23))
  {
    Out_Config_OK();
    *bw = GYRO_BW_47Hz;
  }
  else if(!memcmp("$cmd,set,bmi055,bw,116Hz", data, 24))
  {
    Out_Config_OK();
    *bw = GYRO_BW_116Hz;
  }
  else if(!memcmp("$cmd,set,bmi055,bw,230Hz", data, 24))
  {
    Out_Config_OK();
    *bw = GYRO_BW_230Hz;
  }
  else if(!memcmp("$cmd,set,bmi055,bw,523Hz", data, 24))
  {
    Out_Config_OK();
    *bw = GYRO_BW_523Hz;
  }
  else
    Out_Command_Bad();
}

/****************************************************
 * Function:    Setup_ID
 * Description: Setup Product ID.
 * Input:       id
 * Output:
 * Return:
*****************************************************/
void Setup_ID(uint8_t* data, uint32_t* id)
{
  uint8_t Temp_Buffer[4] = {0};
  uint8_t Buffer_P = 0;
  uint8_t P_Offset = 12;

  while((data[P_Offset] != '*'))
  {
    Temp_Buffer[Buffer_P++] = data[P_Offset++];
  }

  *id = ((uint32_t)Temp_Buffer[0])<<24;
  *id |= ((uint32_t)Temp_Buffer[1])<<16;
  *id |= ((uint32_t)Temp_Buffer[2])<<8;
  *id |= (uint32_t)Temp_Buffer[3];

  Out_Config_OK();
}

