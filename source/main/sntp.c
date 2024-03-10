/**
*	SNTP����
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

static const char	*TAG = "sntp";

/**
*	SNTP �����X�V�̃R�[���o�b�N�֐�
*	 in	����
*	out	�Ȃ�
*/
void	sntp_update( struct timeval *tv )
{
	settimeofday( tv, NULL );	/* ������RTC */
	ESP_LOGI( TAG, "Time is synchronized from custom code" );
	sntp_set_sync_status( SNTP_SYNC_STATUS_COMPLETED );
}

/**
*	SNTP������
*	 in	�Ȃ�
*	out	�Ȃ�
*/
void	sntp_initialize( void )
{
	sntp_set_sync_mode( SNTP_SYNC_MODE_IMMED );	/* �����ɔ��f */
	esp_sntp_setoperatingmode( SNTP_OPMODE_POLL );	/* ��莞�ԂŌ��ɍs������ */
	esp_sntp_setservername( 0, ntpserver1 );
	esp_sntp_setservername( 1, ntpserver2 );
	esp_sntp_setservername( 2, ntpserver3 );
	sntp_set_time_sync_notification_cb( sntp_update );	/*  */
	esp_sntp_init( );				/* ������ */
	sntp_set_sync_interval( NTP_POLLTIME );		/* �X�V�Ԋu(ms) */
	sntp_restart( );				/* �K�p(�ċN��) */
}

/* end of sntp.c */
