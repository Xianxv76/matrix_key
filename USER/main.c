#include "sys.h"
#include "timer.h"
#include "delay.h"
#include "key.h"

 u32 BB;
	
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	delay_init(168);
  KEY_Init();
	TIM3_Int_Init(); 
  BB=0;
	colrest();
  colall(); 						
  while(1)
		{
//			colall();
//			delay_ms(1000);
//			//colrest();
//			delay_ms(1000);
			//	x = GPIOE->IDR;
			//BB = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10);
			//	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
			//	GPIO_ResetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
	
  	}
}
