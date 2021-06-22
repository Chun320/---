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


float cos_tab[91]={/*cos(0°)*/1,				 /*cos(1°)*/0.999848,  /*cos(2°)*/0.999391,  /*cos(3°)*/0.99863,   /*cos(4°)*/0.997564,
									 /*cos(5°)*/0.996195,  /*cos(6°)*/0.994522,  /*cos(7°)*/0.992546,  /*cos(8°)*/0.990268,  /*cos(9°)*/0.987688,
									 /*cos(10°)*/0.984808, /*cos(11°)*/0.981627, /*cos(12°)*/0.978148, /*cos(13°)*/0.97437,  /*cos(14°)*/0.970296,
									 /*cos(15°)*/0.965926, /*cos(16°)*/0.961262, /*cos(17°)*/0.956305, /*cos(18°)*/0.951057, /*cos(19°)*/0.945519,
									 /*cos(20°)*/0.939693, /*cos(21°)*/0.93358,  /*cos(22°)*/0.927184, /*cos(23°)*/0.920505, /*cos(24°)*/0.913545,
									 /*cos(25°)*/0.906308, /*cos(26°)*/0.898794, /*cos(27°)*/0.891007, /*cos(28°)*/0.882948, /*cos(29°)*/0.87462,
									 /*cos(30°)*/0.866025, /*cos(31°)*/0.857167, /*cos(32°)*/0.848048, /*cos(33°)*/0.838671, /*cos(34°)*/0.829038,
									 /*cos(35°)*/0.819152, /*cos(36°)*/0.809017, /*cos(37°)*/0.798636, /*cos(38°)*/0.788011, /*cos(39°)*/0.777146,
									 /*cos(40°)*/0.766044, /*cos(41°)*/0.75471,  /*cos(42°)*/0.743145, /*cos(43°)*/0.731354, /*cos(44°)*/0.71934,
									 /*cos(45°)*/0.707107, /*cos(46°)*/0.694658, /*cos(47°)*/0.681998, /*cos(48°)*/0.669131, /*cos(49°)*/0.656059,
									 /*cos(50°)*/0.642788, /*cos(51°)*/0.62932,  /*cos(52°)*/0.615661, /*cos(53°)*/0.601815, /*cos(54°)*/0.587785,
									 /*cos(55°)*/0.573576, /*cos(56°)*/0.559193, /*cos(57°)*/0.544639, /*cos(58°)*/0.529919, /*cos(59°)*/0.515038,
									 /*cos(60°)*/0.5,      /*cos(61°)*/0.48481,  /*cos(62°)*/0.469472, /*cos(63°)*/0.45399,  /*cos(64°)*/0.438371,
									 /*cos(65°)*/0.422618, /*cos(66°)*/0.406737, /*cos(67°)*/0.390731, /*cos(68°)*/0.374607, /*cos(69°)*/0.358368,
									 /*cos(70°)*/0.34202,  /*cos(71°)*/0.325568, /*cos(72°)*/0.309017, /*cos(73°)*/0.292372, /*cos(74°)*/0.275637,
								 	 /*cos(75°)*/0.258819, /*cos(76°)*/0.241922, /*cos(77°)*/0.224951, /*cos(78°)*/0.207912, /*cos(79°)*/0.190809,
									 /*cos(80°)*/0.173648, /*cos(81°)*/0.156434, /*cos(82°)*/0.139173, /*cos(83°)*/0.121869, /*cos(84°)*/0.104528,
									 /*cos(85°)*/0.087156, /*cos(86°)*/0.069756, /*cos(87°)*/0.052336, /*cos(88°)*/0.034899, /*cos(89°)*/0.017452,
									 /*cos(90°)*/0};

/*************************************************
 UCOSIII中以下优先级用户程序不能使用
 将这些优先级分配给了UCOSIII的5个系统内部任务
 优先级0：中断服务服务管理任务 OS_IntQTask()
 优先级1：时钟节拍任务 OS_TickTask()
 优先级2：定时任务 OS_TmrTask()
 优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
 优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
 *************************************************/
//开始任务
//任务优先级
#define START_TASK_PRIO     3
//任务堆栈大小
#define START_STK_SIZE      1024
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO     4
//任务堆栈大小
#define TOUCH_STK_SIZE      128
//任务控制块
OS_TCB  TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//任务函数
void touch_task(void *p_arg);

//LED0任务
//任务优先级
#define LED0_TASK_PRIO     5
//任务堆栈大小
#define LED0_STK_SIZE      128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *p_arg);

//DMWINDEMO任务
//任务优先级
#define EMWINDEMO_TASK_PRIO         6
//任务堆栈大小
#define EMWINDEMO_STK_SIZE 					2048
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//任务函数
void emwindemo_task(void *p_arg);

