#include "config.h"
#include "Car.h"
/*ע�⣺ϵͳ����STM32F103VET6CPU��ΪSTM32F10X_HD�������������ļ�Ҳ��ҪΪHD*/

/* �Զ���ͬ��ؼ���    --------------------------------------------------------*/
static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

/*******************************************************************************
* ������	: RCC_Configuration
* ��������  : ��ϵͳ������ʱ��
* �������  : ��
* ������  : ��
* ����ֵ    : ��
*******************************************************************************/
void RCC_Configuration(void)
{
		/* ����ö�����ͱ��� HSEStartUpStatus */
  	ErrorStatus HSEStartUpStatus;
  	/* ��λϵͳʱ������ */
  	RCC_DeInit();
  	/* ���� HSE */
  	RCC_HSEConfig(RCC_HSE_ON);
  	/* �ȴ� HSE �����ȶ� */
  	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	  /* �ж� HSE ���Ƿ���ɹ����������if()�ڲ� */
  	if(HSEStartUpStatus == SUCCESS)
  	{
    	/* ѡ�� HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ */
    	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    	
    	/* ѡ�� PCLK1 ʱ��ԴΪ HCLK��AHB��2��Ƶ */
    	RCC_PCLK1Config(RCC_HCLK_Div2);
			/* ѡ�� PCLK2 ʱ��ԴΪ HCLK��AHB��1��Ƶ */
    	RCC_PCLK2Config(RCC_HCLK_Div1); 
			
    	/* ���� FLASH ��ʱ������Ϊ2 */
    	FLASH_SetLatency(FLASH_Latency_2);
    	/* ʹ�� FLASH Ԥȡ���� */
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    	/* ѡ�����໷��PLL��ʱ��ԴΪ HSE 1��Ƶ����Ƶ��Ϊ9����PLL���Ƶ��Ϊ 8MHz * 9 = 72MHz */
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    	/* ʹ�� PLL */ 
    	RCC_PLLCmd(ENABLE);
    	/* �ȴ� PLL ����ȶ� */
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    	/* ѡ�� SYSCLK ʱ��ԴΪ PLL */
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    	/* �ȴ� PLL ��Ϊ SYSCLK ʱ��Դ */
    	while(RCC_GetSYSCLKSource() != 0x08);
  	} 
		/* ��APB1�����ϵĸ���ʱ��*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |
													 RCC_APB1Periph_UART4  | RCC_APB1Periph_UART5  |
		                       RCC_APB1Periph_TIM2   |RCC_APB1Periph_TIM3    |
		                       RCC_APB1Periph_TIM4   | RCC_APB1Periph_TIM5,   
		                       ENABLE);
  	/* ��APB2�����ϵĸ���ʱ��*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_GPIOB  |
		                       RCC_APB2Periph_GPIOC  | RCC_APB2Periph_GPIOD  | 
		                       RCC_APB2Periph_GPIOE  | RCC_APB2Periph_USART1 |
													 RCC_APB2Periph_AFIO   ,ENABLE);
}

/*******************************************************************************
* ������  		: GPIO_Configuration
* ��������    	: ���ø�GPIO�˿ڹ���
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;	
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		  //JTAG,SWD

		/********************PA������******************/
		/* ����PA��Ӧ��Ϊ������������תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |
																	GPIO_Pin_7 | GPIO_Pin_15 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
  	/* ����PA��Ӧ��Ϊ�������룬���תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | 
																	 GPIO_Pin_8 | GPIO_Pin_9 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
		/********************PB������******************/
  	/* ����PB��Ӧ��Ϊ��©��������תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);
		
		/* ����PB��Ӧ��Ϊ������������תƵ��Ϊ50MHz*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |
																	GPIO_Pin_8 | GPIO_Pin_9 |
																	GPIO_Pin_12 | GPIO_Pin_13 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);
		
		/* ����PB��Ӧ��Ϊ�������룬���תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;																
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);	

		/********************PC������******************/
		/* ����PC��Ӧ��Ϊ��©��������תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 ;																	
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);
		/* ����PC��Ӧ��Ϊ�������룬���תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_6 | 
																		GPIO_Pin_7 | GPIO_Pin_8 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);
		/* ����PC��Ӧ��Ϊ������������תƵ��Ϊ50MHz*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);	
	
		/********************PD������******************/
		/* ����PD��Ӧ��Ϊ������������תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0   |  GPIO_Pin_1 | GPIO_Pin_3  |
																	GPIO_Pin_4 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOD , &GPIO_InitStructure);
		
		/* ����PD��Ӧ��Ϊ��©��������תƵ��Ϊ50MHz*/		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  | GPIO_Pin_9  |
																	GPIO_Pin_10  ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOD , &GPIO_InitStructure);
		
		
		/* ����PD��Ӧ��Ϊ�������룬���תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |
											 GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
											 GPIO_Pin_15 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOD , &GPIO_InitStructure);		
		
		/********************PE������******************/
		/* ����PE��Ӧ��Ϊ��©��������תƵ��Ϊ50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8  | 	GPIO_Pin_9  |GPIO_Pin_10  |
																	GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_11|		GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOE , &GPIO_InitStructure);
		/*GPIOE Configuration: TIM1 channel 1 and 2 */   
