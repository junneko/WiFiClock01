/**
*	SNTP処理
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

static const char	*TAG = "sntp";

/**
*	SNTP 時刻更新のコールバック関数
*	 in	時刻
*	out	なし
*/
void	sntp_update( struct timeval *tv )
{
	settimeofday( tv, NULL );	/* 時刻→RTC */
	ESP_LOGI( TAG, "Time is synchronized from custom code" );
	sntp_set_sync_status( SNTP_SYNC_STATUS_COMPLETED );
}

/**
*	SNTP初期化
*	 in	なし
*	out	なし
*/
void	sntp_initialize( void )
{
	sntp_set_sync_mode( SNTP_SYNC_MODE_IMMED );	/* 即座に反映 */
	esp_sntp_setoperatingmode( SNTP_OPMODE_POLL );	/* 一定時間で見に行く方式 */
	esp_sntp_setservername( 0, ntpserver1 );
	esp_sntp_setservername( 1, ntpserver2 );
	esp_sntp_setservername( 2, ntpserver3 );
	sntp_set_time_sync_notification_cb( sntp_update );	/*  */
	esp_sntp_init( );				/* 初期化 */
	sntp_set_sync_interval( NTP_POLLTIME );		/* 更新間隔(ms) */
	sntp_restart( );				/* 適用(再起動) */
}

/* end of sntp.c */
