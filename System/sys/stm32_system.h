/***************************** (C) COPYRIGHT ************************************
* File Name          : stm32_system.h
* Author             : �ǹ�Ƕ��ʽ
* Version            : V1.0
* Date               : 27/11/2019
* Description        : STM32H743ʱ��������������Դ�ļ�
* Note               : ��Ȩ����  �Ͻ�����
********************************************************************************
* ʵ��ƽ̨: �ǹ�����STM32H743VIT6������ 
* �Ա�����: https://shop148702745.taobao.com/
* ������̳: http://www.xg123-bbs.com/
* QQȺ:542830257
********************************************************************************/
#ifndef __STM_SYSTEM_H_
#define __STM_SYSTEM_H_

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32_types.h"

#define FOSC  25     //MCUʱ�Ӿ���25MHz

/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : NVIC�ж�����
* Input          : IRQn:�жϺ�
				   PriorityGroup�����ȼ�����
				   priority�����ȼ�
* Output         : None
* Return         : None
* Note			 : ���ú���������core_cm7.h��, ע�������жϵ����ȼ�������һ����
				   ���ȼ�����ʱ������е��жϵ�,�˺�������������жϵģ�������
				   ϵͳ�ж�,SysTick
*******************************************************************************/
void NVIC_Config(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t priority);

/*******************************************************************************
* Function Name  : Sys_Soft_Reset
* Description    : ϵͳ��λ 
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Sys_Soft_Reset(void);

/*******************************************************************************
* Function Name  : System_Init
* Description    : ϵͳ��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void System_Init(void);

#endif 
