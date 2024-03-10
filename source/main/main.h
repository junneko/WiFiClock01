/**
*	�e��C���N���[�h
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	<stdio.h>
#include	<string.h>
#include	<inttypes.h>
#include	<time.h>
#include	<sys/time.h>

#include	"freertos/FreeRTOS.h"
#include	"freertos/task.h"
#include	"freertos/event_groups.h"

#include	"esp_system.h"
#include	"esp_mac.h"
#include	"esp_wifi.h"
#include	"esp_netif.h"
#include	"esp_log.h"
#include	"spi_flash_mmap.h"

#include	"esp_event.h"
#include	"nvs_flash.h"

#include	"driver/spi_master.h"
#include	"driver/gpio.h"

#include	"lwip/err.h"
#include	"lwip/sys.h"

#include	"esp_sntp.h"
#include	"esp_http_server.h"

#include	"define.h"
#include	"userdef.h"
#include	"struct_std.h"
#include	"struct_user.h"
#include	"prototype.h"

/* ----- �ݒ�f�[�^ ----- */
_Extern	byte		timezone;	/* �^�C���]�[�� 1 */
_Extern	byte		tdispmode;	/* �\�����ԃ��[�h 0:12h 1:24h 2:28h 3:32h 1 */

_Extern	byte		timermode[4];	/* �^�C�}���[�h 4 */
_Extern	timer_st	on_time[4];	/* �^�C�}�ɂ��ON���� 16 */
_Extern	timer_st	off_time[4];	/* �^�C�}�ɂ��OFF���� 16 */

_Extern	char	ntpserver1[32];	
_Extern	char	ntpserver2[32];	
_Extern	char	ntpserver3[32];	

_Extern	char	staticip[16];	
_Extern	char	staticnm[16];	
_Extern	char	staticgw[16];	
_Extern	char	staticdns1[16];	
_Extern	char	staticdns2[16];	

/* ----- SPI ----- */
_Extern	spi_device_handle_t	spi;
_Extern	spi_transaction_t	trnst;

/* ----- ESP ----- */
_Extern	esp_err_t	esperr;
_Extern	char	ssid[32];
_Extern	char	password[32];

/* ----- ���� ----- */
_Extern	time_t	nowtime;		/* �ŐV���� */
_Extern	struct tm	timeinfo;	/* ������� */

/* ----- IP�A�h���X ----- */
_Extern	esp_netif_ip_info_t	ip_info;	/* WiFi�Ŏ擾�����A�h���X��� */

_Extern byte	singleon[4];		/* �V���O�����[�hON������ */

/* ----- ���[�h ----- */
_Extern	int	wifi_mode;
_Extern	int	addr_mode;

/* -----  ----- */
_Extern	uint8_t	timerstatus;


/* ----- �\�� ----- */
extern uint8_t	segbuf[16];

/* -----  ----- */
extern const char	*version;

/* end of main.h */
