#include "MyIncludes.h"

u16 sys_cnt = 0;
/*******************************************************************************
* Function Name  : systick_isr
* Description    : SysTick中断回调
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void systick_isr(void)
{
	if(sys_cnt < 1000) sys_cnt++;
	else
	{
		sys_cnt = 0;
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);  //翻转LED
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	}
}

char lcd_id[12];			//存放LCD ID字符串
u16 color;
/*******************************************************************************
* Function Name  : tft_test
* Description    : LCD测试
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void tft_test(u8 dir)
{
	LCD_Scan_Dir(dir);
	
	TFT_Clear(WHITE);
	color = TFT_ReadPoint(10,10);
	delay_ms(500);
	TFT_Clear(BLUE);
	color = TFT_ReadPoint(10,10);
	delay_ms(500);
	TFT_Clear(RED);
	color = TFT_ReadPoint(10,10);
	delay_ms(500);
	TFT_Clear(WHITE);
	color = TFT_ReadPoint(10,10);
	delay_ms(500);
	TFT_Clear(YELLOW);
	color = TFT_ReadPoint(10,10);
	delay_ms(500);
	
	//划线测试(不加粗)
	GUI_Draw_Line(10,10,200,160,NORMAL_LINE,SOLID_LINE,RED);
	GUI_Draw_Line(10,20,200,170,NORMAL_LINE,DOTTED_LINE,RED);
	GUI_Draw_Line(10,30,200,180,NORMAL_LINE,DASHED_LINE,RED);
	//划线测试(加粗)
	GUI_Draw_Line(10,50,200,210,THICK_LINE,SOLID_LINE,RED);
	GUI_Draw_Line(10,60,200,220,THICK_LINE,DOTTED_LINE,RED);
	GUI_Draw_Line(10,70,200,230,THICK_LINE,DASHED_LINE,RED);
	delay_ms(2000);
	TFT_Clear(YELLOW);
	
	GUI_Draw_Rectangle(20,30,100,60,0,RED,0);  //画矩形
	GUI_Draw_Rectangle(130,30,100,60,0,RED,1); //填充矩形
  GUI_Draw_Circle(50,130,20,RED);             //画空心圆
	GUI_Draw_FilledCircle(150,130,20,RED);      //画实心圆
	GUI_Draw_Ellipse(60,200,50,30,RED);         //画空心椭圆
	GUI_Fill_Ellipse(170,200,50,30,RED);        //画实心椭圆
	
	delay_ms(2000);
	TFT_Clear(YELLOW);
	
	sprintf((char*)lcd_id,"LCD ID:%04X",Lcd_Para.id);//将LCD ID打印到lcd_id数组,LCD ID是16进制
	
	TFT_ShowStr(50,60,16,"STM32H7 DEMO ^_^",0,RED,YELLOW);
	TFT_ShowStr(50,80,16,lcd_id,0,RED,YELLOW);
	TFT_ShowStr(50,100,16,"www.feifanembed.com",0,RED,YELLOW);
	TFT_ShowStr(50,120,16,"2019/12/01",0,RED,YELLOW);
	
	delay_ms(1000);
}

int main(void)
{			
	System_Init();    //系统时钟初始化
	
	LED_Init();	
	
	SysTick_Init(systick_isr);
	
	USART1_Init(115200,NULL,NULL);
	
	GUIx_Init();     //LCD初始化

	#if ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 ) //竖屏
	
	maze();

	// if(Lcd_Para.id == 0x1963)   //4.3寸屏默认是横放的,和默认竖放相反
	// {
	// 	tft_test(U2D_L2R);
	// 	tft_test(U2D_R2L);
	// 	tft_test(D2U_R2L);
	// 	tft_test(D2U_L2R);
	// }
	// else
	// {
	// 	tft_test(L2R_U2D);
	// 	tft_test(L2R_D2U);
	// 	tft_test(R2L_U2D);
	// 	tft_test(R2L_D2U);
	// }
	
	#elif( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )  //横屏
	
	if(Lcd_Para.id == 0x1963)  //4.3寸屏默认是横放的,和默认竖放相反
	{
		tft_test(L2R_U2D);
		tft_test(L2R_D2U);
		tft_test(R2L_U2D);
		tft_test(R2L_D2U);
	}
	else
	{
		tft_test(U2D_L2R);
		tft_test(U2D_R2L);
		tft_test(D2U_R2L);
		tft_test(D2U_L2R);
	}
	
	#endif
	
    while(1)
    {	

    }    
}




