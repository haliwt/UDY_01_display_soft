/*
 * bsp_smg.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#include "bsp.h"

#define TM1639_CHAR_H               0x76    // H: 0111 0110 (b,c,e,f,g)


static const uint8_t Number_Table[] = {
    0xF3, // 0: 0011 1111   （f,e,d,c,b,a）--0x3F
    0x60, // 1: 0000 0110 --0x06--写数据式冲低位开始，向高位开始写
    0xB5, // 2: 0101 1011 --0x5B
    0xF4, // 3: 0100 1111 --0x4F
    0x66, // 4: 0110 0110
    0xD6, // 5: 0110 1101 --0x6D
    0xD7, // 6: 0111 1101  --0x7D 
    0x70, // 7: 0000 0111
    0xF7, // 8: 0111 1111
    0xF6, // 9: 0110 1111
    0x00, // 10: 0000 0000
};


// 字母和特殊字符显示码
//static const uint8_t TM1639_Char_Table[] = {
//    0x67, // H: 0111 0110 (b,c,e,f,g)
//    0x36, // °: 0110 0011 (b,c,g)
//    0x93, // C: 0011 1001 (a,d,e,f)
//    0x05  // RH的H部分: 0101 0000 (e,g)
//};


// 字母和特殊字符显示码
static const uint8_t TM1639_Char_Err_Table[] = {
    0x97, // E: 0111 1001 (b,c,e,f,g)
    0x05, // r: 0101 0000 (b,c,g)
    
  
};




/**
 * @brief  显示0-24小时时间
 * @param  None
 * @retval None
 * @note   前两位显示小时(00-24)，最后一位显示'H'
 */
/**
 * @brief  display 4bit timer timing value 数字
 * @param  num: 要显示
 * @retval None
 */
void TM1639_Display_4Bit_Time(uint8_t hours,uint8_t minutes)
{
  
   // 1. 提前把 4 位数码管对应的数字拆分好（清晰直观，方便打断点调试）
    uint8_t hr_dec = hours / 10;
    uint8_t hr_uni = hours % 10;
    uint8_t min_dec = minutes / 10;
    uint8_t min_uni = minutes % 10;


    //hours decade 入十位（最左边）
    
	TM1639_Write_Digit_Full(TM1639_ADDR_DIG5_H, TM1639_ADDR_DIG5_L, Number_Table[hr_dec]);

   
        
    //hours unit 
    if(gpro_t.g_time_disp_colon_flag ==1){ //time colon symbol blink 
    	 TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, Number_Table[hr_uni] | TM1639_DOT);
    }
	else{
         TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, Number_Table[hr_uni]);

	}

	//minute decade入十位（中间）
  
	  if(gpro_t.g_time_disp_colon_flag ==1){ //time colon symbol blink 
	      TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L,Number_Table[min_dec] | TM1639_DOT);
	  }
	  else{
          TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L,Number_Table[min_dec]);
	  }
	
    //minute uint
	TM1639_Write_Digit_Full(TM1639_ADDR_DIG8_H, TM1639_ADDR_DIG8_L,Number_Table[min_uni]);
}

void TM1639_donotDisplay_4Bit_Time(void)
{

   //hours decade 入十位（最左边）
   
	TM1639_Write_Digit_Full(TM1639_ADDR_DIG5_H, TM1639_ADDR_DIG5_L, Number_Table[0x0A]);

   
        
  
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, Number_Table[0x0A]);

	


    TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L,Number_Table[0x0A]);
	
	
  
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG8_H, TM1639_ADDR_DIG8_L,Number_Table[0x0A]);


}


/******************************************************************************
	*
	*Function Name:void SMG_Display_Err(uint8_t idata)
	*Funcion: 
	*Input Ref: idata: 1 -ptc warning  2 - fan warning
	*Return Ref:
	*
******************************************************************************/
/**
 * @brief  display 4bit timer timing value 数字
 * @param  num: 要显示
 * @retval None
 */
void TM1639_Display_4Bit_Error(uint8_t idata)
{
  
   
	  TM1639_Write_Digit_Full(TM1639_ADDR_DIG5_H, TM1639_ADDR_DIG5_L, TM1639_Char_Err_Table[0]);//"E"
	   
	   // 写入十位（中间）
	   if(gpro_t.g_time_disp_colon_flag==1){ 
	      TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, TM1639_Char_Err_Table[1] | TM1639_DOT);//"r"
	   }
	   else{
		TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, TM1639_Char_Err_Table[1] );//"r"

	   }


	   if(gpro_t.g_time_disp_colon_flag==1){ 
	    TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L,Number_Table[0]|TM1639_DOT);//"0"
	   }
	   else{
	       TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L,Number_Table[0]);//"0"


	   }
	   

	   if(idata==1){ //ptc warning "Er:01"
	   // 写入个位（最右边
		TM1639_Write_Digit_Full(TM1639_ADDR_DIG8_H, TM1639_ADDR_DIG8_L,Number_Table[1]);

	   }
	   else if(idata == 2){ //fan warning "Er:02"
	   
		 TM1639_Write_Digit_Full(TM1639_ADDR_DIG8_H, TM1639_ADDR_DIG8_L,Number_Table[2]);
	   }
	   
}

/**
 * @brief  Display 4-bit temperature value
 * @param  temp: Temperature value to display (30-50)
 * @retval None
 */
void TM1639_Display_4Bit_Temp(uint8_t temp)
{
    // Display temperature value (XX°C)
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG5_H, TM1639_ADDR_DIG5_L, Number_Table[temp / 10]);
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, Number_Table[temp % 10]);
    
    // Display 'C' for Celsius
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L, 0x93); // 'C' character
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG8_H, TM1639_ADDR_DIG8_L, 0x00); // Empty
}



/********************************************************************************
	*
	*Functin Name: void Display_TimingValue_TimeColon_Handler(void)
	*Function : 
	*Input Ref:  NO
	*Return Ref: NO
	*
********************************************************************************/
void Display_TimingValue_TimeColon_Handler(void)
{

      if(gpro_t.set_timer_timing_doing_value == 1 || gpro_t.key_add_dec_pressed_flag == 1) return ;

	  if(run_t.gTimer_time_colon >0 ){ //200 //10*20ms=300ms

		   run_t.gTimer_time_colon =0;
	       
		 gpro_t.g_time_disp_colon_flag = gpro_t.g_time_disp_colon_flag ^ 0x01;
	  
	     Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);
	  
	 }
 }




