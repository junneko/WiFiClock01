/**
*	WiFi
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

extern httpd_handle_t	*httpserver;

static EventGroupHandle_t	s_wifi_event_group;	/* FreeRTOS event group to signal when we are connected */

static const char *DTAG = "dynamic_ip";
static const char *STAG = "static_ip";
static const char *WTAG = "wifi";

/* ------------------------------ Wi-Fi Access Point(�ݒ�p) ------------------------------ */
/**
*	�A�N�Z�X�|�C���g�C�x���g�n���h���[
*	 in	arg:
*		event_base:
*		event_id:
*		event_data:
*	out	�Ȃ�
*/
static void	ap_event_handler( void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data )
{
	/* ----- �ڑ������Ƃ� ----- */
	if( event_id == WIFI_EVENT_AP_STACONNECTED ){	
		gpio_set_level( WIFI_EN, ON );
		/* ----- HTTP SERVER START ----- */
		if( httpserver == NULL ){
			httpserver = webserver_start( );
		}
	}
	/* ----- �ؒf�����Ƃ� ----- */
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
*	WiFi �\�t�g�A�N�Z�X�|�C���g���[�h������
*	 in	�Ȃ�
*	out	�Ȃ�
*/
void	initialize_wifi_softap( void )
{
	ESP_ERROR_CHECK( esp_netif_init( ));
	ESP_ERROR_CHECK( esp_event_loop_create_default( ));
	esp_netif_create_default_wifi_ap( );
	wifi_init_config_t	cfg = WIFI_INIT_CONFIG_DEFAULT( );
	ESP_ERROR_CHECK( esp_wifi_init( &cfg ));
	ESP_ERROR_CHECK( esp_event_handler_instance_register( WIFI_EVENT, ESP_EVENT_ANY_ID, &ap_event_handler, NULL, NULL ));

	/* ----- �A�N�Z�X�|�C���g�ݒ���e ----- */
	wifi_config_t	wifi_config = {
		.ap = {
			.ssid = AP_ESP_WIFI_SSID,		/* SSID */
			.ssid_len = strlen( AP_ESP_WIFI_SSID ),	/*  */
			.channel = AP_ESP_WIFI_CHANNEL,		/* WiFi�`�����l�� */
			.password = AP_ESP_WIFI_PASS,		/* �p�X���[�h */
			.max_connection = AP_MAX_STA_CONN,	/* �ő�ڑ��䐔 */
			.authmode = WIFI_AUTH_WPA_WPA2_PSK,	/* �F�ؕ��� *//*WIFI_AUTH_OPEN*/
		}
	};
	if( strlen( AP_ESP_WIFI_PASS ) == 0 ){			/* �p�X���[�h�Ȃ� */
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}
	ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_AP ));	/* ���샂�[�h(AP MODE) */
	ESP_ERROR_CHECK( esp_wifi_set_config( WIFI_IF_AP, &wifi_config ));	/* �A�N�Z�X�|�C���g�ݒ� */
	ESP_ERROR_CHECK( esp_wifi_start( ));			/* �A�N�Z�X�|�C���g����J�n */

}

/*  ------------------------------ Wi-Fi Station Mode(�^�p�p)  ------------------------------ */

static int	s_retry_num = 0;

