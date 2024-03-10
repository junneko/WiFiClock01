/**
*	CGI����
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

/**
*	setup��ʂ̏���
*	 in	���N�G�X�g���
*	out	�G���[�R�[�h
*/
esp_err_t	cgi_setup( httpd_req_t *req )
{
	char	*buf;
	char	param[32], str[16];
	size_t	buf_len;
	nvs_handle_t	my_handle;
	esp_err_t	ret;
	int	sts;

	extern const char	*timezonetbl[];

	ret = ESP_OK;
	buf_len = req->content_len;	/* �f�[�^�T�C�Y */
	buf = malloc( buf_len + 1 );	/* NULL��+1�������m�� */
	sts = httpd_req_recv( req, buf, buf_len );
	/* ----- �p�����[�^����(�����ꍇ�͒P�Ȃ�y�[�W�v��) ----- */
	if( sts > 0 ){
		ret = nvs_open( "storage", NVS_READWRITE, &my_handle );
		if( ret == ESP_OK ){
			*( buf + buf_len ) = C_NULL;	/* str terminal */
			/* ----- �l�̎��o�� ----- */
			if( httpd_query_key_value( buf, "SSID", param, sizeof( param )) == ESP_OK ){	/* SSID */
				strcpy( ssid, param );
				ret = nvs_set_str( my_handle, "SSID", ssid );
			}
			if( httpd_query_key_value( buf, "PWD", param, sizeof( param )) == ESP_OK ){	/* PASSWORD */
				strcpy( password, param );
				ret = nvs_set_str( my_handle, "PASSWORD", password );
			}
			if( httpd_query_key_value( buf, "IPADR", param, sizeof( param )) == ESP_OK ){	/* IPADDR */
				strcpy( staticip, param );
				ret = nvs_set_str( my_handle, "IPADDRESS", staticip );
			}
			if( httpd_query_key_value( buf, "NMASK", param, sizeof( param )) == ESP_OK ){	/* NETMASK */
				strcpy( staticnm, param );
				ret = nvs_set_str( my_handle, "SUBNETMASK", staticnm );
			}
			if( httpd_query_key_value( buf, "GWAY", param, sizeof( param )) == ESP_OK ){	/* GATEWAY */
				strcpy( staticgw, param );
				ret = nvs_set_str( my_handle, "GATEWAY", staticgw );
			}
			if( httpd_query_key_value( buf, "DNS1", param, sizeof( param )) == ESP_OK ){	/* DNS MAIN */
				strcpy( staticdns1, param );
				ret = nvs_set_str( my_handle, "DNSMAIN", staticdns1 );
			}
			if( httpd_query_key_value( buf, "DNS2", param, sizeof( param )) == ESP_OK ){	/* DNS BACKUP */
				strcpy( staticdns2, param );
				ret = nvs_set_str( my_handle, "DNSBACKUP", staticdns2 );
			}
			if( httpd_query_key_value( buf, "TZ", param, sizeof( param )) == ESP_OK ){	/* TIMEZONE */
				timezone = atoi( param );
				ret = nvs_set_u8( my_handle, "TIMEZONE", timezone );
				strcpy( str, timezonetbl[timezone] );
				setenv( "TZ", str, 1 );
				tzset( );
			}
			if( httpd_query_key_value( buf, "DT", param, sizeof( param )) == ESP_OK ){	/* DISPTIME */
				tdispmode = atoi( param );
				ret = nvs_set_u8( my_handle, "DISPTIME", tdispmode );
			}
			ret = nvs_commit( my_handle );	/* �l��nvs�ɏ������� */
			nvs_close( my_handle );
		}
	}
	/* ----- �㏈�� ----- */
	free( buf );		/* ��������� */

	return	ret;
}

/**
*	NTP�T�[�o�ݒ�̏���
*	 in	���N�G�X�g���
*	out	�G���[�R�[�h
*/
esp_err_t	cgi_ntpserver( httpd_req_t *req )
{
	char	*buf;
	char	param[32];
	size_t	buf_len;
	nvs_handle_t	my_handle;
	esp_err_t	ret;
	int	sts;

	ret = ESP_OK;
	buf_len = req->content_len;		/* �����񒷂� */
	buf = malloc( buf_len + 1 );		/* NULL��1 */
	sts = httpd_req_recv( req, buf, buf_len );
	/* ----- �p�����[�^����(�����ꍇ�͒P�Ȃ�y�[�W�v��) ----- */
	if( sts > 0 ){
		ret = nvs_open( "storage", NVS_READWRITE, &my_handle );
		if( ret == ESP_OK ){
			*( buf + buf_len ) = C_NULL;	/* str terminal */
			/* ----- �e�L�[�̃f�[�^�ǂݏo�� ----- */
			if( httpd_query_key_value( buf, "NS1", param, sizeof( param )) == ESP_OK ){	/* SERVER1 */
				strcpy( ntpserver1, param );
				ret = nvs_set_str( my_handle, "NTPSERVER1", ntpserver1 );
			}
			if( httpd_query_key_value( buf, "NS2", param, sizeof( param )) == ESP_OK ){	/* SERVER2 */
				strcpy( ntpserver2, param );
				ret = nvs_set_str( my_handle, "NTPSERVER2", ntpserver2 );
			}
			if( httpd_query_key_value( buf, "NS3", param, sizeof( param )) == ESP_OK ){	/* SERVER3 */
				strcpy( ntpserver3, param );
				ret = nvs_set_str( my_handle, "NTPSERVER3", ntpserver3 );
			}
			ret = nvs_commit( my_handle );	/* �l��nvs�ɏ������� */
			nvs_close( my_handle );
		}
	}
	/* ----- �㏈�� ----- */
	free( buf );		/* ��������� */

	return	ret;
}

