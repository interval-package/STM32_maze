/***************************** (C) COPYRIGHT ************************************
* File Name          : delay.h
* Author             : 星光嵌入式
* Version            : V1.0
* Date               : 27/11/2019
* Description        : 延时函数驱动程序源文件，实现us、ms延时
* Note               : 版权所有  严禁盗版
********************************************************************************
* 实验平台: 星光猛禽STM32H743VIT6开发板 
* 淘宝店铺: https://shop148702745.taobao.com/
* 技术论坛: www.feifanembed.com
* QQ群:542830257
********************************************************************************/
#ifndef __DELAY_H_
#define __DELAY_H_ 

#include "stm32h7xx.h"	   
#include "stm32_system.h"	

/*******************************************************************************
* Function Name  : delay_us
* Description    : 微秒延时函数，延时nus
* Input          : nus:微秒延时的个数
* Output         : None
* Return         : None
* Note			 : 400MHz下us延时函数
*******************************************************************************/
void delay_us(uint32_t nus);

/*******************************************************************************
* Function Name  : delay_ms
* Description    : 毫秒延时函数，延时nms
* Input          : nms:毫秒延时的个数
* Output         : None
* Return         : None
* Note			 : 168MHz下ms延时函数
*******************************************************************************/
void delay_ms(uint32_t ms);

/*******************************************************************************
* Function Name  : delay_ns
* Description    : ns级延时函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : 400MHz下ns延时函数
*******************************************************************************/
void delay_ns(void);


#endif /* __DELAY_H_ */

