#include "sys.h"
#include "timer.h"
#include "key.h"

u32 i[16]={ 0,0,0,0,
	          0,0,0,0,
	          0,0,0,0,
	          0,0,0,0,
          } ;
u8 key_state = 0;

u32 a[3]={0,0,0},c[3]={0,0,0},d[3]={0,0,0};
u32 i_10ms=0;

 u32 x;
	
//  i为电平高低   a判断是否触发了3次  

void TIM3_Int_Init(void) 
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);    //使能相应的时钟
   
    TIM_TimeBaseInitStructure.TIM_Period =99;                 //10ms           
    TIM_TimeBaseInitStructure.TIM_Prescaler=8399;  
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //分频

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 

    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;     //中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; 
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;      //使能中断
    NVIC_Init(&NVIC_InitStructure);

    TIM_SetCounter(TIM3,0);      //定时器3清零
    TIM_Cmd(TIM3, ENABLE); 
}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
    {   
			 button();   				
    }
   TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
}


	void button(void)
	{


	switch(key_state) 
			{
				case 0:
/////////////  等待得到高电平 //////////////					
									colall();
									x = GPIOC->IDR >> 10 & 0x0f ;  //读取234 口电平
									if(x!=0x0F)
									{
										d[0]++;
										i_10ms++;
										if(i_10ms >= 2){
											key_state=1;						//连续两次高电平
											i_10ms=0;
										}
									}
								 break ;
					
			  case 1: 
					    
 //////////////    扫描   /////////////////////
/////////////     第一排  /////////////////////
									col1();
									x = GPIOC->IDR >> 10 & 0x0f ;
									switch (x)	        
											{
											case 0x0B : i[0]++; key_state=2;
												break;   //第一列
											case 0x0D : i[1]++; key_state=2; 
												break;	  //第二列
											case 0x0E : i[2]++; key_state=2; 
												break;
											default :  break;
											}	
											
								if(key_state==2)   //确定为第一排  跳出后续扫描
									break;
								
		//////////////////  第二排	//////////////////				
									col2();	
									x = GPIOC->IDR >> 10 & 0x0f ;	
									switch (x)	
											{
											case 0x0B : i[3]++; key_state=2;
												break;   //第一列
											case 0x0D : i[4]++; key_state=2; 
												break;	  //第二列
											case 0x0E : i[5]++; key_state=2; 
												break;
											default :  break;
											}	
									
							 if(key_state==2)    //确定为第二排  跳出扫描
									break;

		///////////////  第三排   ////////////////////					 
									col3();
									x = GPIOC->IDR >> 10 & 0x0f ;
									switch (x)	
											{
											case 0x0B : i[6]++; key_state=2;
												break;   //第一列
											case 0x0D : i[7]++; key_state=2; 
												break;	  //第二列
											case 0x0E : i[8]++; key_state=2; 
												break;
											default : key_state=2; break;
											}	
										break;
									
				case 2: 
 ///////////   等待得到低电平 复位   ///////////

									 x = GPIOC->IDR >> 10 & 0x0f ;
			 
									 if(x==0x0F)
									 {
										 i_10ms++;
										 if(i_10ms>=2) {
											 key_state=0;
											 i_10ms=0;
										 }	 
									 }break;
          }					
				}




		void col1(void)
		{
				COLUMN0=0;
				COLUMN1=1;
				COLUMN2=1;
		}

		void col2(void)
		{
				COLUMN0=1;
				COLUMN1=0;
				COLUMN2=1;
		}
    void col3(void)
		{
				COLUMN0=1;
				COLUMN1=1;
				COLUMN2=0;
		}
		
    void colrest(void)
		{
				COLUMN0=1;
				COLUMN1=1;
				COLUMN2=1;
		}
    void colall(void)
		{
				COLUMN0=0;
				COLUMN1=0;
				COLUMN2=0;
		}



			
//			
//			i[0]=ROW0;
//			i[1]=ROW1;
//			i[2]=ROW2;
//			
//			////////       第一行  ////////////
//					colrest();   //复位
//					col1();      //1口置高
//			
//					if(c[0]==0)       //未触发时  检测高电平
//										 if (i[0]==1)
//										 {
//														 a[0]++;
//														 if(a[0]>=2)
//															 {
//																a[0]=0;
//																row[0]++;   // 确定行触发
//																c[0]=1;
////																colrest();
////																col1();
////																 if (i[0]==1)
////																	 column[0]++;
////																 
//														 
//															 }
//																 
//																 
//															 
//										 }				 
//						 
//					if(c[0]==1)        //  触发过一次 防误触 检测低电平
//										if(i[0]==0)	
//										{
//														a[0]++;
//														if(a[0]>=3)
//															{
//																c[0]=0;
//																a[0]=0;
//															}							
//										}

