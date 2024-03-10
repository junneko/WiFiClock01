/**
*	�\���̒�`(��i����)
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#ifndef	_STRUCT_USER_DEFINED
#define	_STRUCT_USER_DEFINED

#include	"userdef.h"

/* �^�C�}�p���ԍ\���� 4byte */
struct TIMER_ST{
	byte	wday;			/* ���j�� bit0-6=Sun-Sat */
	byte	hour;
	byte	min;
	byte	sec;
};
typedef struct TIMER_ST	timer_st;

#endif

/* end of file */
