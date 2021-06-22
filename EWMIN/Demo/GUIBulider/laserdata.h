#ifndef __LASERDATA_H
#define __LASERDATA_H

#include "sys.h"
#include "wm.h"

#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_EDIT_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_0 (GUI_ID_USER + 0x04)
#define ID_BUTTON_3 (GUI_ID_USER + 0x05)
#define ID_MULTIEDIT_0 (GUI_ID_USER + 0x06)
#define ID_TEXT_2 (GUI_ID_USER + 0x07)
#define ID_BUTTON_2 (GUI_ID_USER + 0x08)
#define ID_RADIO_0 (GUI_ID_USER + 0x09)
#define ID_RADIO_1 (GUI_ID_USER + 0x0A)
#define ID_SPINBOX_0 (GUI_ID_USER + 0x0B)

extern WM_HWIN hID_EDIT_0;
extern WM_HWIN hID_MULTIEDIT_0;
extern u8 TimerRun;
extern u8 FSD11_flag;
extern u8 level_flag;
extern u8 angle_val;
extern char *level_str[];
extern char str_val[10];      //²âµÃ¾àÀë×Ö·û´®´æ´¢
extern char level_str_val[10];//¼ÆËã´¹Ö±¾àÀë×Ö·û´®´æ´¢

WM_HWIN stm32_show(void);

#endif

