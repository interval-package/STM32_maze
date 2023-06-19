
#include "delay.h"

/*******************************************************************************
* Function Name  : delay_us
* Description    : 微秒延时函数，延时nus
* Input          : nus:微秒延时的个数
* Output         : None
* Return         : None
* Note			 : 400MHz下us延时函数
*******************************************************************************/
void delay_us(uint32_t nus)    
{ 
	uint32_t i,j; 
	
	for(i = 0; i < nus; i++)
	{ 
		for( j = 0; j < 140; j++);
	}
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : 毫秒延时函数，延时nms
* Input          : nms:毫秒延时的个数
* Output         : None
* Return         : None
* Note			 : 400MHz下ms延时函数
*******************************************************************************/
void delay_ms(uint32_t ms)    
{ 
	uint32_t i; 
	
	for(i = 0; i < ms; i++)
	{ 
		delay_us(1000);
	}
}

/*******************************************************************************
* Function Name  : delay_ns
* Description    : ns级延时函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : 168MHz下ns延时函数
*******************************************************************************/
void delay_ns(void)
{
    uint32_t j; 
	
	for(j = 0; j < 2; j++);	
}

