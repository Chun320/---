#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "sram.h"
#include "usart.h"
#include "malloc.h" 
#include "GUI.h"
#include "includes.h"
#include "touch.h"
#include "DIALOG.h"
#include "EmWinHZFont.h"
#include "sdio_sdcard.h"
#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"
#include "fontupd.h"
#include "EmWinHZFont.h"
#include "WM.h"
#include "mytext.h"
#include "laserdata.h"


float cos_tab[91]={/*cos(0��)*/1,				 /*cos(1��)*/0.999848,  /*cos(2��)*/0.999391,  /*cos(3��)*/0.99863,   /*cos(4��)*/0.997564,
									 /*cos(5��)*/0.996195,  /*cos(6��)*/0.994522,  /*cos(7��)*/0.992546,  /*cos(8��)*/0.990268,  /*cos(9��)*/0.987688,
									 /*cos(10��)*/0.984808, /*cos(11��)*/0.981627, /*cos(12��)*/0.978148, /*cos(13��)*/0.97437,  /*cos(14��)*/0.970296,
									 /*cos(15��)*/0.965926, /*cos(16��)*/0.961262, /*cos(17��)*/0.956305, /*cos(18��)*/0.951057, /*cos(19��)*/0.945519,
									 /*cos(20��)*/0.939693, /*cos(21��)*/0.93358,  /*cos(22��)*/0.927184, /*cos(23��)*/0.920505, /*cos(24��)*/0.913545,
									 /*cos(25��)*/0.906308, /*cos(26��)*/0.898794, /*cos(27��)*/0.891007, /*cos(28��)*/0.882948, /*cos(29��)*/0.87462,
									 /*cos(30��)*/0.866025, /*cos(31��)*/0.857167, /*cos(32��)*/0.848048, /*cos(33��)*/0.838671, /*cos(34��)*/0.829038,
									 /*cos(35��)*/0.819152, /*cos(36��)*/0.809017, /*cos(37��)*/0.798636, /*cos(38��)*/0.788011, /*cos(39��)*/0.777146,
									 /*cos(40��)*/0.766044, /*cos(41��)*/0.75471,  /*cos(42��)*/0.743145, /*cos(43��)*/0.731354, /*cos(44��)*/0.71934,
									 /*cos(45��)*/0.707107, /*cos(46��)*/0.694658, /*cos(47��)*/0.681998, /*cos(48��)*/0.669131, /*cos(49��)*/0.656059,
									 /*cos(50��)*/0.642788, /*cos(51��)*/0.62932,  /*cos(52��)*/0.615661, /*cos(53��)*/0.601815, /*cos(54��)*/0.587785,
									 /*cos(55��)*/0.573576, /*cos(56��)*/0.559193, /*cos(57��)*/0.544639, /*cos(58��)*/0.529919, /*cos(59��)*/0.515038,
									 /*cos(60��)*/0.5,      /*cos(61��)*/0.48481,  /*cos(62��)*/0.469472, /*cos(63��)*/0.45399,  /*cos(64��)*/0.438371,
									 /*cos(65��)*/0.422618, /*cos(66��)*/0.406737, /*cos(67��)*/0.390731, /*cos(68��)*/0.374607, /*cos(69��)*/0.358368,
									 /*cos(70��)*/0.34202,  /*cos(71��)*/0.325568, /*cos(72��)*/0.309017, /*cos(73��)*/0.292372, /*cos(74��)*/0.275637,
								 	 /*cos(75��)*/0.258819, /*cos(76��)*/0.241922, /*cos(77��)*/0.224951, /*cos(78��)*/0.207912, /*cos(79��)*/0.190809,
									 /*cos(80��)*/0.173648, /*cos(81��)*/0.156434, /*cos(82��)*/0.139173, /*cos(83��)*/0.121869, /*cos(84��)*/0.104528,
									 /*cos(85��)*/0.087156, /*cos(86��)*/0.069756, /*cos(87��)*/0.052336, /*cos(88��)*/0.034899, /*cos(89��)*/0.017452,
									 /*cos(90��)*/0};

