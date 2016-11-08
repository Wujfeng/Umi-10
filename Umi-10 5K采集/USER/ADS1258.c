#include "ADS1258.h"

ADS1258_data ad_collect_data;
ADS1258_Final_Data MCU_Out_Acc;

/****************************************************************************
 * Function:    ADS1258_ID_check
 * Description: READ ADS1258 ID and check.
 * Input:       void
 * Output:
 * Return:      if ID is right return 1,else return 0
****************************************************************************/
uint8_t ADS1258_ID_check()
{
  uint16_t ID = 0;
  
  ADS1258_CS_LOW;
  
  ID = ADS1258_SendWord( (Register_Read_Command|ID_ADDRESS)<<8 | 0x00 );
  
  if( (ID&0x00ff) == ID_Value )
   return 1;
  else
   return 0;
}

/****************************************************************************
 * Function:    ADS1258 DRDY and START IO Init
 * Description: Init ADS1258 DRDY as input and START as output 
 * Input:       void
 * Output:
 * Return:      void
****************************************************************************/
void AD_DRDY_START_Init(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  /* Enable SYSCFG clock */

  /* Configure DRDY->PB7 pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect EXTI Line2 to DRDY->B7 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);

  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);


  /* Configure START->PB6 pin as output */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  ADS1258_START_LOW(); 
}

/****************************************************************************
 * Function:    ADS1258_Init
 * Description: Init ADS1258 enable channel ,set collect cycle
 * Input:       void
 * Output:
 * Return:      void
****************************************************************************/
void ADS1258_Init(void)
{
  AD_DRDY_START_Init();
  
	ADS1258_CS_LOW;    

  ADS1258_SendWord( ( (Register_Write_Command|CONFIG0_ADDRESS)<<8) | 0x46 );  /*[7:5] is command byte  [3:0] is address byte*/
                                                                              /*reset time: 256clk,status ->enable,CHOP=1*/
  
  ADS1258_SendWord( ( (Register_Write_Command|CONFIG1_ADDRESS)<<8) | 0x32);   /*DLY[2:0]=011 DRATE[1:0]=10 single channel IO_DRDY= 10191/2(CHOP=1) HZ*/                                   
 
  ADS1258_SendWord( ( (Register_Write_Command|MUXSG0_ADDRESS)<<8) | 0x55);    /*AIN0 AIN2 AIN4 AIN6 enable*/
  
  ADS1258_SendWord( ( (Register_Write_Command|MUXSG1_ADDRESS)<<8) | 0x00);    /*disable other channel*/
  
  ADS1258_CS_HIGH;

  ADS1258_START_HIGH();                                                       /*start conversion*/  
}

/****************************************************************************
 * Function:    translate_u24_to_s24
 * Description: translate signed 24bits number into signed 32bites number
 * Input:       signed 24bits number
 * Output:
 * Return:      signed 32bits number
****************************************************************************/
int translate_s24_to_s32(unsigned int data)
{
  int sint = 0;
  
  if((data&0x00800000)!=0)
  {
    sint = (int)(data | 0xff000000);             /* if data is a minus 24 bits number, 
                                                    if data is minus and you want chang it into a signed 32 bytes number
                                                    you should fill up high byte with f. */                                               
  }
  else
  {
    sint = (int)data;
  }
  return sint;
}

/****************************************************************************
 * Function:    ADS1258_data_check_accumulation
 * Description: judge data for which channel and do accumulation.
 * Input:       High_byte and Low_byte
 * Output:      ad_collect_data
 * Return:      void
****************************************************************************/
void ADS1258_data_check_accumulation(unsigned int H_byte,unsigned int L_byte)
{
    switch(H_byte&0x1f00)                /* [4:0] is the byte of chanel */
    {
      case Channel_ACC_X: ad_collect_data.X_32 = translate_s24_to_s32( ( (H_byte&0x00ff) <<16) + L_byte );
                          ad_collect_data.sum_X += ad_collect_data.X_32;
                          ad_collect_data.X_count++;
                          break;
      
      case Channel_ACC_Y: ad_collect_data.Y_32 = translate_s24_to_s32( ( (H_byte&0x00ff) <<16) + L_byte );
                          ad_collect_data.sum_Y += ad_collect_data.Y_32;
                          ad_collect_data.Y_count++;
                          break;
      
      case Channel_ACC_Z: ad_collect_data.Z_32 = translate_s24_to_s32( ( (H_byte&0x00ff) <<16) + L_byte );
                          ad_collect_data.sum_Z += ad_collect_data.Z_32;
                          ad_collect_data.Z_count++;
                          break;
      
      case Channel_TEMP:  ad_collect_data.TEMP_32 =  translate_s24_to_s32( ( (H_byte&0x00ff) <<16) + L_byte );
                          ad_collect_data.sum_TEMP += ad_collect_data.TEMP_32;
                          ad_collect_data.TEMP_count++;
                          break;
      default :  break;
    }   
}

