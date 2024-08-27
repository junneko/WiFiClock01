/**
*	定義ファイル
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
/* ----- 表示バージョン ----- */
#define	UVER	0	/* 大改訂 */
#define	MVER	3	/* 中改訂(機能追加) */
#define	LVER	0	/* 小改訂(バグ修正など) */

/* ----- I/O設定用 ----- */
#define	ESP32_PIN34	(1ULL<<34)
#define	ESP32_PIN35	(1ULL<<35)
#define	ESP32_PIN36	(1ULL<<36)
#define	ESP32_PIN39	(1ULL<<39)

/* ----- I/Oピン定義 ----- */
#define	DISPLAYPIN	34	/* 表示なし(L:ブランク表示で電流節約) */
#define	DISPENPIN	35	/* 7セグ表示有効(L:転送しない) */
#define	MODEPIN		36	/* Wi-Fiモード/IP表示 */
#define	STATICIPPIN	39	/* IP ADDRESS固定 */

#define	TIMER1PIN	16
#define	TIMER2PIN	17
#define	TIMER3PIN	18
#define	TIMER4PIN	19

#define	WIFI_EN		25
#define	IP_EN		26
#define	TIME_SYNC	27

/* ----- SPI ----- */
#define	PIN_NUM_MISO	-1	/* 12 未使用 */
#define	PIN_NUM_MOSI	13
#define	PIN_NUM_CLK	14
#define	PIN_NUM_CS	15

/* ----- IPアドレス取得 ----- */
#define	IP_DYNAMIC	1
#define	IP_STATIC	0

/* ----- Wi-Fi ----- */
/* --- AP MODE --- */
#define	WIFI_STATION	1
#define	WIFI_AP		0

/* --- Wi-Fi Access Point --- */
#define	AP_ESP_WIFI_SSID	"KDBWiFiClock01AP"	/* SSID */
#define	AP_ESP_WIFI_PASS	""			/* password(OPEN) */
#define	AP_ESP_WIFI_CHANNEL	1
#define	AP_MAX_STA_CONN		4

/* --- Wi-Fi Station Point --- */
#define	STA_ESP_WIFI_SSID	""		/* 初期はAPmodeになるように空白 *//**/
#define	STA_ESP_WIFI_PASS	""		/**/
#define	STA_ESP_MAXIMUM_RETRY	5

#define	WIFI_CONNECTED_BIT	BIT0
#define	WIFI_FAIL_BIT		BIT1
#define	WIFI_STA_CONN_BIT	BIT2
#define	WIFI_STA_DCON_BIT	BIT3

/* --- 時刻表示定義 --- */
#define	MODE12H			0
#define	MODE24H			1
#define	MODE28H			2
#define	MODE30H			3

#define	HOUR28OFFSET		14400L		/* 28時間制 */
#define	HOUR30OFFSET		21600L		/* 30時間制 */

/* ----- タイマモード ----- */
#define	TIMMODE_OFF		0
#define	TIMMODE_SINGLE		1
#define	TIMMODE_CONTINUE	2

/* ----- SNTP ----- */
#define	NTP_POLLTIME		1024000		/* NTP更新間隔(ms) */

/* ----- 初期値 ----- */
#define	STATIC_IP_DEFAULT	"192.168.1.100"
#define	STATIC_NM_DEFAULT	"255.255.255.0"
#define	STATIC_GW_DEFAULT	"192.168.1.1"
#define	STATIC_DNSM_DEFAULT	"192.168.1.1"
#define	STATIC_DNSB_DEFAULT	"192.168.1.1"

#define	NTPSERVER1_DEFAULT	"ntp.jst.mfeed.ad.jp"
#define	NTPSERVER2_DEFAULT	"time.cloudflare.com"
#define	NTPSERVER3_DEFAULT	"time.aws.com"

/* end of define.h */