//		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOE, &GPIO_InitStructure);
		
		/********************NAV IO��������******************/
		//NAV0 PE5 .		//NAV1 PE6.		//NAV2 PE4 .		//NAV3 PC13. 	//NAV4 PE3 .
		//NAV5 PC14.		//NAV6 PE2 .		//NAV7 PC15.		//NAV8 PE1.		//NAV9 PD5.
		//NAV10 PE0.		//NAV11 PD6.		//NAV12 PB5.		//NAV13 PD7.		//NAV14 PB4.		//NAV15 PB3.
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |	GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5 |GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &GPIO_InitStructure);		
		
		/********************���ڽӿ�����******************/
		/* ����PA9,PA10(����1)TxΪ�ڶ������������ģʽ Rx��Ϊ���������*/
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
	
		/* ����PA2,PA3(����2)TxΪ�ڶ������������ģʽ Rx��Ϊ���������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
		
		/* ����PB10,PB11(����3)TxΪ�ڶ������������ģʽ Rx��Ϊ���������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB , &GPIO_InitStructure);   
		
		/* ����PC10,PC11(����4)TxΪ�ڶ������������ģʽ Rx��Ϊ���������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC , &GPIO_InitStructure);  
		
		/* ����PC12,PD2(����5)TxΪ�ڶ������������ģʽ Rx��Ϊ���������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD , &GPIO_InitStructure); 		
}
void IWDG_Configuration(void)
{
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ��Ԥ��Ƶ�Ĵ�������װ�ؼĴ���
		IWDG_SetPrescaler(IWDG_Prescaler_256);//Ԥ��Ƶֵ 40K/256=156��һ����156��
		IWDG_SetReload(500);//��װ��ֵ������500�Ρ�
		IWDG_ReloadCounter();	//ι��
		IWDG_Enable();//ʹ��
		IWDG_GetFlagStatus(IWDG_FLAG_PVU);//�õ���־λ��״̬���Ƿ�λ
}
/******************************************************************************
* ������  		: Systick_Configuration
* ��������    	: ����Systick��ʱ��,��װ��ʱ��Ϊ1s
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************/
void Systick_Configuration(void)
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=72/8;		    
	fac_ms=(u16)fac_us*1000;		
}
//��ʱnus
//usΪҪ��ʱ��us��.		    								   
void Delay_us(u16 us)
{		
	u32 temp;	    	 
	SysTick->LOAD=us*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do	
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
/******************************************************************************
* ������  		: Delay_ms
* ��������    	: n ms��ʱ
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************/
void Delay_ms(u16 ms)
{
	u32 temp;		   
	SysTick->LOAD=(u32)ms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  
}


void Delay_s(u8 s)
{
	u8 tt;
	for(tt=0;tt<s;tt++)
		Delay_ms(1000);
}

void Init_Device(void)
{
	RCC_Configuration();
	GPIO_Configuration();
//	IWDG_Configuration();
	Systick_Configuration();
	EXTI_Config();
	Frequt_Init(65535,17);//18��Ƶ//Ƶ������ڿ��Ը�ΪPWM���������PWM_Init(9000,0);	//����Ƶ��PWMƵ��=72000/900=8Khz
//	nDAC_OUT(0x0F,0);//4ͨ��DAC(0-5V)�����ʼ��Ϊ0
//	TIM2_Config();
//	TIM3_Config();
	TIM5_Config();
//	TIM6_Config(10000);
//	TIM7_Config();
//	USART1_Configuration();
	//USART2_Configuration();
	USART3_Configuration();
	USART5_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	DMA1_Config_Receive2 ();//���ô���2��DMA����	
	DMA1_Config_Send_2 ();  //���ô���2�ķ���
	DMA1_Config_Receive3 ();//���ô���3��DMA����	
	DMA1_Config_Send_3 ();  //���ô���3�ķ���
}
u16 ConvertSpeed(float argspeed)
{
	return (u16)(177/argspeed);
}




