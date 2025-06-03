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
  
    //hours decade 入十位（最左边）
    
	TM1639_Write_Digit_Full(TM1639_ADDR_DIG5_H, TM1639_ADDR_DIG5_L, Number_Table[hours/10]);

   
        
    //hours unit 
    if(gpro_t.g_time_disp_colon_flag ==1){ //time colon symbol blink 
    	 TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, Number_Table[hours %10] | TM1639_DOT);
    }
	else{
         TM1639_Write_Digit_Full(TM1639_ADDR_DIG6_H, TM1639_ADDR_DIG6_L, Number_Table[hours %10]);

	}

	//minute decade入十位（中间）
  
	  if(gpro_t.g_time_disp_colon_flag ==1){ //time colon symbol blink 
	      TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L,Number_Table[minutes/10] | TM1639_DOT);
	  }
	  else{
          TM1639_Write_Digit_Full(TM1639_ADDR_DIG7_H, TM1639_ADDR_DIG7_L,Number_Table[minutes/10]);
	  }
	
    //minute uint
	TM1639_Write_Digit_Full(TM1639_ADDR_DIG8_H, TM1639_ADDR_DIG8_L,Number_Table[minutes%10]);
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



/********************************************************************************
	*
	*Functin Name: void Display_TimeColon_Blink_Fun(void)
	*Function : 
	*Input Ref:  NO
	*Return Ref: NO
	*
********************************************************************************/
void Display_TimeColon_Blink_Fun(void)
{

  if(run_t.gTimer_time_colon >0 && (gpro_t.set_timer_timing_doing_value==0 ||gpro_t.set_timer_timing_doing_value==3)){ //200 //10*20ms=300ms

	   run_t.gTimer_time_colon =0;
       
	 	  gpro_t.g_time_disp_colon_flag = gpro_t.g_time_disp_colon_flag ^ 0x01;
  
	}
}

/*************************************************************************
*
*Function Name:void SmgBlink_Colon_Function(uint8_t twobit,uint8_t threebit,uint8_t sel)
*Function :display smg of colon ":" blink 
*Input Ref:
*
*
*************************************************************************/