/*************************************************
 UCOSIII���������ȼ��û�������ʹ��
 ����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
 ���ȼ�0���жϷ������������� OS_IntQTask()
 ���ȼ�1��ʱ�ӽ������� OS_TickTask()
 ���ȼ�2����ʱ���� OS_TmrTask()
 ���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
 ���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
 *************************************************/
//��ʼ����
//�������ȼ�
#define START_TASK_PRIO     3
//�����ջ��С
#define START_STK_SIZE      1024
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//TOUCH����
//�����������ȼ�
#define TOUCH_TASK_PRIO     4
//�����ջ��С
#define TOUCH_STK_SIZE      128
//������ƿ�
OS_TCB  TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//������
void touch_task(void *p_arg);

//LED0����
//�������ȼ�
#define LED0_TASK_PRIO     5
//�����ջ��С
#define LED0_STK_SIZE      128
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *p_arg);

//DMWINDEMO����
//�������ȼ�
#define EMWINDEMO_TASK_PRIO         6
//�����ջ��С
#define EMWINDEMO_STK_SIZE 					2048
//������ƿ�
OS_TCB EmwindemoTaskTCB;
//�����ջ
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//������
void emwindemo_task(void *p_arg);

//�ֿ��������
//�����������ȼ�
#define FONTUPDATA_TASK_PRIO		7
//�����ջ��С
#define FONTUPDATA_STK_SIZE			128
//������ƿ�
OS_TCB FontupdataTaskTCB;
//�����ջ
CPU_STK FONTUPDATA_TASK_STK[FONTUPDATA_STK_SIZE];
//�ֿ��������
void fontupdata_task(void *p_arg);

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	HAL_Init();                   	//��ʼ��HAL��    
	Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);               	//��ʼ����ʱ����
	uart_init(115200);             	//��ʼ��USART
	LED_Init();						          //��ʼ��LED	
	KEY_Init();						          //��ʼ��KEY
	TFTLCD_Init();                  //��ʼ��LCD
	W25QXX_Init();								  //��ʼ��W25Q128
	SRAM_Init();					          //��ʼ���ⲿSRAM  
	my_mem_init(SRAMIN);			      //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);			      //��ʼ���ⲿ�ڴ��
	
	exfuns_init();									//Ϊfatfs�ļ�ϵͳ�����ڴ�
	f_mount(fs[0],"0:",1);					//����SD��
	f_mount(fs[1],"1:",1);					//����FLASH
	while(font_init())							//��ʼ���ֿ�
	{
		LCD_ShowString(30,70,200,16,16,"Font Error!");
		while(SD_Init())							//���SD��
		{
			LCD_ShowString(30,90,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,90,200+30,70+16,WHITE);
			delay_ms(200);		    
		}
		update_font(30,90,16,"0:");	//����ֿⲻ���ھ͸����ֿ�
		delay_ms(2000);
		LCD_Clear(WHITE);	//����
		break;
	}
	TP_Init();                      //��������ʼ��
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
	//������ʼ����
	OSTaskCreate((OS_TCB*      ) &StartTaskTCB,      //������ƿ�
							 (CPU_CHAR*    ) "start_task",       //��������
							 (OS_TASK_PTR  ) start_task,         //������
							 (void*        ) 0,                  //��������Ĳ���
							 (OS_PRIO      ) START_TASK_PRIO,    //�������ȼ�
							 (CPU_STK*     ) &START_TASK_STK[0], //�����ջ����ַ
							 (CPU_STK_SIZE ) START_STK_SIZE/10,  //�����ջ�����λ
							 (CPU_STK_SIZE ) START_STK_SIZE,     //�����ջ��С
							 (OS_MSG_QTY   ) 0, //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK      ) 0, //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ���
							 (void*        ) 0, //�û�����Ĵ洢��
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //�û�����Ĵ洢��
							 (OS_ERR*      ) &err);              //��Ÿú�������ʱ�ķ���ֵ
	
	OS_CRITICAL_EXIT();
	OSStart(&err);
 	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
	
	#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif

	__HAL_RCC_CRC_CLK_ENABLE();
	//WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();              //STemWin��ʼ��
	OS_CRITICAL_ENTER();     //�����ٽ���
	//����STemWin_Demo����
	OSTaskCreate((OS_TCB*      ) &EmwindemoTaskTCB,      //������ƿ�
							 (CPU_CHAR*    ) "Emwmindemo_task",       //��������
							 (OS_TASK_PTR  ) emwindemo_task,         //������
							 (void*        ) 0,                  //��������Ĳ���
							 (OS_PRIO      ) EMWINDEMO_TASK_PRIO,    //�������ȼ�
							 (CPU_STK*     ) &EMWINDEMO_TASK_STK[0], //�����ջ����ַ
							 (CPU_STK_SIZE ) EMWINDEMO_STK_SIZE/10,  //�����ջ�����λ
							 (CPU_STK_SIZE ) EMWINDEMO_STK_SIZE,     //�����ջ��С
							 (OS_MSG_QTY   ) 0, //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK      ) 0, //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ���
							 (void*        ) 0, //�û�����Ĵ洢��
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //�û�����Ĵ洢��
							 (OS_ERR*      ) &err); 
	
