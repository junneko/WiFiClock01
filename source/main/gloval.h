/**
*	グローバル変数定義
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/

/* ----- 設定データ ----- */
_Extern	byte		timezone;	/* タイムゾーン 1 */
_Extern	byte		tdispmode;	/* 表示時間モード 0:12h 1:24h 2:28h 3:32h 1 */

_Extern	byte		timermode[4];	/* タイマモード 4 */
_Extern	timer_st	on_time[4];	/* タイマによるON時刻 16 */
_Extern	timer_st	off_time[4];	/* タイマによるOFF時刻 16 */

_Extern	char		ntpserver1[32];	
_Extern	char		ntpserver2[32];	
_Extern	char		ntpserver3[32];	

_Extern	char		staticip[16];	
_Extern	char		staticnm[16];	
_Extern	char		staticgw[16];	
_Extern	char		staticdns1[16];	
_Extern	char		staticdns2[16];	

/* ----- SPI ----- */
_Extern	spi_device_handle_t	spi;
_Extern	spi_transaction_t	trnst;

/* ----- ESP ----- */
_Extern	esp_err_t	esperr;
_Extern	char		ssid[32];
_Extern	char		password[32];

/* ----- 時刻 ----- */
_Extern	time_t	nowtime;		/* 最新時刻 */
_Extern	struct tm	cmptime;	/* 比較用時刻情報(28,30時間制用) */
_Extern	struct tm	timeinfo;	/* 時刻情報 */

/* ----- IPアドレス ----- */
_Extern	esp_netif_ip_info_t	ip_info;	/* WiFiで取得したアドレス情報 */

_Extern byte	singleon[4];		/* シングルモードONしたか */

/* ----- モード ----- */
_Extern	int	wifi_mode;
_Extern	int	addr_mode;		/* アドレス取得方法 */

/* -----  ----- */
_Extern	uint8_t	timerstatus;

/* ----- 表示 ----- */
_Extern uint8_t	segbuf[16];		/* 7SEG転送用バッファ 年月日8桁、時分秒8桁 */
_Extern int	dispenflg;		/* 7SEGに転送するか */
_Extern int	dispflg;		/* 7SEGに表示するか */
_Extern int	modepin;		/* 動作モードピン状態 */

/* -----  ----- */
extern const char	*version;

/* end of file */
