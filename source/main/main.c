/* 
*	KDB-WiFi-Clock01メイン
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#define		_GLOBAL_DEFINED
#include	"main.h"

/* ----- バージョン ----- */
const char	*version = "0.31.00";

/**
*	内部値初期化
*	 in	なし
*	out	なし
*/
static void	val_initialize( void )
{
	int	i;

	timerstatus = 0xff;

	for( i = 0; i < 4; i++ ){
		singleon[i] = 0;
	}
}

/**
*	I/O初期化
*	 in	なし
*	out	なし
*/
static void	io_initialize( void )
{
	gpio_config_t	io_conf = {};

	/* ----- 機能リセット ----- */
	gpio_reset_pin( DISPLAYPIN );	/* 36 */
	gpio_reset_pin( STATICIPPIN );	/* 39 */

	/* ----- 入力 PU/PDなし 割り込みなしグループ ----- */
	io_conf.intr_type = GPIO_INTR_DISABLE;		// disable interrupt
	io_conf.mode = GPIO_MODE_INPUT;			// 
	io_conf.pin_bit_mask = ESP32_PIN34 | ESP32_PIN35 | ESP32_PIN36 | ESP32_PIN39;	// GPIO4,5,GPIO12(MISO),35
	io_conf.pull_down_en = 0;			// disable pull-down mode
	io_conf.pull_up_en = 0;				// disable pull-up mode
	gpio_config( &io_conf );			// configure GPIO with the given settings

	/* ----- 出力 ----- */
	esp_rom_gpio_pad_select_gpio( WIFI_EN );	// WiFi LED
	gpio_set_direction( WIFI_EN, GPIO_MODE_OUTPUT );
	esp_rom_gpio_pad_select_gpio( IP_EN );		// IP LED
	gpio_set_direction( IP_EN, GPIO_MODE_OUTPUT );
	esp_rom_gpio_pad_select_gpio( TIME_SYNC );	// SNTP SYNC LED
	gpio_set_direction( TIME_SYNC, GPIO_MODE_OUTPUT );

	/* ----- タイマー出力 ----- */
	esp_rom_gpio_pad_select_gpio( TIMER1PIN );	// TIMER1
	gpio_set_direction( TIMER1PIN, GPIO_MODE_OUTPUT );
	esp_rom_gpio_pad_select_gpio( TIMER2PIN );	// TIMER2
	gpio_set_direction( TIMER2PIN, GPIO_MODE_OUTPUT );
	esp_rom_gpio_pad_select_gpio( TIMER3PIN );	// TIMER3
	gpio_set_direction( TIMER3PIN, GPIO_MODE_OUTPUT );
	esp_rom_gpio_pad_select_gpio( TIMER4PIN );	// TIMER4
	gpio_set_direction( TIMER4PIN, GPIO_MODE_OUTPUT );
}