//��������������							 
	OSTaskCreate((OS_TCB*      ) &TouchTaskTCB,      //������ƿ�
							 (CPU_CHAR*    ) "touch_task",       //��������
							 (OS_TASK_PTR  ) touch_task,         //������
							 (void*        ) 0,                  //��������Ĳ���
							 (OS_PRIO      ) TOUCH_TASK_PRIO,    //�������ȼ�
							 (CPU_STK*     ) &TOUCH_TASK_STK[0], //�����ջ����ַ
							 (CPU_STK_SIZE ) TOUCH_STK_SIZE/10,  //�����ջ�����λ
							 (CPU_STK_SIZE ) TOUCH_STK_SIZE,     //�����ջ��С
							 (OS_MSG_QTY   ) 0, //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK      ) 0, //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ���
							 (void*        ) 0, //�û�����Ĵ洢��
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //�û�����Ĵ洢��
							 (OS_ERR*      ) &err); 

//LED0��˸����							 
	OSTaskCreate((OS_TCB*      ) &Led0TaskTCB,       //������ƿ�
							 (CPU_CHAR*    ) "led0_task",        //��������
							 (OS_TASK_PTR  ) led0_task,          //������
							 (void*        ) 0,                  //��������Ĳ���
							 (OS_PRIO      ) LED0_TASK_PRIO,     //�������ȼ�
							 (CPU_STK*     ) &LED0_TASK_STK[0],  //�����ջ����ַ
							 (CPU_STK_SIZE ) LED0_STK_SIZE/10,   //�����ջ�����λ
							 (CPU_STK_SIZE ) LED0_STK_SIZE,      //�����ջ��С
							 (OS_MSG_QTY   ) 0, //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
							 (OS_TICK      ) 0, //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ���
							 (void*        ) 0, //�û�����Ĵ洢��
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //�û�����Ĵ洢��
							 (OS_ERR*      ) &err);

//�ֿ��������
	OSTaskCreate((OS_TCB*     )&FontupdataTaskTCB,		
								 (CPU_CHAR*   )"Fontupdata task", 		
                 (OS_TASK_PTR )fontupdata_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )FONTUPDATA_TASK_PRIO,     
                 (CPU_STK*    )&FONTUPDATA_TASK_STK[0],	
                 (CPU_STK_SIZE)FONTUPDATA_STK_SIZE/10,	
                 (CPU_STK_SIZE)FONTUPDATA_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);								 
							 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB, &err);
	OS_CRITICAL_EXIT();      //�˳��ٽ���
}

