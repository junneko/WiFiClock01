/**
*	タイマ処理
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

/**
*	ONタイマ
*	 in	なし
*	out	なし
*/
void	on_timer( byte pnt )
{
	byte	flg;

	/* --- 時間チェック --- */
	if( on_time[pnt].hour != timeinfo.tm_hour ){	/* 時合わない */
		return;
	}
	if( on_time[pnt].min != timeinfo.tm_min ){	/* 分合わない */
		return;
	}
	if( on_time[pnt].sec != timeinfo.tm_sec ){	/* 秒合わない */
		return;
	}
	/* --- ON動作 --- */
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
	if( timermode[pnt] == TIMMODE_SINGLE ){	/* シングル動作 */
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
		if( flg == 0x07 ){		/* OFFタイマ時刻同じときはONのままに */
			timermode[pnt] = TIMMODE_OFF;
		}
		else{
			singleon[pnt] = on;	/* ON済 */
		}
	}
}

/**
*	OFFタイマ
*	 in	なし
*	out	なし
*/
void	off_timer( byte pnt )
{
	/* --- 時間チェック --- */
	if( off_time[pnt].hour != timeinfo.tm_hour ){	/* 時合わない */
		return;
	}
	if( off_time[pnt].min != timeinfo.tm_min ){	/* 分合わない */
		return;
	}
	if( off_time[pnt].sec != timeinfo.tm_sec ){	/* 秒合わない */
		return;
	}
	/* --- OFF動作 --- */
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

	if( timermode[pnt] == TIMMODE_SINGLE ){	/* シングル動作はOFFで終了 */
		timermode[pnt] = TIMMODE_OFF;
	}
}

/**
*	タイマ処理
*	 in	なし
*	out	なし
*/
void	timer_sequence( void )
{
	byte	i, wdbit;

	for( i = 0; i < 4; i++ ){
		if( timermode[i] == TIMMODE_SINGLE ){		/* シングルモードは曜日に関係なく実行 */
			if( singleon[i] == on ){	/* ONを経てから動作(ON->OFFの場合) */
				off_timer( i );
			}
			on_timer( i );			/* ONだけ行いたい人が居るかもなのでONを後ろに */
		}
		else if( timermode[i] == TIMMODE_CONTINUE ){	/* 繰り返しモード */
			wdbit = BYTE_BIT_X( timeinfo.tm_wday );
			if( on_time[i].wday & wdbit ){	/* ONタイマ有効曜日 */
				on_timer( i );
			}
			if( off_time[i].wday & wdbit ){	/* OFFタイマ有効曜日 */
				off_timer( i );		/* 同時刻設定されるとONしっぱなしなのでOFFを後ろに */
			}
		}
	}
}

/* end of timer.c */