/**
*	�X�e�[�V�����C�x���g�n���h��(xEventGroupWaitBits�֐������s���ɔ���)
*	 in	arg:
*		event_base:
*		event_id:
*		event_data:
*	out	�Ȃ�
*/
static void	sta_event_handler( void* arg, esp_event_base_t event_base, int32_t event_id, void *event_data )
{
	/* ----- WiFi�Ɋւ���C�x���g ----- */
	if( event_base == WIFI_EVENT ){
		/* --- �ڑ��J�n --- */
		if( event_id == WIFI_EVENT_STA_START ){			/*  */
			esp_wifi_connect( );
			gpio_set_level( WIFI_EN, ON );
		}
		/* --- �ڑ����� --- */
		else if( event_id == WIFI_EVENT_STA_CONNECTED ){
			if( addr_mode == IP_STATIC ){
				set_static_ip( arg );
			}
		}
		/* --- �ؒf���� --- */
		else if( event_id == WIFI_EVENT_STA_DISCONNECTED ){
			if( s_retry_num < STA_ESP_MAXIMUM_RETRY ){	/* ���s�񐔓� */
				esp_wifi_connect( );
				s_retry_num++;
			}
			else{
				xEventGroupSetBits( s_wifi_event_group, WIFI_FAIL_BIT );	/* ���s���s���Z�b�g */
			}
		}
	}
	/* ----- IP�A�h���X�Ɋւ���C�x���g ----- */
	else if( event_base == IP_EVENT ){
		if( event_id == IP_EVENT_STA_GOT_IP ){		/* IP�A�h���X���� */
			ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
			if( addr_mode == IP_DYNAMIC ){
				ESP_LOGI( DTAG, "dynamic ip:" IPSTR, IP2STR( &event->ip_info.ip ));
			}
			else{
				ESP_LOGI( STAG, "static ip:" IPSTR, IP2STR( &event->ip_info.ip ));
			}
			s_retry_num = 0;
			xEventGroupSetBits( s_wifi_event_group, WIFI_CONNECTED_BIT );		/* WiFi�ڑ�����(IP�擾OK) */
		}
	}
}

/* ----- ������ ----- */
void	initialize_wifi_station( void )
{
	esp_event_handler_instance_t	instance_any_id;
	esp_event_handler_instance_t	instance_got_ip;
	esp_netif_t	*sta_netif;
	wifi_config_t	wifi_config = {
		.sta = {
			.ssid = "",	/* �󔒏����� */
        		.password = "",	/* �󔒏����� */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
		},
	};
	EventBits_t	bits;

	httpserver = NULL;
	strcpy((char *)wifi_config.sta.ssid, ssid );
	strcpy((char *)wifi_config.sta.password, password );
	s_wifi_event_group = xEventGroupCreate( );		/* �C�x���g���m���邽�߂̏����ݒ� */
	ESP_ERROR_CHECK( esp_netif_init( ));			/* netif������ */
	ESP_ERROR_CHECK( esp_event_loop_create_default( ));	/*  */
	sta_netif = esp_netif_create_default_wifi_sta( );
	assert( sta_netif );

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT( );	/* �ؖ����������ϐ����쐬 */
	ESP_ERROR_CHECK( esp_wifi_init( &cfg ));
	ESP_ERROR_CHECK( esp_event_handler_instance_register( IP_EVENT, IP_EVENT_STA_GOT_IP, &sta_event_handler, sta_netif, &instance_got_ip ));	/* IP_EVENT���荞�ݏ����o�^ */
	ESP_ERROR_CHECK( esp_event_handler_instance_register( WIFI_EVENT, ESP_EVENT_ANY_ID, &sta_event_handler, sta_netif, &instance_any_id ));	/* WIFI_EVENT���荞�ݏ����o�^ */

	ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ));
	ESP_ERROR_CHECK( esp_wifi_set_config( WIFI_IF_STA, &wifi_config ));
	ESP_ERROR_CHECK( esp_wifi_start( ));
	ESP_LOGI( WTAG, "wifi_init_sta finished." );

	/* ----- �C�x���g�̉�����ҋ@(WIFI_CONNECTED_BIT, WIFI_FAIL_BIT�ǂ��炩�Ŕ�����) ----- */
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
	else if( bits & WIFI_FAIL_BIT ){	/* �Đڑ����s�I��� */
		ESP_LOGI( WTAG, "Failed to connect to SSID:%s, password:%s", ssid, password );
		if( httpserver ){
			httpd_stop( httpserver );	/* HTTP�T�[�o��~ */
			httpserver = NULL;
		}
		gpio_set_level( WIFI_EN, OFF );
		gpio_set_level( IP_EN, OFF );
	}
	else{
		ESP_LOGE( WTAG, "UNEXPECTED EVENT" );
	}

	/* �C�x���g�̓o�^������ */
	ESP_ERROR_CHECK( esp_event_handler_instance_unregister( IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip ));
	ESP_ERROR_CHECK( esp_event_handler_instance_unregister( WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id ));
	vEventGroupDelete( s_wifi_event_group );
}

/* end of wifi.c */
