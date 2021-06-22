#include "laserdata.h"
#include "DIALOG.h"
#include "EmWinHZFont.h"
#include "usart.h"

WM_HWIN hID_EDIT_0;
WM_HWIN hID_MULTIEDIT_0;
WM_HWIN hID_BUTTON_3;
WM_HWIN hID_SPINBOX_0;

u8 TimerRun = 0;
u8 FSD11_flag = 0;
u8 level_flag = 0;
u8 angle_val = 0;
char str_val[10];      //��þ����ַ����洢
char level_str_val[10];//���㴹ֱ�����ַ����洢

char *level_str[] = {"�ο���", "���ĵ�", "���ϵ�", "���ϵ�", "���µ�", "���µ�", "ƫ����"};
char *level_button_str[] = {"�زο���", "�����ĵ�", "�����ϵ�", "�����ϵ�", "�����µ�", "�����µ�", "���"};

//��ȡ��������������ָ��
char send_date[] = {0x80,0x03,0x9C,0x7D,0x00,0x01,0x24,0x53};

void send_buffer(char *buff, int len)
{
	for(u8 i=0; i<len; i++)
	{
		while((USART2->SR&0x0080)==0);   
		USART2->DR=buff[i];
	}
}

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "chuanganqi", ID_FRAMEWIN_0, 0, 0, 480, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "dancicaiji", ID_BUTTON_0, 330, 15, 120, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "lianxucaiji", ID_BUTTON_1, 330, 65, 120, 45, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 17, 60, 110, 50, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "mm", ID_TEXT_0, 127, 60, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "dangqian", ID_BUTTON_3, 183, 60, 120, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 330, 110, 120, 40, 0, 0x64, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 9, 120, 300, 160, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "qingkongshuju", ID_BUTTON_2, 330, 230, 120, 45, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio1", ID_RADIO_0, 30, 20, 120, 25, 0, 0x1, 0 },
  { RADIO_CreateIndirect, "Radio2", ID_RADIO_1, 160, 20, 120, 25, 0, 0x1, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 330, 160, 120, 45, 0, 0x0, 0 },
};

//�������ڵĻص�����
static void _BkCallback(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			
			break;
		case WM_TIMER:
			if(TimerRun == 1)
			{
				send_buffer(send_date, sizeof(send_date));
				WM_RestartTimer(pMsg->Data.v, 200);
			}
			else
				WM_DeleteTimer(pMsg->Data.v);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}	
}