/**
*	保存パラメータ読み出し
*	 in	なし
*	out	なし
*/
static void	parameter_read( void )
{
	esp_err_t	err;
	nvs_handle_t	my_handle;
	size_t	size;
	int	i;

	err = nvs_open( "storage", NVS_READWRITE, &my_handle );
	if( err == ESP_OK ){
		/* ----- 固定アドレス ----- */
		nvs_get_str( my_handle, "IPADDRESS", NULL, &size );
		err = nvs_get_str( my_handle, "IPADDRESS", staticip, &size );
		if( err != ESP_OK ){
			strcpy( staticip, STATIC_IP_DEFAULT );		/* IP */
		}
		nvs_get_str( my_handle, "SUBNETMASK", NULL, &size );
		err = nvs_get_str( my_handle, "SUBNETMASK", staticnm, &size );
		if( err != ESP_OK ){
			strcpy( staticnm, STATIC_NM_DEFAULT );		/* NETMASK */
		}
		nvs_get_str( my_handle, "GATEWAY", NULL, &size );
		err = nvs_get_str( my_handle, "GATEWAY", staticgw, &size );
		if( err != ESP_OK ){
			strcpy( staticgw, STATIC_GW_DEFAULT );		/* GATEWAY */
		}
		nvs_get_str( my_handle, "DNSMAIN", NULL, &size );
		err = nvs_get_str( my_handle, "DNSMAIN", staticdns1, &size );
		if( err != ESP_OK ){
			strcpy( staticdns1, STATIC_DNSM_DEFAULT );	/* DNS1 */
		}
		nvs_get_str( my_handle, "DNSBACKUP", NULL, &size );
		err = nvs_get_str( my_handle, "DNSBACKUP", staticdns2, &size );
		if( err != ESP_OK ){
			strcpy( staticdns2, STATIC_DNSB_DEFAULT );	/* DNS2 */
		}
		/* ----- NTPサーバ ----- */
		nvs_get_str( my_handle, "NTPSERVER1", NULL, &size );
		err = nvs_get_str( my_handle, "NTPSERVER1", ntpserver1, &size );
		if( err != ESP_OK ){
			strcpy( ntpserver1, NTPSERVER1_DEFAULT );	/* NTP1 */
		}
		nvs_get_str( my_handle, "NTPSERVER2", NULL, &size );
		err = nvs_get_str( my_handle, "NTPSERVER2", ntpserver2, &size );
		if( err != ESP_OK ){
			strcpy( ntpserver2, NTPSERVER2_DEFAULT );	/* NTP2 */
		}
		nvs_get_str( my_handle, "NTPSERVER3", NULL, &size );
		err = nvs_get_str( my_handle, "NTPSERVER3", ntpserver3, &size );
		if( err != ESP_OK ){
			strcpy( ntpserver3, NTPSERVER3_DEFAULT );	/* NTP3 */
		}
		/* ----- Wi-Fi -----*/
		nvs_get_str( my_handle, "SSID", NULL, &size );		/* サイズ調査？ */
		err = nvs_get_str( my_handle, "SSID", ssid, &size );
		if( err != ESP_OK ){
			strcpy( ssid, STA_ESP_WIFI_SSID );
		}
		nvs_get_str( my_handle, "PASSWORD", NULL, &size );	/* サイズ調査？ */
		err = nvs_get_str( my_handle, "PASSWORD", password, &size );
		if( err != ESP_OK ){
			strcpy( password, STA_ESP_WIFI_PASS );
		}
		/* -----  -----*/
		err = nvs_get_u8( my_handle, "TIMEZONE", &timezone );	/* タイムゾーン */
		if( err != ESP_OK ){
			timezone = 9;		/* UTC+9.0 */
		}
		err = nvs_get_u8( my_handle, "DISPTIME", &tdispmode );	/* 表示時間モード */
		if( err != ESP_OK ){
			tdispmode = 1;		/* 24h */
		}
		/* ----- TIMER MODE -----*/
		err = nvs_get_u8( my_handle, "TIMERMODE1", &timermode[0] );
		if( err != ESP_OK ){
			timermode[0] = TIMMODE_OFF;
		}
		err = nvs_get_u8( my_handle, "TIMERMODE2", &timermode[1] );
		if( err != ESP_OK ){
			timermode[1] = TIMMODE_OFF;
		}
		err = nvs_get_u8( my_handle, "TIMERMODE3", &timermode[2] );
		if( err != ESP_OK ){
			timermode[2] = TIMMODE_OFF;
		}
		err = nvs_get_u8( my_handle, "TIMERMODE4", &timermode[3] );
		if( err != ESP_OK ){
			timermode[3] = TIMMODE_OFF;
		}
		/* ----- ON時間1 ----- */
		err = nvs_get_u8( my_handle, "ONWDAY1", &on_time[0].wday );
		if( err != ESP_OK ){
			on_time[0].wday = 0;
		}
		err = nvs_get_u8( my_handle, "ONHOUR1", &on_time[0].hour );
		if( err != ESP_OK ){
			on_time[0].hour = 0;
		}
		err = nvs_get_u8( my_handle, "ONMIN1", &on_time[0].min );
		if( err != ESP_OK ){
			on_time[0].min = 0;
		}
		err = nvs_get_u8( my_handle, "ONSEC1", &on_time[0].sec );
		if( err != ESP_OK ){
			on_time[0].sec = 0;
		}
		/* ----- ON時間2 ----- */
		err = nvs_get_u8( my_handle, "ONWDAY2", &on_time[1].wday );
		if( err != ESP_OK ){
			on_time[1].wday = 0;
		}
		err = nvs_get_u8( my_handle, "ONHOUR2", &on_time[1].hour );
		if( err != ESP_OK ){
			on_time[1].hour = 0;
		}
		err = nvs_get_u8( my_handle, "ONMIN2", &on_time[1].min );
		if( err != ESP_OK ){
			on_time[1].min = 0;
		}
		err = nvs_get_u8( my_handle, "ONSEC2", &on_time[1].sec );
		if( err != ESP_OK ){
			on_time[1].sec = 0;
		}
		/* ----- ON時間3 ----- */
		err = nvs_get_u8( my_handle, "ONWDAY3", &on_time[2].wday );
		if( err != ESP_OK ){
			on_time[2].wday = 0;
		}
		err = nvs_get_u8( my_handle, "ONHOUR3", &on_time[2].hour );
		if( err != ESP_OK ){
			on_time[2].hour = 0;
		}
		err = nvs_get_u8( my_handle, "ONMIN3", &on_time[2].min );
		if( err != ESP_OK ){
			on_time[2].min = 0;
		}
		err = nvs_get_u8( my_handle, "ONSEC3", &on_time[2].sec );
		if( err != ESP_OK ){
			on_time[2].sec = 0;
		}
		/* ----- ON時間4 ----- */
		err = nvs_get_u8( my_handle, "ONWDAY4", &on_time[3].wday );
		if( err != ESP_OK ){
			on_time[3].wday = 0;
		}
		err = nvs_get_u8( my_handle, "ONHOUR4", &on_time[3].hour );
		if( err != ESP_OK ){
			on_time[3].hour = 0;
		}
		err = nvs_get_u8( my_handle, "ONMIN4", &on_time[3].min );
		if( err != ESP_OK ){
			on_time[3].min = 0;
		}
		err = nvs_get_u8( my_handle, "ONSEC4", &on_time[3].sec );
		if( err != ESP_OK ){
			on_time[3].sec = 0;
		}
		/* ----- OFF時間1 ----- */
		err = nvs_get_u8( my_handle, "OFFWDAY1", &off_time[0].wday );
		if( err != ESP_OK ){
			off_time[0].wday = 0;
		}
		err = nvs_get_u8( my_handle, "OFFHOUR1", &off_time[0].hour );
		if( err != ESP_OK ){
			off_time[0].hour = 0;
		}
		err = nvs_get_u8( my_handle, "OFFMIN1", &off_time[0].min );
		if( err != ESP_OK ){
			off_time[0].min = 0;
		}
		err = nvs_get_u8( my_handle, "OFFSEC1", &off_time[0].sec );
		if( err != ESP_OK ){
			off_time[0].sec = 0;
		}
		/* ----- OFF時間2 ----- */
		err = nvs_get_u8( my_handle, "OFFWDAY2", &off_time[1].wday );
		if( err != ESP_OK ){
			off_time[1].wday = 0;
		}
		err = nvs_get_u8( my_handle, "OFFHOUR2", &off_time[1].hour );
		if( err != ESP_OK ){
			off_time[1].hour = 0;
		}
		err = nvs_get_u8( my_handle, "OFFMIN2", &off_time[1].min );
		if( err != ESP_OK ){
			off_time[1].min = 0;
		}
		err = nvs_get_u8( my_handle, "OFFSEC2", &off_time[1].sec );
		if( err != ESP_OK ){
			off_time[1].sec = 0;
		}
		/* ----- OFF時間3 ----- */
		err = nvs_get_u8( my_handle, "OFFWDAY3", &off_time[2].wday );
		if( err != ESP_OK ){
			off_time[2].wday = 0;
		}
		err = nvs_get_u8( my_handle, "OFFHOUR3", &off_time[2].hour );
		if( err != ESP_OK ){
			off_time[2].hour = 0;
		}
		err = nvs_get_u8( my_handle, "OFFMIN3", &off_time[2].min );
		if( err != ESP_OK ){
			off_time[2].min = 0;
		}
		err = nvs_get_u8( my_handle, "OFFSEC3", &off_time[2].sec );
		if( err != ESP_OK ){
			off_time[2].sec = 0;
		}
		/* ----- OFF時間4 ----- */
		err = nvs_get_u8( my_handle, "OFFWDAY4", &off_time[3].wday );
		if( err != ESP_OK ){
			off_time[3].wday = 0;
		}
		err = nvs_get_u8( my_handle, "OFFHOUR4", &off_time[3].hour );
		if( err != ESP_OK ){
			off_time[3].hour = 0;
		}
		err = nvs_get_u8( my_handle, "OFFMIN4", &off_time[3].min );
		if( err != ESP_OK ){
			off_time[3].min = 0;
		}
		err = nvs_get_u8( my_handle, "OFFSEC4", &off_time[3].sec );
		if( err != ESP_OK ){
			off_time[3].sec = 0;
		}

		nvs_close( my_handle );
	}
	else{	/* NVSエラー */
		strcpy( staticip, STATIC_IP_DEFAULT );		/* IP */
		strcpy( staticnm, STATIC_NM_DEFAULT );		/* NETMASK */
		strcpy( staticgw, STATIC_GW_DEFAULT );		/* GATEWAY */
		strcpy( staticdns1, STATIC_DNSM_DEFAULT );	/* DNS1 */
		strcpy( staticdns2, STATIC_DNSB_DEFAULT );	/* DNS */
		/* ----- NTPサーバ ----- */
		strcpy( ntpserver1, NTPSERVER1_DEFAULT );	/* NTP1 */
		strcpy( ntpserver2, NTPSERVER2_DEFAULT );	/* NTP2 */
		strcpy( ntpserver3, NTPSERVER3_DEFAULT );	/* NTP3 */
		/* ----- Wi-Fi -----*/
		strcpy( ssid, STA_ESP_WIFI_SSID );
		strcpy( password, STA_ESP_WIFI_PASS );
		/* ----- TIMER -----*/
		for( i = 0; i < 4; i++ ){
			timermode[i] = TIMMODE_OFF;
			on_time[i].wday = 0x00;
			off_time[i].wday = 0x00;
			on_time[i].hour = 0;
			on_time[i].min = 0;
			on_time[i].sec = 0;
			off_time[i].hour = 0;
			off_time[i].min = 0;
			off_time[i].sec = 0;
			singleon[i] = 0;
		}
	}
}