/**
*	�^�C�}�ݒ�̏���
*	 in	���N�G�X�g���
*	out	�G���[�R�[�h
*/
esp_err_t	cgi_timer( httpd_req_t *req )
{
	char	*buf, str[16];
	char	param[32];
	int	i, ch;
	size_t	buf_len;
	nvs_handle_t	my_handle;
	esp_err_t	ret;
	int	sts;

	ch = 0;
	ret = ESP_OK;
	buf_len = req->content_len;
	buf = malloc( buf_len + 1 );	/* NULL��1 */
	sts = httpd_req_recv( req, buf, buf_len );
	/* ----- �p�����[�^����(�����ꍇ�͒P�Ȃ�y�[�W�v��) ----- */
	if( sts > 0 ){
		ret = nvs_open( "storage", NVS_READWRITE, &my_handle );
		if( ret == ESP_OK ){
			*( buf + buf_len ) = C_NULL;	/* str terminal */
			/* ----- CH ----- */
			if( httpd_query_key_value( buf, "TIMER", param, sizeof( param )) == ESP_OK ){	/* CH */
				ch = atoi( param ) - 1;
			}
			/* ----- MODE ----- */
			if( httpd_query_key_value( buf, "TMD", param, sizeof( param )) == ESP_OK ){	/* MODE */
				timermode[ch] = atoi( param );
				strcpy( str, "TIMERMODE " );
				str[9] = ch + '1';
				ret = nvs_set_u8( my_handle, str, timermode[ch] );
			}
			/* ----- ON TIME ----- */
			strcpy( param, "ONW= " );	/* ON WDAY�̓r�b�g�ɂ܂Ƃ߂�̂�strstr�g�p */
			on_time[ch].wday = 0x00;
			for( i = 0; i < 7; i++ ){
				param[4] = i + '0';
				if( strstr( buf, param ) != NULL ){
					on_time[ch].wday |= BYTE_BIT_X( i );
				}
			}
			strcpy( str, "ONWDAY " );
			str[6] = ch + '1';
			ret = nvs_set_u8( my_handle, str, on_time[ch].wday );
			if( httpd_query_key_value( buf, "ONH", param, sizeof( param )) == ESP_OK ){	/* ON HOUR */
				on_time[ch].hour = atoi( param );
				strcpy( str, "ONHOUR " );
				str[6] = ch + '1';
				ret = nvs_set_u8( my_handle, str, on_time[ch].hour );
			}
			if( httpd_query_key_value( buf, "ONM", param, sizeof( param )) == ESP_OK ){	/* ON MINUTE */
				on_time[ch].min = atoi( param );
				strcpy( str, "ONMIN " );
				str[5] = ch + '1';
				ret = nvs_set_u8( my_handle, str, on_time[ch].min );
			}
			if( httpd_query_key_value( buf, "ONS", param, sizeof( param )) == ESP_OK ){	/* ON SECOND */
				on_time[ch].sec = atoi( param );
				strcpy( str, "ONSEC " );
				str[5] = ch + '1';
				ret = nvs_set_u8( my_handle, str, on_time[ch].sec );
			}
			/* ----- OFF TIME ----- */
			strcpy( param, "OFW= " );	/* OFF WDAY�̓r�b�g�ɂ܂Ƃ߂�̂�strstr�g�p */
			off_time[ch].wday = 0x00;
			for( i = 0; i < 7; i++ ){
				param[4] = i + '0';
				if( strstr( buf, param ) != NULL ){
					off_time[ch].wday |= BYTE_BIT_X( i );
				}
			}
			strcpy( str, "OFFWDAY " );
			str[7] = ch + '1';
			ret = nvs_set_u8( my_handle, str, off_time[ch].wday );
			if( httpd_query_key_value( buf, "OFH", param, sizeof( param )) == ESP_OK ){	/* OFF HOUR */
				off_time[ch].hour = atoi( param );
				strcpy( str, "OFFHOUR " );
				str[7] = ch + '1';
				ret = nvs_set_u8( my_handle, str, off_time[ch].hour );
			}
			if( httpd_query_key_value( buf, "OFM", param, sizeof( param )) == ESP_OK ){	/* OFF MINUTE */
				off_time[ch].min = atoi( param );
				strcpy( str, "OFFMIN " );
				str[6] = ch + '1';
				ret = nvs_set_u8( my_handle, str, off_time[ch].min );
			}
			if( httpd_query_key_value( buf, "OFS", param, sizeof( param )) == ESP_OK ){	/* OFF SECOND */
				off_time[ch].sec = atoi( param );
				strcpy( str, "OFFSEC " );
				str[6] = ch + '1';
				ret = nvs_set_u8( my_handle, str, off_time[ch].sec );
			}
			ret = nvs_commit( my_handle );	/* �l��nvs�ɏ������� */
			nvs_close( my_handle );
		}
	}
	/* ----- �㏈�� ----- */
	free( buf );

	return	ret;
}

/* end of cgi.c */
