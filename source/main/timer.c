/**
*	�^�C�}����
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

/**
*	ON�^�C�}
*	 in	�Ȃ�
*	out	�Ȃ�
*/
void	on_timer( byte pnt )
{
	byte	flg;

	/* --- ���ԃ`�F�b�N --- */
	if( on_time[pnt].hour != timeinfo.tm_hour ){	/* ������Ȃ� */
		return;
	}
	if( on_time[pnt].min != timeinfo.tm_min ){	/* ������Ȃ� */
		return;
	}
	if( on_time[pnt].sec != timeinfo.tm_sec ){	/* �b����Ȃ� */
		return;
	}
	/* --- ON���� --- */
	if( pnt == 0 ){
		BYTE_BIT5_OFF( timerstatus );
		gpio_set_level( TIMER1PIN, 1 );
	}
	else if( pnt == 1 ){
		BYTE_BIT1_OFF( timerstatus );
		gpio_set_level( TIMER2PIN, 1 );
	}
	else if( pnt == 2 ){
		BYTE_BIT4_OFF( timerstatus );
		gpio_set_level( TIMER3PIN, 1 );
	}
	else{
		BYTE_BIT2_OFF( timerstatus );
		gpio_set_level( TIMER4PIN, 1 );
	}
	if( timermode[pnt] == TIMMODE_SINGLE ){	/* �V���O������ */
		flg = 0;
		if( on_time[pnt].hour == off_time[pnt].hour ){
			flg |= 0x01;
		}
		if( on_time[pnt].min == off_time[pnt].min ){
			flg |= 0x02;
		}
		if( on_time[pnt].sec == off_time[pnt].sec ){
			flg |= 0x04;
		}
		if( flg == 0x07 ){		/* OFF�^�C�}���������Ƃ���ON�̂܂܂� */
			timermode[pnt] = TIMMODE_OFF;
		}
		else{
			singleon[pnt] = on;	/* ON�� */
		}
	}
}

/**
*	OFF�^�C�}
*	 in	�Ȃ�
*	out	�Ȃ�
*/
void	off_timer( byte pnt )
{
	/* --- ���ԃ`�F�b�N --- */
	if( off_time[pnt].hour != timeinfo.tm_hour ){	/* ������Ȃ� */
		return;
	}
	if( off_time[pnt].min != timeinfo.tm_min ){	/* ������Ȃ� */
		return;
	}
	if( off_time[pnt].sec != timeinfo.tm_sec ){	/* �b����Ȃ� */
		return;
	}
	/* --- OFF���� --- */
	if( pnt == 0 ){		/* TIMER1 */
		BYTE_BIT5_ON( timerstatus );
		gpio_set_level( TIMER1PIN, 0 );
	}
	else if( pnt == 1 ){	/* TIMER2 */
		BYTE_BIT1_ON( timerstatus );
		gpio_set_level( TIMER2PIN, 0 );
	}
	else if( pnt == 2 ){	/* TIMER3 */
		BYTE_BIT4_ON( timerstatus );
		gpio_set_level( TIMER3PIN, 0 );
	}
	else{			/* TIMER4 */
		BYTE_BIT2_ON( timerstatus );
		gpio_set_level( TIMER4PIN, 0 );
	}

	if( timermode[pnt] == TIMMODE_SINGLE ){	/* �V���O�������OFF�ŏI�� */
		timermode[pnt] = TIMMODE_OFF;
	}
}

/**
*	�^�C�}����
*	 in	�Ȃ�
*	out	�Ȃ�
*/
void	timer_sequence( void )
{
	byte	i, wdbit;

	for( i = 0; i < 4; i++ ){
		if( timermode[i] == TIMMODE_SINGLE ){		/* �V���O�����[�h�͗j���Ɋ֌W�Ȃ����s */
			if( singleon[i] == on ){	/* ON���o�Ă��瓮��(ON->OFF�̏ꍇ) */
				off_timer( i );
			}
			on_timer( i );			/* ON�����s�������l�����邩���Ȃ̂�ON������ */
		}
		else if( timermode[i] == TIMMODE_CONTINUE ){	/* �J��Ԃ����[�h */
			wdbit = BYTE_BIT_X( timeinfo.tm_wday );
			if( on_time[i].wday & wdbit ){	/* ON�^�C�}�L���j�� */
				on_timer( i );
			}
			if( off_time[i].wday & wdbit ){	/* OFF�^�C�}�L���j�� */
				off_timer( i );		/* �������ݒ肳����ON�����ςȂ��Ȃ̂�OFF������ */
			}
		}
	}
}

/* end of timer.c */