static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, &GUI_FontHZ24);
    FRAMEWIN_SetText(hItem, "���⴫�������ݲɼ�");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetTitleHeight(hItem, 30);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontHZ16);
    BUTTON_SetText(hItem, "��¼����");
		WM_DisableWindow(hItem);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontHZ16);
    BUTTON_SetText(hItem, "�ɼ�����");
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, &GUI_FontHZ16);
    BUTTON_SetText(hItem, "�������");
    
    hID_EDIT_0 = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetFont(hID_EDIT_0, GUI_FONT_32_ASCII);
    EDIT_SetText(hID_EDIT_0, "0.00");
    EDIT_SetTextAlign(hID_EDIT_0, GUI_TA_RIGHT | GUI_TA_VCENTER);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
    
    hID_BUTTON_3 = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hID_BUTTON_3, &GUI_FontHZ24);
    BUTTON_SetText(hID_BUTTON_3, level_button_str[level_flag]);
    
    hID_MULTIEDIT_0 = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
		MULTIEDIT_SetWrapWord(hID_MULTIEDIT_0);
		MULTIEDIT_SetReadOnly(hID_MULTIEDIT_0, 1);
		MULTIEDIT_SetBufferSize(hID_MULTIEDIT_0, 100);
		MULTIEDIT_SetAutoScrollV(hID_MULTIEDIT_0, 1);
    MULTIEDIT_SetFont(hID_MULTIEDIT_0, &GUI_FontHZ16);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_BOTTOM);
		TEXT_SetFont(hItem, &GUI_FontHZ16);
    TEXT_SetText(hItem, "�������Ƕ�");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
    RADIO_SetFont(hItem, GUI_FONT_24B_ASCII);
    RADIO_SetText(hItem, "FSD11-30", 0);
    RADIO_SetGroupId(hItem, 1);
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
    RADIO_SetFont(hItem, GUI_FONT_24B_ASCII);
    RADIO_SetText(hItem, "FSD11-50", 0);
    RADIO_SetGroupId(hItem, 1);
		
    hID_SPINBOX_0 = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
		SPINBOX_SetSkin(hID_SPINBOX_0, SPINBOX_SKIN_FLEX);
		SPINBOX_SetButtonSize(hID_SPINBOX_0, 70);
		SPINBOX_SetRange(hID_SPINBOX_0, 0, 90);
		SPINBOX_SetStep(hID_SPINBOX_0, 1);
    SPINBOX_SetFont(hID_SPINBOX_0, GUI_FONT_32_ASCII);
		SPINBOX_SetValue(hID_SPINBOX_0,30);
		angle_val = SPINBOX_GetValue(hID_SPINBOX_0);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        //��¼���ݰ�ť�����¼�
			
        break;
      case WM_NOTIFICATION_RELEASED:
        //��¼���ݰ�ť�ͷ��¼�
				if(level_flag < 6)
				{
					MULTIEDIT_SetFont(hID_MULTIEDIT_0, &GUI_FontHZ24);
					MULTIEDIT_AddText(hID_MULTIEDIT_0, level_str[level_flag]);
					MULTIEDIT_AddText(hID_MULTIEDIT_0, ":");
					MULTIEDIT_AddText(hID_MULTIEDIT_0, str_val);
					MULTIEDIT_AddText(hID_MULTIEDIT_0, ";ƫ��:");
					MULTIEDIT_AddText(hID_MULTIEDIT_0, level_str_val);
					
					//�������һ���ƽ��󣬲�����س���
					level_flag++;
					if(level_flag < 6)
					{
						MULTIEDIT_AddText(hID_MULTIEDIT_0, "\n");
						WM_EnableWindow(hID_BUTTON_3);
					}
					if(level_flag > 5)
					{
						hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
						WM_DisableWindow(hItem);
					}
					BUTTON_SetText(hID_BUTTON_3, level_button_str[level_flag]);
				}
        break;
      }
      break;
    case ID_BUTTON_1:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        //�ɼ����ݰ�ť�����¼�
			
        break;
      case WM_NOTIFICATION_RELEASED:
        //�ɼ����ݰ�ť�ͷ��¼�
				if(TimerRun == 0)
				{
					TimerRun = 1;
					WM_CreateTimer(WM_HBKWIN,0,200,0);
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
					BUTTON_SetFont(hItem, &GUI_FontHZ16);
					BUTTON_SetText(hItem, "ֹͣ�ɼ�");
					if(level_flag < 6)
					{
						hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
						WM_EnableWindow(hItem);
					}
				}
				else
				{
					TimerRun = 0;
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
					BUTTON_SetFont(hItem, &GUI_FontHZ16);
					BUTTON_SetText(hItem, "�ɼ�����");
					if(level_flag < 6)
					{
						hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
						WM_DisableWindow(hItem);
					}
				}
					
        break;
      }
      break;
			
		case ID_BUTTON_2: 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				//������ݰ�ť�����¼�
			
        break;
      case WM_NOTIFICATION_RELEASED:
				//������ݰ�ť�ͷ��¼�
				MULTIEDIT_SetText(hID_MULTIEDIT_0, "");
				level_flag = 0;
				BUTTON_SetText(hID_BUTTON_3, level_str[level_flag]);
				WM_EnableWindow(hID_BUTTON_3);
				EDIT_SetText(hID_EDIT_0, "0.00");
				if(TimerRun ==1)
				{
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
					WM_EnableWindow(hItem);
				}
        break;
      }
      break;	
			
			case ID_BUTTON_3: 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				//������ݰ�ť�����¼�
			
        break;
      case WM_NOTIFICATION_RELEASED:
				//������ݰ�ť�ͷ��¼�
				switch(level_flag)
				{
					case 0://�زο���
						Uart_Send_Str(&UART3_Handler, "G92 Z0\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 Z10 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G28 X Y\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 X100 Y210 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G28 Z\r\n");
						Uart_Send_Str(&UART3_Handler, "\r\n");
					break;
					
					case 1://�����ĵ�
						Uart_Send_Str(&UART3_Handler, "G1 Z10 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 X110 Y90 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 Z2 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "\r\n");
					break;
					
					case 2://�����ϵ�
						Uart_Send_Str(&UART3_Handler, "G1 Z10 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 X200 Y0 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 Z2 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "\r\n");
					break;
					
					case 3://�����ϵ�
						Uart_Send_Str(&UART3_Handler, "G1 Z10 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 X20 Y0 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 Z2 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "\r\n");
					break;
					
					case 4://�����µ�
						Uart_Send_Str(&UART3_Handler, "G1 Z10 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 X20 Y170 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 Z2 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "\r\n");
					break;
					
					case 5://�����µ�
						Uart_Send_Str(&UART3_Handler, "G1 Z10 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 X200 Y170 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "G1 Z2 F800\r\n");
						Uart_Send_Str(&UART3_Handler, "\r\n");
					break;
				}
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
				WM_DisableWindow(hItem);
        break;
      }
      break;
			
    case ID_EDIT_0://�����ı����¼�
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        //�ı������¼�
			
        break;
      case WM_NOTIFICATION_RELEASED:
        //�ı����ͷ��¼�
				
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        //�ı������ݸı��¼�
			
        break;
      }
      break;
    case ID_MULTIEDIT_0: // Notifications sent by 'Multiedit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				//�����ı������¼�
				
        break;
      case WM_NOTIFICATION_RELEASED:
				//�����ı����ͷ��¼�
				
      case WM_NOTIFICATION_VALUE_CHANGED:
				//�����ı������ݸı��¼�
			
        break;
      }
      break;
		case ID_RADIO_0:
			switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        //��ѡ�����¼�
				
        break;
      case WM_NOTIFICATION_RELEASED:
        //��ѡ���ͷ��¼�
				//FSD11_30������
				FSD11_flag = 0;
			
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        //��ѡ�����ݸı��¼�
				
        break;
      }
      break;
		case ID_RADIO_1:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        //��ѡ�����¼�
				
        break;
      case WM_NOTIFICATION_RELEASED:
        //��ѡ���ͷ��¼�
				//FSD11_50������
				FSD11_flag = 1;
        
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        //��ѡ�����ݸı��¼�
			
        break;
      }
      break;
		case ID_SPINBOX_0: // Notifications sent by 'Spinbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        //΢�������¼�
			
        break;
      case WM_NOTIFICATION_RELEASED:
        //΢�����ͷ��¼�
			
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        //ָ�����Ƴ�΢���������¼�
			
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        //΢�������ݸı��¼�
				angle_val = SPINBOX_GetValue(hID_SPINBOX_0);
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

WM_HWIN stm32_show(void) 
{
	WM_HWIN hWin;
	
	WM_SetCallback(WM_HBKWIN,_BkCallback);
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
	return hWin;
}