//STemWin������
void emwindemo_task(void *p_arg)
{	
	float offset=0;  //��òο������ݴ洢�����㴹ֱ����ƫ������
	u8 S_val[4];           //����ת�����̱���
	float val=0, level_val=0; //��þ������ݴ洢�����㴹ֱ�������ݴ洢
	
	//����Ƥ��
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	
	stm32_show();
	
	while(1)
	{
		GUI_Delay(20);//��ʱ
		if(USART2_RX_STA&0x80)
		{
			//�жϲ����Ƿ񳬳�
			if((USART2_RX_BUF[3]>=0x7F) && (USART2_RX_BUF[4]==0xFF))
			{
				EDIT_SetFont(hID_EDIT_0, &GUI_FontHZ24);
				EDIT_SetText(hID_EDIT_0, "�������̣�");
			}
			else
			{
				//ת���õ�����Ч���ݣ���ʮ�����ƻ�Ϊʮ��������
				S_val[0] = USART2_RX_BUF[3]>>4;
				val = S_val[0]*16*16*16;
				S_val[1] = USART2_RX_BUF[3]&0x0F;
				val += S_val[1]*16*16;
				S_val[2] = USART2_RX_BUF[4]>>4;
				val += S_val[2]*16;
				S_val[3] = USART2_RX_BUF[4]&0x0F;
				val += S_val[3];
				//printf("%f\r\n", num);
				//�������Ӧ�Ĳ�������
				if(val <= 32768)
				{
					//FSD11-30���������ݼ���
					if(FSD11_flag == 0)
						val = 30+((val *1)/1000);
					//FSD11-50���������ݼ���
					else if(FSD11_flag == 1)
						val = 50+((val *2)/1000);
				}
				else
				{
					//FSD11-30���������ݼ���
					if(FSD11_flag == 0)
						val = 30+(((val-65535)*1)/1000);
					//FSD11-50���������ݼ���
					else if(FSD11_flag == 1)
						val = 50+(((val-65535)*2)/1000);
				}
				//�������Ǻ����������������������Ĵ�ֱ����
				level_val = val*cos_tab[angle_val];
				if(level_flag == 0)
					offset = level_val;
				level_val -= offset;
				
				//����Ӧ����ֵת��Ϊ�ַ���
				sprintf(str_val, "%.2f", val);
				sprintf(level_str_val, "%.2f", level_val);
				
				//�����ı���Ķ�Ӧ�ı�
				EDIT_SetFont(hID_EDIT_0, GUI_FONT_32_ASCII);
				EDIT_SetText(hID_EDIT_0, level_str_val);
			}
			
			USART2_RX_STA = 0;
		}
		//����3���ճ����
		if(USART3_RX_STA&0x80)
		{
//			//��ȡ�����ַ�����������60���ַ���
//			u8 b = USART3_RX_STA&0x3f;
//			//�����յ����ַ�ͨ������3���ͳ�ȥ
//			HAL_UART_Transmit(&UART3_Handler, (u8 *)USART3_RX_BUF, b, 1000);
//			//���ͻ��лس���
//			Uart_Send_Str(&UART3_Handler, "\r\n");
			//���������ɱ�־λ
			USART3_RX_STA = 0;
		}
	}
}

//�ֿ��������
//KEY_UP������2s�����ֿ�
void fontupdata_task(void *pdata)
{
	OS_ERR err;
	while(1)
	{
		if(WK_UP == 1)				//KEY_UP������
		{
			OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);//��ʱ2s
			if(WK_UP == 1)			//����KEY_UP��
			{
				LCD_Clear(WHITE);
				OSSchedLock(&err);		//����������
				LCD_ShowString(10,50,250,30,16,"Font Updataing,Please Wait...");
				update_font(10,70,16,"0:");//�����ֿ�
				LCD_Clear(WHITE);
				POINT_COLOR = RED;
				LCD_ShowString(10,50,280,30,16,"Font Updata finshed,Please Restart!");
				OSSchedUnlock(&err);	//����������
			}
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);//��ʱ10ms
	}
}


//������������
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_PERIODIC,&err);
	}
}

//LED0������
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
	}
}




