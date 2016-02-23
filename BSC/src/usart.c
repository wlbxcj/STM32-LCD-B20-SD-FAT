#include"usart.h"
//#include <stdarg.h>
/********************************************************

//函数名称：USART_Configuration(void)
//功能描述：对串口1进行初始化
//输入参数：无
//返回：无

//说明：初始化分为两大块，即串口所使用I/O口的初始化和串口功能的初始化。
//对于USART串口要用的引脚，根据其数据方向，要设置其为GPIO_M_IN_FLOATING浮空输入
//或GPIO_Mode_AF_PP复用推挽输出，其他的和GPIO引脚设置一样。
********************************************************/
void USART_Configuration(void)
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//使能串口1时钟		 高速时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能串口2时钟		 低速时钟
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//使能串口3时钟		 低速时钟
	//配置USART1 RX （PA.10）为浮空输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//配置USART1 RX （PA.9）为复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//******************************************************************************
	// 串口1参数初始化定义部分,串口1参数为38400 ， 8 ，1 ，N  接收中断方式
	//******************************************************************************
	USART_InitStructure.USART_BaudRate=115200;						//设定传输速率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//传输数据位数
    USART_InitStructure.USART_StopBits = USART_StopBits_1; 			//设定停止位个数	
    USART_InitStructure.USART_Parity = USART_Parity_No; 			//不用检验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
																	//不用流量控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使用发送和接收端口
	USART_Init(USART1,&USART_InitStructure);						//初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  				//使能串口1接收中断
	USART_Cmd(USART1,ENABLE);										//使能串口1
}

/*****************************************************
//函数名称：void NVIC_Configuration(void)
//功能描述：中断配置	
//输入参数：无
//返回：无
*****************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);				
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	   //先占优先级1位，从优先级3位
	//使能串口1中断
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*****************************************************
//函数名称：Uart1_PutChar
//功能描述：串口1发送单字节函数
//输入参数：unsigned char ch	要发送的数据
//返回：无
*****************************************************/
void Usart1_PutChar(u8 ch)
{	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET); 	//等待数据发送完毕
}

/*****************************************************
//函数名称：Uart1_PutChar
//功能描述：串口1发送单字节函数
//输入参数：unsigned char ch	要发送的数据
//返回：无
*****************************************************/
void Usart1_PutStr(u8 *p, int ulLen)
{	
	while(ulLen--)
	{
		Usart1_PutChar(*p);
		p++;
	}
}


