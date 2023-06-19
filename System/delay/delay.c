
#include "delay.h"

/*******************************************************************************
* Function Name  : delay_us
* Description    : ΢����ʱ��������ʱnus
* Input          : nus:΢����ʱ�ĸ���
* Output         : None
* Return         : None
* Note			 : 400MHz��us��ʱ����
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
* Description    : ������ʱ��������ʱnms
* Input          : nms:������ʱ�ĸ���
* Output         : None
* Return         : None
* Note			 : 400MHz��ms��ʱ����
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
* Description    : ns����ʱ����
* Input          : None
* Output         : None
* Return         : None
* Note			 : 168MHz��ns��ʱ����
*******************************************************************************/
void delay_ns(void)
{
    uint32_t j; 
	
	for(j = 0; j < 2; j++);	
}

