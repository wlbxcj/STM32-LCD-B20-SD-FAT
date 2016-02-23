#include "stm32f10x_conf.h"
#include "lcd_ssd1289.h"
#include "lcd_touch.h"
#include "usart.h"
#include "18b20.h"
#include "delay.h"
#include "sdio.h"
#include "ff.h"
#include "diskio.h"
#include "stdio.h"
#include <string.h>
#include <math.h>
/*
 *	keil c编译器处理汉字字符串时，会提示
 *	“warning:  #870-D: invalid multibyte character sequence”。
 *	如果一个源文件中出个上百个warning，想从中找出一个error就太困难了
 *	（这也是因为keil这个IDE不能对errors和warnings进行归类显示所致）。
 *	因此，我找了一个解决的办法：
 *	在源文件中加入编译器选项，#pragma  diag_suppress 870，就可以禁止出现这个warning消息了。
 *	如果还要禁止其他warning，只要添加在后面即可，如#pragma  diag_suppress 870,223,......，如此，输出就干净了。
 *	另外一种解决方法(网友提供)：char * sptr; sptr = "我在CSDN"; 	 
 */
//#pragma  diag_suppress 870
					
u8 num[10]={'0','1','2','3','4','5','6','7','8','9'};
u8 touch_flag;
u8 aa[4];

FATFS fs[2];            // Work area (file system object) for logical drive
FRESULT res;            // FatFs function common result code
UINT br, bw;            // File R/W count
FIL fsrc, fdst;         // file objects
uint8_t textFileBuffer[] = "感谢wlb！^_^ \r\n";

int main(void)
{	
	u16 x = 0;
	s16 temp;
	char acTem[10] = {0};
	SystemInit();
	Lcd_GPIO_Config();
	Lcd_FMSC_Config();
	LCD_MyInit(0);

	Touch_Init();
	USART_Configuration();
	SD_NVIC_Configuration();
	
	DS18B20_Init();
	LCD_PutString(19,190,"I LOVE YOU !!!",Red,Black); 
	LCD_DrawRec(19,220,100,50,Red);
	
	disk_initialize( 0 );	     // SD 卡硬件初始化，非常重要 ;	
 	f_mount(0, &fs[0]);			 // 为逻辑驱动器注册工作区  //  
	// 在刚刚注册的工作区中打开一个文件，如果文件不存在则创建该文件，属性为可读可写 ，存在则打开/		                                           
	res = f_open( &fsrc , "0:/Demo.TXT" , FA_CREATE_NEW|FA_WRITE); 	
	if ( res == FR_OK )	// 将缓冲区的数据写到文件中 //
  	{ 
	    res = f_write(&fsrc, textFileBuffer, sizeof(textFileBuffer), &bw); 
	  	Usart1_PutStr( "\r\n Demo.TXT 文件创建成功 \n", strlen("\r\n Demo.TXT 文件创建成功 \n"));
		f_close(&fsrc);	  //关闭文件 //			  
    }
  	else if ( res == FR_EXIST )
  	{
		Usart1_PutStr( "\r\n Demo.TXT 已经存在 \n", strlen("\r\n Demo.TXT 已经存在 \n"));
  	}
	
	while(1)
	{
		LCD_PutString(19,100,"DS18B20  is OK!",Red,White);
		temp=DS18B20_GetTemp();
		if(temp<0)
		{
			temp=-temp;
			LCD_PutString(50,120,"-",Red,White);
		}
		else
			LCD_PutString(50,120," ",Red,White);
		aa[0]='9';
		aa[1]='8';
		aa[2]=num[temp/10];
		aa[3]=num[temp%10];
		
		LCD_PutString(52,120,aa,Red,White);
		//printf("temp is %d \r\n",tempq);
		DelayUs(5500);
		//LCD_PutString(19,100,num[temp%10],Red,White);
		memset(acTem, 0, sizeof(acTem));
		sprintf(acTem, "%d", x++);
		LCD_PutString(50, 140, (u8 *)acTem, Red, White);
		if(touch_flag)
		{	
			//触屏部分
			/*x=Read_X();			
			y=Read_Y();
			aa[0]=num[x/1000];
			aa[1]=num[x%1000/100];
			aa[2]=num[x%100/10];
			aa[3]=num[x%10];
			
			Usart1_PutStr("x:  ");Usart1_PutStr(aa);
			Usart1_PutStr("\r\n");
			LCD_PutString(100,20,aa,Red,Black);
			aa[0]=num[y/1000];
			aa[1]=num[y%1000/100];
			aa[2]=num[y%100/10];
			aa[3]=num[y%10];
			
			LCD_PutString(120,40,aa,Red,Black);
			Usart1_PutStr("y:  ");Usart1_PutStr(aa);
			Usart1_PutStr("\r\n");/
			touch_flag=0;*/	
		} 
			
	}
	 
	//return 0;
}

void EXTI15_10_IRQHandler(void)
{
	
//#if defined (STM32F10X_HD_VL) || defined (STM32F10X_HD) || defined (STM32F10X_XL)
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
		EXTI_ClearITPendingBit(EXTI_Line13);	   //清外部中断3
		touch_flag=1;
		//LCD_PutString(19,190,"I LOVE MY JOB ",Red,Black);
  }

}

