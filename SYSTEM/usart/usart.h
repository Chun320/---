#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	

#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
#define USART2_REC_LEN  			7 	//定义最大接收字节数 7
#define USART3_REC_LEN  			60 	//定义最大接收字节数 200
#define EN_USART_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; //UART句柄

extern u8 USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8 USART2_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART2_Handler; //UART句柄

extern u8 USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8 USART3_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART3_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRx1Buffer[RXBUFFERSIZE];//HAL库USART接收Buffer
extern u8 aRx2Buffer[RXBUFFERSIZE];//HAL库USART接收Buffer
extern u8 aRx3Buffer[RXBUFFERSIZE];//HAL库USART接收Buffer

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void Uart_Send_Str(UART_HandleTypeDef *huart, char *str);

#endif