/**
*	内部初期化
*	 in	なし
*	out	なし
*/
static void	internal_initialize( void )
{
	char	str[16];
	int	retry, ofst = 0;
	time_t	tmp;
	int	retry_count = 8;
	esp_err_t	ret;
	extern const char	*timezonetbl[];

	/* ----- nvs (non volatile storage)領域を初期化 ----- */
	ret = nvs_flash_init( );
	if(( ret == ESP_ERR_NVS_NO_FREE_PAGES ) || ( ret == ESP_ERR_NVS_NEW_VERSION_FOUND )){	/* 空のページが含まれているか新バージョンのコードがある */
		ESP_ERROR_CHECK( nvs_flash_erase( ));	/* 一度イレース */
		ret = nvs_flash_init( );
	}
	ESP_ERROR_CHECK( ret );

	/* ----- 周辺初期化 ----- */
	val_initialize( );		/* 変数初期化 */
	io_initialize( );		/* I/O初期化 */
	spi_initialize( );		/* SPI初期化 */

	seg7_initialize( );		/* 7セグメント消す */

	/* ----- 保存パラメータ読み出し ----- */
	parameter_read( );

	/* ----- バージョン表示 ----- */
	version_display( );		/* バージョンは必ず表示 */
	dispenflg = gpio_get_level( DISPENPIN );
	dispflg = gpio_get_level( DISPLAYPIN );

	/* ----- モード決定 ----- */
	wifi_mode = gpio_get_level( MODEPIN );
	if( wifi_mode ){		/* OPEN:Station */
		if( strlen( ssid ) == 0 ){	/* SSID未設定は設定モードへ */
			wifi_mode = WIFI_AP;
		}
		addr_mode = gpio_get_level( STATICIPPIN );	/* OPEN:DHCP,SHORT:STATIC */
	}
	else{				/* SHORT:Access Point */
		addr_mode = IP_STATIC;
	}
	/* ---------- Station mode(通常動作) ---------- */
	if( wifi_mode == WIFI_STATION ){
		initialize_wifi_station( );
		esp_netif_get_ip_info( esp_netif_get_handle_from_ifkey( "WIFI_STA_DEF" ), &ip_info );	/* アドレス情報ゲット */
		ipaddress_display( );		/* IPアドレス表示 */
		/* ----- Set timezone ----- */
		strcpy( str, timezonetbl[timezone] );
		setenv( "TZ", str, 1 );
		tzset( );
		/* ----- SNTP初期化 ----- */
		sntp_initialize( );
		vTaskDelay( 1000 / portTICK_PERIOD_MS );	/* 1sec */
		/* ----- 時刻が取得されるのを待つ(IPアドレス表示中) ----- */
		nowtime = 0;
		retry = 0;
		_forever{
			if( sntp_get_sync_status( ) == SNTP_SYNC_STATUS_COMPLETED ){	// SNTP_SYNC_STATUS_RESET：ステータスリセットされます。
				break;							// SNTP_SYNC_STATUS_COMPLETED：時刻同期完了
			}								// SNTP_SYNC_STATUS_IN_PROGRESS：時間同期進行中
			retry++;
			if( retry >= retry_count ){
				break;
			}
			vTaskDelay( 2000 / portTICK_PERIOD_MS );	/* 2sec */
		}
		/* ----- 時刻検証 ----- */
		time( &nowtime );
		localtime_r( &nowtime, &timeinfo );	/* 現在時刻 */
		if( tdispmode == MODE28H ){		/* 28h */
			tmp = nowtime - HOUR28OFFSET;		/* 4時間分秒戻る */
			ofst = 4;
		}
		else if( tdispmode == MODE30H ){	/* 30h */
			tmp = nowtime - HOUR30OFFSET;		/* 6時間分秒戻る */
			ofst = 6;
		}
		if( ofst != 0 ){			/* 28,30時間制のみ */
			localtime_r( &tmp, &cmptime );
			if( cmptime.tm_mday != timeinfo.tm_mday ){	/* 日が変わったときだけ処理(24-30) */
				timeinfo = cmptime;
				timeinfo.tm_hour += ofst;	/* 前日のデータに時間足す */
			}
		}
		/* ----- 時刻表示 ----- */
		if( dispenflg && dispflg ){
			time_display( &timeinfo );	/* 表示 */
		}
		else{
			seg7_dispoff( );
		}
	}
	/* ---------- Access Point mode(アクセスポイント設定) ---------- */
	else{
		initialize_wifi_softap( );
		vTaskDelay( 2000 / portTICK_PERIOD_MS );	/* 2sec Ver表示 */
		esp_netif_get_ip_info( esp_netif_get_handle_from_ifkey( "WIFI_AP_DEF" ), &ip_info );	/* アドレス情報ゲット */
		if( dispenflg && dispflg ){
			apmode_display( );	/* APmode */
		}
		else{
			seg7_dispoff( );
		}
	}
}

