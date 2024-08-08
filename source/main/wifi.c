/**
*	WiFi
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

extern httpd_handle_t	*httpserver;

static EventGroupHandle_t	s_wifi_event_group;	/* FreeRTOS event group to signal when we are connected */

static const char *DTAG = "dynamic_ip";
static const char *STAG = "static_ip";
static const char *WTAG = "wifi";

/* ------------------------------ Wi-Fi Access Point(設定用) ------------------------------ */
/**
*	アクセスポイントイベントハンドラー
*	 in	arg:
*		event_base:
*		event_id:
*		event_data:
*	out	なし
*/
static void	ap_event_handler( void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data )
{
	/* ----- 接続したとき ----- */
	if( event_id == WIFI_EVENT_AP_STACONNECTED ){	
		gpio_set_level( WIFI_EN, ON );
		/* ----- HTTP SERVER START ----- */
		if( httpserver == NULL ){
			httpserver = webserver_start( );
		}
	}
	/* ----- 切断したとき ----- */
	else if( event_id == WIFI_EVENT_AP_STADISCONNECTED ){
		gpio_set_level( WIFI_EN, OFF );
		/* ----- HTTP SERVER STOP ----- */
		if( httpserver ){
			httpd_stop( httpserver );
			httpserver = NULL;
		}
	}
}

/**
*	WiFi ソフトアクセスポイントモード初期化
*	 in	なし
*	out	なし
*/
void	initialize_wifi_softap( void )
{
	ESP_ERROR_CHECK( esp_netif_init( ));
	ESP_ERROR_CHECK( esp_event_loop_create_default( ));
	esp_netif_create_default_wifi_ap( );
	wifi_init_config_t	cfg = WIFI_INIT_CONFIG_DEFAULT( );
	ESP_ERROR_CHECK( esp_wifi_init( &cfg ));
	ESP_ERROR_CHECK( esp_event_handler_instance_register( WIFI_EVENT, ESP_EVENT_ANY_ID, &ap_event_handler, NULL, NULL ));

	/* ----- アクセスポイント設定内容 ----- */
	wifi_config_t	wifi_config = {
		.ap = {
			.ssid = AP_ESP_WIFI_SSID,		/* SSID */
			.ssid_len = strlen( AP_ESP_WIFI_SSID ),	/*  */
			.channel = AP_ESP_WIFI_CHANNEL,		/* WiFiチャンネル */
			.password = AP_ESP_WIFI_PASS,		/* パスワード */
			.max_connection = AP_MAX_STA_CONN,	/* 最大接続台数 */
			.authmode = WIFI_AUTH_WPA_WPA2_PSK,	/* 認証方式 *//*WIFI_AUTH_OPEN*/
		}
	};
	if( strlen( AP_ESP_WIFI_PASS ) == 0 ){			/* パスワードなし */
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}
	ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_AP ));	/* 動作モード(AP MODE) */
	ESP_ERROR_CHECK( esp_wifi_set_config( WIFI_IF_AP, &wifi_config ));	/* アクセスポイント設定 */
	ESP_ERROR_CHECK( esp_wifi_start( ));			/* アクセスポイント動作開始 */

}

/*  ------------------------------ Wi-Fi Station Mode(運用用)  ------------------------------ */

static int	s_retry_num = 0;

/**
*	ステーションイベントハンドラ(xEventGroupWaitBits関数を実行中に発生)
*	 in	arg:
*		event_base:
*		event_id:
*		event_data:
*	out	なし
*/
static void	sta_event_handler( void* arg, esp_event_base_t event_base, int32_t event_id, void *event_data )
{
	/* ----- WiFiに関するイベント ----- */
	if( event_base == WIFI_EVENT ){
		/* --- 接続開始 --- */
		if( event_id == WIFI_EVENT_STA_START ){			/*  */
			esp_wifi_connect( );
			gpio_set_level( WIFI_EN, ON );
		}
		/* --- 接続した --- */
		else if( event_id == WIFI_EVENT_STA_CONNECTED ){
			if( addr_mode == IP_STATIC ){
				set_static_ip( arg );
			}
		}
		/* --- 切断した --- */
		else if( event_id == WIFI_EVENT_STA_DISCONNECTED ){
			if( s_retry_num < STA_ESP_MAXIMUM_RETRY ){	/* 試行回数内 */
				esp_wifi_connect( );
				s_retry_num++;
			}
			else{
				xEventGroupSetBits( s_wifi_event_group, WIFI_FAIL_BIT );	/* 試行失敗情報セット */
			}
		}
	}
	/* ----- IPアドレスに関するイベント ----- */
	else if( event_base == IP_EVENT ){
		if( event_id == IP_EVENT_STA_GOT_IP ){		/* IPアドレス完了 */
			ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
			if( addr_mode == IP_DYNAMIC ){
				ESP_LOGI( DTAG, "dynamic ip:" IPSTR, IP2STR( &event->ip_info.ip ));
			}
			else{
				ESP_LOGI( STAG, "static ip:" IPSTR, IP2STR( &event->ip_info.ip ));
			}
			s_retry_num = 0;
			xEventGroupSetBits( s_wifi_event_group, WIFI_CONNECTED_BIT );		/* WiFi接続完了(IP取得OK) */
		}
	}
}

