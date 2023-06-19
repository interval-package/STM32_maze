
#include "stm32_system.h"

/*******************************************************************************
* Function Name  : SystemClock_Init
* Description    : STM32H7时钟配置,系统时钟400MHz
* Input          : None
* Output         : None
* Return         : None
* Note			 : Fvco=Fs*(plln/pllm),VCO时钟频率
				   Fsys=Fvco/pllp, MCU系统时钟频率,这里为400MHz
				   Fusb=Fvco/pllq, USB,SDIO,RNG时钟频率,必须为48MHz
				   PLLN:主PLL倍频系数(PLL倍频),取值范围:4~512
				   PLLM:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63
				   PLLP:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2~128.(且必须是2的倍数)
				   PLLQ:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:1~128
				   Fs:晶振输入(4-26MHz)(HSE)
*******************************************************************************/
void SystemClock_Init(uint8_t Fosc)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/*!< Supply configuration update enable */
	MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

	/* The voltage scaling allows optimizing the power consumption when the device is 
	 clocked below the maximum system frequency, to update the voltage scaling value 
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	if(Fosc == 25)   //25MHz
	{
		RCC_OscInitStruct.PLL.PLLM = 5;
		RCC_OscInitStruct.PLL.PLLN = 160;
	}
	else if(Fosc == 8)  //8MHz
	{
		RCC_OscInitStruct.PLL.PLLM = 2;
		RCC_OscInitStruct.PLL.PLLN = 200;
	}
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLQ = 4;

	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* Select PLL as system clock source and configure  bus clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1
	| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}
	
	/*activate CSI clock mondatory for I/O Compensation Cell*/  
	__HAL_RCC_CSI_ENABLE() ;

	/* Enable SYSCFG clock mondatory for I/O Compensation Cell */
	__HAL_RCC_SYSCFG_CLK_ENABLE() ;

	/* Enables the I/O Compensation Cell */    
	HAL_EnableCompensationCell(); 
}

/*******************************************************************************
* Function Name  : MPU_Config
* Description    : Configure the MPU attributes as Write Through for SRAM1/2
* Input          : None
* Output         : None
* Return         : None
* Note			 : The Base Address is 0x20010000 since this memory interface is the AXI.
         	       The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
*******************************************************************************/
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU attributes as WT for SRAM */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress      = 0x24000000;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes as WT for SDRAM */
//	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//	MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
//	MPU_InitStruct.Size             = MPU_REGION_SIZE_32MB;
//	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
//	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE; 
//	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
//	MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
//	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
//	MPU_InitStruct.SubRegionDisable = 0x00;
//	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;

//	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes as WT for QSPI */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress      = 0x90000000;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes as WT for QSPI */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress      = 0x90000000;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_128MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER3;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/*******************************************************************************
* Function Name  : CPU_CACHE_Enable
* Description    : STM32F7的L1-Cache使能
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

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
void NVIC_Config(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t priority)
{
	NVIC_SetPriorityGrouping(PriorityGroup); //设置中断分组
	NVIC_SetPriority(IRQn, priority);	     //设置中断优先级
	NVIC_EnableIRQ(IRQn);	                 //使能中断
}
  
/*******************************************************************************
* Function Name  : Sys_Soft_Reset
* Description    : 系统软复位 
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Sys_Soft_Reset(void)
{   
//	SCB->AIRCR =0X05FA0000|(uint32_t)0x04;	
	NVIC_SystemReset();
}

/*******************************************************************************
* Function Name  : System_Init
* Description    : 系统初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void System_Init(void)
{	
	/* Configure the MPU attributes as Write Through */
	MPU_Config();
	
	/* Enable the CPU Cache */
	CPU_CACHE_Enable();
	
	HAL_Init();             //其中配置了SYSTICK,时间为1ms,库函数要用到,这里,我们会重新定义
	SystemClock_Init(FOSC);
}

