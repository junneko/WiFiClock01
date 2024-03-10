/**
*	構造体定義(作品ごと)
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#ifndef	_STRUCT_USER_DEFINED
#define	_STRUCT_USER_DEFINED

#include	"userdef.h"

/* タイマ用時間構造体 4byte */
struct TIMER_ST{
	byte	wday;			/* 許可曜日 bit0-6=Sun-Sat */
	byte	hour;
	byte	min;
	byte	sec;
};
typedef struct TIMER_ST	timer_st;

#endif

/* end of file */