/**
*	7セグの表示(メイン)
*	 in	最新表示ポイント
*	out	更新表示ポイント
*/
static int	seg7_display( int disp )
{
	/* --- 表示OFF --- */
	if( dispflg == 0 ){			/* SHORT:表示しない */
		seg7_dispoff( );
	}
	/* --- GPIO4 OPEN:DISP TIME --- */
	else if( modepin ){
		time_display( &timeinfo );	/* 表示は後 */
		disp = 0;
	}
	/* --- SHORT:DISP ADDRESS --- */
	else{
		if( disp == 0 ){
			ipaddress_display( );	/* IP */
		}
		else if( disp == 2 ){
			netmask_display( );	/* NM */
		}
		else if( disp == 4 ){
			gateway_display( );	/* GW */
		}
		disp++;
		if( disp >= 6 ){
			disp = 0;
		}
	}

	return	disp;
}

/**
*	メイン
*	 in	なし
*	out	なし
*/
void	app_main( void )
{
	time_t	oldt, tmp;
	int	ofst;
	int	dispitm;	/* 表示項目 */

	internal_initialize( );
	/* ----- メインループ ----- */
	ofst = 0;
	dispitm = 0;
	time( &nowtime );	/* 現在時刻取得 */
	oldt = nowtime;
	_forever{
		/* ----- 入力 ----- */
		dispenflg = gpio_get_level( DISPENPIN );
		dispflg = gpio_get_level( DISPLAYPIN );
		modepin = gpio_get_level( MODEPIN );
		/* ---------- Station mode ---------- */
		if( wifi_mode == WIFI_STATION ){
			time( &nowtime );		/* 現在時刻取得 */
			if( oldt != nowtime ){		/* 時刻更新した */
				oldt = nowtime;
				if( sntp_get_sync_status( ) == SNTP_SYNC_STATUS_COMPLETED ){
					gpio_set_level( TIME_SYNC, ON );	/* 更新した1秒だけ */
				}
				else{
					gpio_set_level( TIME_SYNC, OFF );
				}
				localtime_r( &nowtime, &timeinfo );	/* 時刻変換 time_t→rtc_t */
				timer_sequence( );		/* タイマ */
				/* ----- 表示 ----- */
				if( tdispmode == MODE28H ){		/* 28h */
					tmp = nowtime - HOUR28OFFSET;		/* 4時間分秒戻る */
					ofst = 4;
				}
				else if( tdispmode == MODE30H ){	/* 30h */
					tmp = nowtime - HOUR30OFFSET;		/* 6時間分秒戻る */
					ofst = 6;
				}
				if( ofst != 0 ){			/* 28,30時間制のみ */
					localtime_r( &tmp, &cmptime );
					if( cmptime.tm_mday != timeinfo.tm_mday ){	/* 日が変わったときだけ処理(24-30) */
						timeinfo = cmptime;
						timeinfo.tm_hour += ofst;	/* 前日のデータに時間足す */
					}
				}
				if( dispenflg ){
					dispitm = seg7_display( dispitm );
				}
				else{
					dispitm = 0;
				}
			}
		}
		/* ---------- Access Point mode ---------- */
		else{
			ipaddress_display( );		/* IP ADDRESS */
			vTaskDelay( 1000 / portTICK_PERIOD_MS );
			apmode_display( );		/* AP mode */
			vTaskDelay( 1000 / portTICK_PERIOD_MS );
		}
	}
	esp_restart( );				/* バグで抜けてしまったとき用 */
}

/* end of main.c */