//字库更新任务
//设置任务优先级
#define FONTUPDATA_TASK_PRIO		7
//任务堆栈大小
#define FONTUPDATA_STK_SIZE			128
//任务控制块
OS_TCB FontupdataTaskTCB;
//任务堆栈
CPU_STK FONTUPDATA_TASK_STK[FONTUPDATA_STK_SIZE];
//字库更新任务
void fontupdata_task(void *p_arg);

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	HAL_Init();                   	//初始化HAL库    
	Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART
	LED_Init();						          //初始化LED	
	KEY_Init();						          //初始化KEY
	TFTLCD_Init();                  //初始化LCD
	W25QXX_Init();								  //初始化W25Q128
	SRAM_Init();					          //初始化外部SRAM  
	my_mem_init(SRAMIN);			      //初始化内部内存池
	my_mem_init(SRAMEX);			      //初始化外部内存池
	
	exfuns_init();									//为fatfs文件系统分配内存
	f_mount(fs[0],"0:",1);					//挂载SD卡
	f_mount(fs[1],"1:",1);					//挂载FLASH
	while(font_init())							//初始化字库
	{
		LCD_ShowString(30,70,200,16,16,"Font Error!");
		while(SD_Init())							//检测SD卡
		{
			LCD_ShowString(30,90,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,90,200+30,70+16,WHITE);
			delay_ms(200);		    
		}
		update_font(30,90,16,"0:");	//如果字库不存在就更新字库
		delay_ms(2000);
		LCD_Clear(WHITE);	//清屏
		break;
	}
	TP_Init();                      //触摸屏初始化
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
	//创建开始任务
	OSTaskCreate((OS_TCB*      ) &StartTaskTCB,      //任务控制块
							 (CPU_CHAR*    ) "start_task",       //任务名称
							 (OS_TASK_PTR  ) start_task,         //任务函数
							 (void*        ) 0,                  //传递任务的参数
							 (OS_PRIO      ) START_TASK_PRIO,    //任务优先级
							 (CPU_STK*     ) &START_TASK_STK[0], //任务堆栈基地址
							 (CPU_STK_SIZE ) START_STK_SIZE/10,  //任务堆栈深度限位
							 (CPU_STK_SIZE ) START_STK_SIZE,     //任务堆栈大小
							 (OS_MSG_QTY   ) 0, //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK      ) 0, //当使能时间片轮转时的时间片长度，为0时为默认长度
							 (void*        ) 0, //用户补充的存储区
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //用户补充的存储区
							 (OS_ERR*      ) &err);              //存放该函数错误时的返回值
	
	OS_CRITICAL_EXIT();
	OSStart(&err);
 	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
	
	#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif

	__HAL_RCC_CRC_CLK_ENABLE();
	//WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();              //STemWin初始化
	OS_CRITICAL_ENTER();     //进入临界区
	//创建STemWin_Demo任务
	OSTaskCreate((OS_TCB*      ) &EmwindemoTaskTCB,      //任务控制块
							 (CPU_CHAR*    ) "Emwmindemo_task",       //任务名称
							 (OS_TASK_PTR  ) emwindemo_task,         //任务函数
							 (void*        ) 0,                  //传递任务的参数
							 (OS_PRIO      ) EMWINDEMO_TASK_PRIO,    //任务优先级
							 (CPU_STK*     ) &EMWINDEMO_TASK_STK[0], //任务堆栈基地址
							 (CPU_STK_SIZE ) EMWINDEMO_STK_SIZE/10,  //任务堆栈深度限位
							 (CPU_STK_SIZE ) EMWINDEMO_STK_SIZE,     //任务堆栈大小
							 (OS_MSG_QTY   ) 0, //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK      ) 0, //当使能时间片轮转时的时间片长度，为0时为默认长度
							 (void*        ) 0, //用户补充的存储区
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //用户补充的存储区
							 (OS_ERR*      ) &err); 
	
//创建触摸屏任务							 
	OSTaskCreate((OS_TCB*      ) &TouchTaskTCB,      //任务控制块
							 (CPU_CHAR*    ) "touch_task",       //任务名称
							 (OS_TASK_PTR  ) touch_task,         //任务函数
							 (void*        ) 0,                  //传递任务的参数
							 (OS_PRIO      ) TOUCH_TASK_PRIO,    //任务优先级
							 (CPU_STK*     ) &TOUCH_TASK_STK[0], //任务堆栈基地址
							 (CPU_STK_SIZE ) TOUCH_STK_SIZE/10,  //任务堆栈深度限位
							 (CPU_STK_SIZE ) TOUCH_STK_SIZE,     //任务堆栈大小
							 (OS_MSG_QTY   ) 0, //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK      ) 0, //当使能时间片轮转时的时间片长度，为0时为默认长度
							 (void*        ) 0, //用户补充的存储区
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //用户补充的存储区
							 (OS_ERR*      ) &err); 

