/***************************** (C) COPYRIGHT ************************************
* File Name          : stm32_system.h
* Author             : 星光嵌入式
* Version            : V1.0
* Date               : 27/11/2019
* Description        : STM32H743时钟配置驱动程序源文件
* Note               : 版权所有  严禁盗版
********************************************************************************
* 实验平台: 星光猛禽STM32H743VIT6开发板 
* 淘宝店铺: https://shop148702745.taobao.com/
* 技术论坛: http://www.xg123-bbs.com/
* QQ群:542830257
********************************************************************************/
#ifndef __STM_SYSTEM_H_
#define __STM_SYSTEM_H_

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32_types.h"

#define FOSC  25     //MCU时钟晶振25MHz

/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : NVIC中断配置
* Input          : IRQn:中断号
				   PriorityGroup：优先级分组
				   priority：优先级
* Output         : None
* Return         : None
* Note			 : 调用函数定义在core_cm7.h中, 注意所有中断的优先级分组是一样的
				   优先级分组时针对所有的中断的,此函数是针对外设中断的，不包括
				   系统中断,SysTick
*******************************************************************************/
void NVIC_Config(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t priority);

/*******************************************************************************
* Function Name  : Sys_Soft_Reset
* Description    : 系统软复位 
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Sys_Soft_Reset(void);

/*******************************************************************************
* Function Name  : System_Init
* Description    : 系统初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void System_Init(void);

#endif 
