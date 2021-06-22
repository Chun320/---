#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	

#define USART1_REC_LEN  			200  	//�����������ֽ��� 200
#define USART2_REC_LEN  			7 	//�����������ֽ��� 7
#define USART3_REC_LEN  			60 	//�����������ֽ��� 200
#define EN_USART_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART1_Handler; //UART���

extern u8 USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART2_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART2_Handler; //UART���

extern u8 USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART3_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART3_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRx1Buffer[RXBUFFERSIZE];//HAL��USART����Buffer
extern u8 aRx2Buffer[RXBUFFERSIZE];//HAL��USART����Buffer
extern u8 aRx3Buffer[RXBUFFERSIZE];//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void Uart_Send_Str(UART_HandleTypeDef *huart, char *str);

#endif