//LED0闪烁任务							 
	OSTaskCreate((OS_TCB*      ) &Led0TaskTCB,       //任务控制块
							 (CPU_CHAR*    ) "led0_task",        //任务名称
							 (OS_TASK_PTR  ) led0_task,          //任务函数
							 (void*        ) 0,                  //传递任务的参数
							 (OS_PRIO      ) LED0_TASK_PRIO,     //任务优先级
							 (CPU_STK*     ) &LED0_TASK_STK[0],  //任务堆栈基地址
							 (CPU_STK_SIZE ) LED0_STK_SIZE/10,   //任务堆栈深度限位
							 (CPU_STK_SIZE ) LED0_STK_SIZE,      //任务堆栈大小
							 (OS_MSG_QTY   ) 0, //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
							 (OS_TICK      ) 0, //当使能时间片轮转时的时间片长度，为0时为默认长度
							 (void*        ) 0, //用户补充的存储区
							 (OS_OPT       ) OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //用户补充的存储区
							 (OS_ERR*      ) &err);

//字库更新任务
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
	OS_CRITICAL_EXIT();      //退出临界区
}

//STemWin任务函数
void emwindemo_task(void *p_arg)
{	
	float offset=0;  //测得参考点数据存储（计算垂直距离偏移量）
	u8 S_val[4];           //进制转换过程变量
	float val=0, level_val=0; //测得距离数据存储，计算垂直距离数据存储
	
	//更换皮肤
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
		GUI_Delay(20);//延时
		if(USART2_RX_STA&0x80)
		{
			//判断测量是否超程
			if((USART2_RX_BUF[3]>=0x7F) && (USART2_RX_BUF[4]==0xFF))
			{
				EDIT_SetFont(hID_EDIT_0, &GUI_FontHZ24);
				EDIT_SetText(hID_EDIT_0, "测量超程！");
			}
			else
			{
				//转换得到的有效数据，将十六进制换为十进制数据
				S_val[0] = USART2_RX_BUF[3]>>4;
				val = S_val[0]*16*16*16;
				S_val[1] = USART2_RX_BUF[3]&0x0F;
				val += S_val[1]*16*16;
				S_val[2] = USART2_RX_BUF[4]>>4;
				val += S_val[2]*16;
				S_val[3] = USART2_RX_BUF[4]&0x0F;
				val += S_val[3];
				//printf("%f\r\n", num);
				//计算出对应的测量数据
				if(val <= 32768)
				{
					//FSD11-30传感器数据计算
					if(FSD11_flag == 0)
						val = 30+((val *1)/1000);
					//FSD11-50传感器数据计算
					else if(FSD11_flag == 1)
						val = 50+((val *2)/1000);
				}
				else
				{
					//FSD11-30传感器数据计算
					if(FSD11_flag == 0)
						val = 30+(((val-65535)*1)/1000);
					//FSD11-50传感器数据计算
					else if(FSD11_flag == 1)
						val = 50+(((val-65535)*2)/1000);
				}
				//根据三角函数计算出传感器到测量点的垂直距离
				level_val = val*cos_tab[angle_val];
				if(level_flag == 0)
					offset = level_val;
				level_val -= offset;
				
				//将对应的数值转换为字符串
				sprintf(str_val, "%.2f", val);
				sprintf(level_str_val, "%.2f", level_val);
				
				//设置文本框的对应文本
				EDIT_SetFont(hID_EDIT_0, GUI_FONT_32_ASCII);
				EDIT_SetText(hID_EDIT_0, level_str_val);
			}
			
			USART2_RX_STA = 0;
		}
		//串口3接收程序段
		if(USART3_RX_STA&0x80)
		{
//			//获取接收字符个数（做多60个字符）
//			u8 b = USART3_RX_STA&0x3f;
//			//将接收到的字符通过串口3发送出去
//			HAL_UART_Transmit(&UART3_Handler, (u8 *)USART3_RX_BUF, b, 1000);
//			//发送换行回车符
//			Uart_Send_Str(&UART3_Handler, "\r\n");
			//清除接收完成标志位
			USART3_RX_STA = 0;
		}
	}
}

//字库更新任务
//KEY_UP键长按2s更新字库
void fontupdata_task(void *pdata)
{
	OS_ERR err;
	while(1)
	{
		if(WK_UP == 1)				//KEY_UP键按下
		{
			OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);//延时2s
			if(WK_UP == 1)			//还是KEY_UP键
			{
				LCD_Clear(WHITE);
				OSSchedLock(&err);		//调度器上锁
				LCD_ShowString(10,50,250,30,16,"Font Updataing,Please Wait...");
				update_font(10,70,16,"0:");//更新字库
				LCD_Clear(WHITE);
				POINT_COLOR = RED;
				LCD_ShowString(10,50,280,30,16,"Font Updata finshed,Please Restart!");
				OSSchedUnlock(&err);	//调度器解锁
			}
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);//延时10ms
	}
}


//触摸屏任务函数
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_PERIODIC,&err);
	}
}

//LED0任务函数
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
	}
}




