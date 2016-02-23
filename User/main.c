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
 *	keil c�������������ַ���ʱ������ʾ
 *	��warning:  #870-D: invalid multibyte character sequence����
 *	���һ��Դ�ļ��г����ϰٸ�warning��������ҳ�һ��error��̫������
 *	����Ҳ����Ϊkeil���IDE���ܶ�errors��warnings���й�����ʾ���£���
 *	��ˣ�������һ������İ취��
 *	��Դ�ļ��м��������ѡ�#pragma  diag_suppress 870���Ϳ��Խ�ֹ�������warning��Ϣ�ˡ�
 *	�����Ҫ��ֹ����warning��ֻҪ����ں��漴�ɣ���#pragma  diag_suppress 870,223,......����ˣ�����͸ɾ��ˡ�
 *	����һ�ֽ������(�����ṩ)��char * sptr; sptr = "����CSDN"; 	 
 */
//#pragma  diag_suppress 870
					
u8 num[10]={'0','1','2','3','4','5','6','7','8','9'};
u8 touch_flag;
u8 aa[4];

FATFS fs[2];            // Work area (file system object) for logical drive
FRESULT res;            // FatFs function common result code
UINT br, bw;            // File R/W count
FIL fsrc, fdst;         // file objects
uint8_t textFileBuffer[] = "��лwlb��^_^ \r\n";

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
	
	disk_initialize( 0 );	     // SD ��Ӳ����ʼ�����ǳ���Ҫ ;	
 	f_mount(0, &fs[0]);			 // Ϊ�߼�������ע�Ṥ����  //  
	// �ڸո�ע��Ĺ������д�һ���ļ�������ļ��������򴴽����ļ�������Ϊ�ɶ���д ���������/		                                           
	res = f_open( &fsrc , "0:/Demo.TXT" , FA_CREATE_NEW|FA_WRITE); 	
	if ( res == FR_OK )	// ��������������д���ļ��� //
  	{ 
	    res = f_write(&fsrc, textFileBuffer, sizeof(textFileBuffer), &bw); 
	  	Usart1_PutStr( "\r\n Demo.TXT �ļ������ɹ� \n", strlen("\r\n Demo.TXT �ļ������ɹ� \n"));
		f_close(&fsrc);	  //�ر��ļ� //			  
    }
  	else if ( res == FR_EXIST )
  	{
		Usart1_PutStr( "\r\n Demo.TXT �Ѿ����� \n", strlen("\r\n Demo.TXT �Ѿ����� \n"));
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
			//��������
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
		EXTI_ClearITPendingBit(EXTI_Line13);	   //���ⲿ�ж�3
		touch_flag=1;
		//LCD_PutString(19,190,"I LOVE MY JOB ",Red,Black);
  }

}