/****************************************************************************
 * Function:    ADS1258_data_calculate
 * Description: calculate the data and clear the sum and count numbers
 * Input:       ad_collect_data and MCU_Out_Acc
 * Output:      MCU_Out_Acc
 * Return:      if data is new return 1,else return 0
****************************************************************************/
unsigned char ADS1258_data_calculate(ADS1258_data* AD_collect,ADS1258_Final_Data* MCU_PUTOUT_Data)
{
  unsigned char data_OK_flag = 1;
  
  if(AD_collect->X_count > 0)
  {
    MCU_PUTOUT_Data->ACC_X_f = ( AD_collect->sum_X / AD_collect->X_count )* Vref*AD_scale;
    
    AD_collect->X_count = 0;
    AD_collect->sum_X = 0;
  }
  else data_OK_flag = 0;
  
  if(AD_collect->Y_count > 0)
  {
    MCU_PUTOUT_Data->ACC_Y_f = ( AD_collect->sum_Y / AD_collect->Y_count )* Vref*AD_scale; 
    
    AD_collect->Y_count = 0;
    AD_collect->sum_Y = 0;
  }
  else data_OK_flag = 0;
  
  if(AD_collect->Z_count > 0)
  {
    MCU_PUTOUT_Data->ACC_Z_f = ( AD_collect->sum_Z / AD_collect->Z_count )* Vref*AD_scale;

    AD_collect->Z_count = 0;
    AD_collect->sum_Z = 0;
  }
  else data_OK_flag = 0;
  
  if(AD_collect->TEMP_count > 0)
  {
    MCU_PUTOUT_Data->TEMP = ( ( AD_collect->sum_TEMP / AD_collect->TEMP_count )* Vref*AD_scale+Vref - 0.5f)*100.0f; /*temperature = (Vout - 0.5)*100*/ 
  
    AD_collect->TEMP_count = 0;
    AD_collect->sum_TEMP = 0;
  }
  else data_OK_flag = 0;
  
  return data_OK_flag;
}

/************************************Polling mode collection example*****************************************************/

unsigned int AdData[8];
void ADS1258_Read(void)
{
  while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
  
  SPI2_CS2_LOW();
  
  AdData[0]= SPI2_SendHalfWord(0x0000);
  AdData[1]= SPI2_SendHalfWord(0x0000);
  
  while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
  
  AdData[2]= SPI2_SendHalfWord(0x0000);
  AdData[3]= SPI2_SendHalfWord(0x0000);
  
  while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
  
  AdData[4]= SPI2_SendHalfWord(0x0000);
  AdData[5]= SPI2_SendHalfWord(0x0000);
  
  while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
  
  AdData[6]= SPI2_SendHalfWord(0x0000);
  AdData[7]= SPI2_SendHalfWord(0x0000);

	SPI2_CS2_HIGH();
}

//void ADS1258_data_simple_caltulate()
//{
//  int i=0;
//  for(i=0;i<20;i+=2)
//  {
//    switch(AdData[i]&0x1f00)                /* [4:0] is the byte of chanel */
//      {
//        case Channel_ACC_X: ad_32_data.ACC_X = translate_u24_to_s24( ( (AdData[i]&0x00ff) <<16) + AdData[i+1] );break;
//        case Channel_ACC_Y: ad_32_data.ACC_Y = translate_u24_to_s24( ( (AdData[i]&0x00ff) <<16) + AdData[i+1] );break;
//        case Channel_ACC_Z: ad_32_data.ACC_z = translate_u24_to_s24( ( (AdData[i]&0x00ff) <<16) + AdData[i+1] );break;
//        case Channel_TEMP:  ad_32_data.TEMP =  translate_u24_to_s24( ( (AdData[i]&0x00ff) <<16) + AdData[i+1] );break;
//        default : break;
//      }     
//  }  
//  
//  ad_data.ACC_X = ad_32_data.ACC_X * Vref*scale;
//  ad_data.ACC_Y = ad_32_data.ACC_Y * Vref*scale;
//  ad_data.ACC_z = ad_32_data.ACC_z * Vref*scale;
//  ad_data.TEMP =(ad_32_data.TEMP  * Vref*scale+2.5f-0.75f)*100.0f+25.0f;
//}



