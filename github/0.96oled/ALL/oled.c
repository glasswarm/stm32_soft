#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "stm32f1xx_hal.h"
//#include <stdio.h>
//OLED???
//------size = 8 ???????.-----
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	
//------size = 16 ???????.-----
//[0]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//#define display(row, col, size, ...) {sprintf((char*)txt, __VA_ARGS__); OLED_ShowString(col*8, row*(size==16?2:1), txt, size);}	   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack()
{
	//GPIOB->CRH &= 0XFFF0FFFF;	//??PB12???????
	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
/*	while(1)
	{
		if(!OLED_SDA)				//???????OLED ????
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//??PB12?????????
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
		m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);
	}
	else 
	{
		Write_IIC_Command(dat);
	}
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//????
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//??OLED??    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC??
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//??OLED??     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC??
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//????,???,????????!??????!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //?????(0~7)
		OLED_WR_Byte (0x00,OLED_CMD);      //??????—????
		OLED_WR_Byte (0x10,OLED_CMD);      //??????—????   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //????
}
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //?????(0~7)
		OLED_WR_Byte (0x00,OLED_CMD);      //??????—????
		OLED_WR_Byte (0x10,OLED_CMD);      //??????—????   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //????
}
//???????????,??????
//x:0~127
//y:0~7		 
//size:???? 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//???????			
	if(x>Max_Column-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else 
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
	}
}
//m^n??
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//??2???
//x,y :????	 
//len :?????
//size:????
//mode:??	0,????;1,????
//num:??(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//????????
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120){x=0;y+=2;}
		j++;
	}
}
//????
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
}
/***********????:????BMP??128×64?????(x,y),x???0~127,y?????0~7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;

	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{      
			OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
		}
	}
} 

//???SSD1306					    
void OLED_Init(void)
{ 	
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  
// 	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6|GPIO_PIN_7,GPIO_PIN_SET);
	
	
	HAL_Delay(800);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}


// void oled_display_test(void)
// {
// 	unsigned char txt[16];
// 	unsigned char i;
// 	while(1)
// 	{
//   		OLED_Clear();
// 		OLED_ShowString(0, 0, "test:y=0,size=8", 8);
// 		OLED_ShowString(0, 1, "test:y=1,size=8", 8);
// 		OLED_ShowString(0, 2, "test:y=2,size=8", 8);
// 		OLED_ShowString(0, 3, "test:y=3,size=8", 8);
// 		OLED_ShowString(0, 4, "test:y=4,size=8", 8);
// 		OLED_ShowString(0, 5, "test:y=5,size=8", 8);
// 		OLED_ShowString(0, 6, "test:y=6,size=8", 8);
// 		OLED_ShowString(0, 7, "test:y=7,size=8", 8);
// 		HAL_Delay(1000);
//   		OLED_Clear();
// 		OLED_ShowString(0, 0, "test y=0,size=16", 16);
// 		OLED_ShowString(0, 2, "test y=2,size=16", 16);
// 		OLED_ShowString(0, 4, "test y=4,size=16", 16);
// 		OLED_ShowString(0, 6, "test y=6,size=16", 16);
// 		HAL_Delay(1000);
//   		OLED_Clear();
// #ifndef _OLED_DISPLAY
// #define _OLED_DISPLAY
// #define display(row, col, size, ...) {sprintf((char*)txt, __VA_ARGS__); OLED_ShowString(col*8, row*(size==16?2:1), txt, size);}
// #endif
// 		display(0, 0, 8, "0123");
// 		display(0, 4, 8, "4567");
// 		display(0, 8, 8, "89012345");
// 		display(1, 0, 8, "1");
// 		display(2, 0, 8, "2");
// 		display(3, 0, 8, "3");
// 		display(4, 0, 8, "4");
// 		display(5, 0, 8, "5");
// 		display(6, 0, 8, "6");
// 		display(7, 0, 8, "7");
// 		display(1, 2, 16,"display define");
// 		display(2, 3, 16,"--OLED TEST--");
// 		display(3, 3, 16,"test time: %d", ++i);
// 		HAL_Delay(1000);
// 		OLED_Clear();
// 		display(0, 0, 16, "0123456789012345");
// 		display(1, 0, 16, "1");
// 		display(2, 0, 16, "2");
// 		display(3, 0, 16, "3");
// 		display(2, 3, 16,"--OLED TEST--");
// 		display(3, 3, 16,"test time: %d", ++i);
// 		HAL_Delay(1000);
// 	}
// }