/* ----- 初期化 ----- */
void	initialize_wifi_station( void )
{
	esp_event_handler_instance_t	instance_any_id;
	esp_event_handler_instance_t	instance_got_ip;
	esp_netif_t	*sta_netif;
	wifi_config_t	wifi_config = {
		.sta = {
			.ssid = "",	/* 空白初期化 */
        		.password = "",	/* 空白初期化 */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
		},
	};
	EventBits_t	bits;

	httpserver = NULL;
	strcpy((char *)wifi_config.sta.ssid, ssid );
	strcpy((char *)wifi_config.sta.password, password );
	s_wifi_event_group = xEventGroupCreate( );		/* イベント検知するための初期設定 */
	ESP_ERROR_CHECK( esp_netif_init( ));			/* netif初期化 */
	ESP_ERROR_CHECK( esp_event_loop_create_default( ));	/*  */
	sta_netif = esp_netif_create_default_wifi_sta( );
	assert( sta_netif );

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT( );	/* 証明書を扱う変数を作成 */
	ESP_ERROR_CHECK( esp_wifi_init( &cfg ));
	ESP_ERROR_CHECK( esp_event_handler_instance_register( IP_EVENT, IP_EVENT_STA_GOT_IP, &sta_event_handler, sta_netif, &instance_got_ip ));	/* IP_EVENT割り込み処理登録 */
	ESP_ERROR_CHECK( esp_event_handler_instance_register( WIFI_EVENT, ESP_EVENT_ANY_ID, &sta_event_handler, sta_netif, &instance_any_id ));	/* WIFI_EVENT割り込み処理登録 */

	ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ));
	ESP_ERROR_CHECK( esp_wifi_set_config( WIFI_IF_STA, &wifi_config ));
	ESP_ERROR_CHECK( esp_wifi_start( ));
	ESP_LOGI( WTAG, "wifi_init_sta finished." );

	/* ----- イベントの応答を待機(WIFI_CONNECTED_BIT, WIFI_FAIL_BITどちらかで抜ける) ----- */
	bits = xEventGroupWaitBits( s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY );
	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually happened. */
	if( bits & WIFI_CONNECTED_BIT ){	/*  */
		ESP_LOGI( WTAG, "connected to ap SSID:%s password:%s", ssid, password );
		/* ----- HTTP SERVER START ----- */
		if( httpserver == NULL ){
			httpserver = webserver_start( );
		}
		gpio_set_level( IP_EN, ON );
	}
	else if( bits & WIFI_FAIL_BIT ){	/* 再接続試行終わり */
		ESP_LOGI( WTAG, "Failed to connect to SSID:%s, password:%s", ssid, password );
		if( httpserver ){
			httpd_stop( httpserver );	/* HTTPサーバ停止 */
			httpserver = NULL;
		}
		gpio_set_level( WIFI_EN, OFF );
		gpio_set_level( IP_EN, OFF );
	}
	else{
		ESP_LOGE( WTAG, "UNEXPECTED EVENT" );
	}

	/* イベントの登録を解除 */
	ESP_ERROR_CHECK( esp_event_handler_instance_unregister( IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip ));
	ESP_ERROR_CHECK( esp_event_handler_instance_unregister( WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id ));
	vEventGroupDelete( s_wifi_event_group );
}

/* end of wifi.c */
