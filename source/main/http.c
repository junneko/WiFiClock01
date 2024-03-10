/**
*	HTTP �v���g�R��
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

const char	msg_head[] = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><HTML><HEAD><TITLE>KDB-WIFI-CLOCK SETUP</TITLE></HEAD><BODY BGCOLOR=\"#FFFFFFFF\"><CENTER><B>";
const char	msg_footer[] = "<BR><A HREF=\"./\">MAIN MENU</A></CENTER></BODY></HTML>";

/* ----- �g�b�v�y�[�W ----- */
const char	msg_index[] = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><HTML><HEAD><TITLE>KDB-WIFI-CLOCK SETUP</TITLE></HEAD><BODY BGCOLOR=\"#FFFFFFFF\"><CENTER><B>KDB-WiFi-Clock Setup</B><BR><BR><A HREF=\"./setup.cgi?\">BASIC SETUP</A><BR><BR><A HREF=\"./ntpserver.cgi?\">SET NTP SERVER</A><BR><BR><A HREF=\"./timer.cgi?\">SET TIMER</A><BR><BR><A HREF=\"./status.cgi?\">SYSTEM STATUS</A><BR><BR>Kuroneko Dennou Kenkyuushitsu</CENTER></BODY></HTML>";

/* ----- TIME ZONE ----- */
const char	*timezonestr[42] = /* �ݒ�E�\���p */
{
	"UTC+14:00", "UTC+13:00", "UTC+12:45", "UTC+12:00", "UTC+11:30",	/* 00-04 */
	"UTC+11:00", "UTC+10:30", "UTC+10:00", "UTC+09:30", "UTC+09:00",	/* 05-09 */
	"UTC+08:45", "UTC+08:00", "UTC+07:00", "UTC+06:30", "UTC+06:00",	/* 10-14 */
	"UTC+05:45", "UTC+05:30", "UTC+05:00", "UTC+04:30", "UTC+04:00",	/* 15-19 */
	"UTC+03:30", "UTC+03:07", "UTC+03:00", "UTC+02:00", "UTC+01:00",	/* 20-24 */
	"UTC+00:00", "UTC-01:00", "UTC-02:00", "UTC-03:00", "UTC-03:30",	/* 25-29 */
	"UTC-04:00", "UTC-04:30", "UTC-05:00", "UTC-06:00", "UTC-07:00",	/* 30-34 */
	"UTC-08:00", "UTC-09:00", "UTC-09:30", "UTC-10:00", "UTC-11:00",	/* 35-39 */
	"UTC-12:00", "UTC-13:00"						/* 40-41 */
};
const char	*timezonetbl[42] = /* setenv()�p */
{
	"UTC-14:00", "UTC-13:00", "UTC-12:45", "UTC-12:00", "UTC-11:30",	/* 00-04 */
	"UTC-11:00", "UTC-10:30", "UTC-10:00", "UTC-09:30", "UTC-09:00",	/* 05-09 */
	"UTC-08:45", "UTC-08:00", "UTC-07:00", "UTC-06:30", "UTC-06:00",	/* 10-14 */
	"UTC-05:45", "UTC-05:30", "UTC-05:00", "UTC-04:30", "UTC-04:00",	/* 15-19 */
	"UTC-03:30", "UTC-03:07", "UTC-03:00", "UTC-02:00", "UTC-01:00",	/* 20-24 */
	"UTC-00:00", "UTC+01:00", "UTC+02:00", "UTC+03:00", "UTC+03:30",	/* 25-29 */
	"UTC+04:00", "UTC+04:30", "UTC+05:00", "UTC+06:00", "UTC+07:00",	/* 30-34 */
	"UTC+08:00", "UTC+09:00", "UTC+09:30", "UTC+10:00", "UTC+11:00",	/* 35-39 */
	"UTC+12:00", "UTC+13:00"						/* 40-41 */
};

const char	*disphour[2] = 
{
	"12HOUR", "24HOUR"
};

/* ----- 1���ڕ� ----- */
const char	*itemh = "<TR ALIGN=\"CENTER\"><TD>";
const char	*itemm = "</TD><TD>";
const char	*itemf = "</TD></TR>";

const char	*inputf1 = "\"></TD>";
const char	*inputf2 = "\"></TD></TR>";

const char	*selecth = "<SELECT NAME=\"%s\">";
const char	*selectf = "</SELECT>";

const char	*options = "<OPTION VALUE=\"%02d\" SELECTED>%s</OPTION>";
const char	*optionn = "<OPTION VALUE=\"%02d\">%s</OPTION>";

httpd_handle_t	*httpserver;

/* ---------------------------------------- /�̏���(���j���[�\��) ---------------------------------------- */
esp_err_t	index_get_handler( httpd_req_t *req )
{
	httpd_resp_send( req, msg_index, strlen( msg_index ));	/* ���j���[ */

	return	ESP_OK;
}

/* ---------------------------------------- /setup.cgi�̏��� ---------------------------------------- */

/**
*	SELECT�^�O��OPTION�쐬(�^�C���]�[��)
*	 in	buf:�����񂪓���o�b�t�@�|�C���^�擪
*		name:���ږ�
*		sel:�I�����ꂽ�ʒu
*		max:������
*	out	�Ȃ�
*/
void	timezone_select( char *buf, char *name, int sel )
{
	int	i;
	char	str[64];

	sprintf( str, selecth, name );
	strcat( buf, str );
	for( i = 0; i < 42; i++ ){
		if( i == sel ){
			sprintf( str, options, i, timezonestr[i] );
		}
		else{
			sprintf( str, optionn, i, timezonestr[i] );
		}
		strcat( buf, str );
	}
	strcat( buf, selectf );
}

/**
*	SELECT�^�O��OPTION�쐬(�\������)
*	 in	buf:�����񂪓���o�b�t�@�|�C���^�擪
*		name:���ږ�
*		sel:�I�����ꂽ�ʒu
*		max:������
*	out	�Ȃ�
*/
void	disptime_select( char *buf, char *name, int sel )
{
	int	i;
	char	str[64];

	sprintf( str, selecth, name );
	strcat( buf, str );
	for( i = 0; i < 2; i++ ){
		if( i == sel ){
			sprintf( str, options, i, disphour[i] );
		}
		else{
			sprintf( str, optionn, i, disphour[i] );
		}
		strcat( buf, str );
	}
	strcat( buf, selectf );
}

/**
*	setup.cgi�̃y�[�W
*	 in	
*	out	�Ȃ�
*/
void	html_setup( httpd_req_t *req )
{
	char	*buf;
	const char	*html1 = "BASIC SETUP</B><FORM ACTION=\"setup.cgi\" METHOD=\"POST\"><TABLE BORDER=\"1\"><TR><TH WIDTH=\"200\">ITEM</TH><TH WIDTH=\"300\">VALUE</TH><TH WIDTH=\"200\">SET</TH></TR>";
	const char	*html2 = "<FORM ACTION=\"setup.cgi\" METHOD=\"POST\">";

	/* ---------- ���ʉ�� ---------- */
	buf = malloc( 4000 );		/* ���� */
	strcpy( buf, msg_head );	/* �w�b�_ */
	strcat( buf, html1 );
	/* ----- SSID ----- */
	strcat( buf, itemh );
	strcat( buf, "STATION SSID" );
	strcat( buf, itemm );
	strcat( buf, "<INPUT TYPE=\"TEXT\" NAME=\"SSID\" SIZE=\"33\" MAXLENGTH=\"31\" VALUE=\"" );
	strcat( buf, ssid );
	strcat( buf, inputf1 );
	/* -----  ----- */
	strcat( buf, "<TD ROWSPAN=\"2\" ALIGN=\"CENTER\"><INPUT TYPE=\"SUBMIT\" VALUE=\"SET Wi-Fi\"></TD></TR>" );
	/* ----- PASSWORD ----- */
	strcat( buf, itemh );
	strcat( buf, "PASSWORD" );
	strcat( buf, itemm );
	strcat( buf, "<INPUT TYPE=\"TEXT\" NAME=\"PWD\" SIZE=\"33\" MAXLENGTH=\"31\" VALUE=\"" );
	strcat( buf, password );
	strcat( buf, "\">" );
	strcat( buf, itemf );
	strcat( buf, "</FORM>" );

	/* ----- �Œ�A�h���X ----- */
	strcat( buf, html2 );
	/* ----- IP ADDRESS ----- */
	strcat( buf, itemh );
	strcat( buf, "IP ADDRESS" );
	strcat( buf, itemm );
	strcat( buf, "<INPUT TYPE=\"TEXT\" NAME=\"IPADR\" SIZE=\"16\" MAXLENGTH=\"15\" VALUE=\"" );
	strcat( buf, staticip );
	strcat( buf, inputf1 );
	strcat( buf, "<TD ROWSPAN=\"5\" ALIGN=\"CENTER\"><INPUT TYPE=\"SUBMIT\" VALUE=\"SET STATIC\"></TD></TR>" );
	/* ----- NETMASK ----- */
	strcat( buf, itemh );
	strcat( buf, "SUBNET MASK" );
	strcat( buf, itemm );
	strcat( buf, "<INPUT TYPE=\"TEXT\" NAME=\"NMASK\" SIZE=\"16\" MAXLENGTH=\"15\" VALUE=\"" );
	strcat( buf, staticnm );
	strcat( buf, inputf2 );
	/* ----- GATEWAY ----- */
	strcat( buf, itemh );
	strcat( buf, "DEFAULT GATEWAY" );
	strcat( buf, itemm );
	strcat( buf, "<INPUT TYPE=\"TEXT\" NAME=\"GWAY\" SIZE=\"16\" MAXLENGTH=\"15\" VALUE=\"" );
	strcat( buf, staticgw );
	strcat( buf, inputf2 );
	/* ----- DNS MAIN ----- */
	strcat( buf, itemh );
	strcat( buf, "DNS MAIN" );
	strcat( buf, itemm );
	strcat( buf, "<INPUT TYPE=\"TEXT\" NAME=\"DNS1\" SIZE=\"16\" MAXLENGTH=\"15\" VALUE=\"" );
	strcat( buf, staticdns1 );
	strcat( buf, inputf2 );
	/* ----- DNS BACKUP ----- */
	strcat( buf, itemh );
	strcat( buf, "DNS BACKUP" );
	strcat( buf, itemm );
	strcat( buf, "<INPUT TYPE=\"TEXT\" NAME=\"DNS2\" SIZE=\"16\" MAXLENGTH=\"15\" VALUE=\"" );
	strcat( buf, staticdns2 );
	strcat( buf, inputf2 );
	strcat( buf, "</FORM>" );

	strcat( buf, html2 );
	/* ----- TIME ZONE ----- */
	strcat( buf, itemh );
	strcat( buf, "TIME ZONE" );
	strcat( buf, itemm );
	timezone_select( buf, "TZ", timezone );
	strcat( buf, "</TD><TD ROWSPAN=\"2\"><CENTER><INPUT TYPE=\"SUBMIT\" VALUE=\"SET DISP\"></CENTER></TD></TR>" );
	/* ----- DISP TIME ----- */
	strcat( buf, itemh );
	strcat( buf, "DISP TIME" );
	strcat( buf, itemm );
	disptime_select( buf, "DT", tdispmode );
	strcat( buf, itemf );
	
	/* ----- �I�[���� ----- */
	strcat( buf, "</FORM></TABLE>" );
	strcat( buf, msg_footer );	/* �t�b�^ */

	httpd_resp_send( req, buf, strlen( buf ));	/* �ԐM */
	/* -----  ----- */
	free( buf );
}

/* ----- GET ----- */
esp_err_t	setup_get_handler( httpd_req_t *req )
{
	html_setup( req );

	return	ESP_OK;
}

/* ----- SET ----- */
esp_err_t	setup_set_handler( httpd_req_t *req )
{
	esp_err_t	err;

	/* ---------- �ݒ菈�� ---------- */
	err = cgi_setup( req );	
	if( err != ESP_OK ){
		return	err;
	}

	/* ---------- ���ʉ�� ---------- */
	html_setup( req );

	return	ESP_OK;
}

/* ---------------------------------------- /ntpserver.cgi�̏��� ---------------------------------------- */
void	ntpserver_html( httpd_req_t *req )
{
	char	*buf, str[128];

	/* ---------- ���ʉ�� ---------- */
	buf = malloc( 1000 );		/* ���� */
	strcpy( buf, msg_head );	/* �w�b�_ */
	const char	*html1 = "NTP SERVER SETTING</B><FORM ACTION=\"ntpserver.cgi\" METHOD=\"POST\"><TABLE BORDER=\"1\"><TR><TH WIDTH=\"200\">SERVER</TH><TH WIDTH=\"300\">VALUE</TH></TR>";

	strcpy( buf, msg_head );	/* �w�b�_ */
	strcat( buf, html1 );
	/* ----- SERVER1 ----- */
	strcat( buf, itemh );
	strcat( buf, "NTP SERVER1" );
	strcat( buf, itemm );
	strcpy( str, "<INPUT TYPE=\"TEXT\" NAME=\"NS1\" SIZE=\"033\" MAXLENGTH=\"031\" VALUE=\"" );
	strcat( str, ntpserver1 );
	strcat( str, "\">" );
	strcat( buf, str );
	strcat( buf, itemf );
	/* ----- SERVER2 ----- */
	strcat( buf, itemh );
	strcat( buf, "NTP SERVER2" );
	strcat( buf, itemm );
	strcpy( str, "<INPUT TYPE=\"TEXT\" NAME=\"NS2\" SIZE=\"033\" MAXLENGTH=\"031\" VALUE=\"" );
	strcat( str, ntpserver2 );
	strcat( str, "\">" );
	strcat( buf, str );
	strcat( buf, itemf );
	/* ----- SERVER3 ----- */
	strcat( buf, itemh );
	strcat( buf, "NTP SERVER3" );
	strcat( buf, itemm );
	strcpy( str, "<INPUT TYPE=\"TEXT\" NAME=\"NS3\" SIZE=\"033\" MAXLENGTH=\"031\" VALUE=\"" );
	strcat( str, ntpserver3 );
	strcat( str, "\">" );
	strcat( buf, str );
	strcat( buf, itemf );
	/* ----- �I�[���� ----- */
	strcat( buf, "<TR><TD COLSPAN=\"2\"><CENTER><INPUT TYPE=\"SUBMIT\" VALUE=\"APPLY\"></CENTER></TD></TR></TABLE></FORM>" );
	strcat( buf, msg_footer );	/* �t�b�^ */

	httpd_resp_send( req, buf, strlen( buf ));	/* �ԐM */
	/* -----  ----- */
	free( buf );
}

/* ----- GET ----- */
esp_err_t	ntpserver_get_handler( httpd_req_t *req )
{
	ntpserver_html( req );

	return	ESP_OK;
}

/* ----- SET ----- */
esp_err_t	ntpserver_set_handler( httpd_req_t *req )
{
	esp_err_t	err;

	/* ---------- �f�[�^���� ---------- */
	err = cgi_ntpserver( req );
	if( err != ESP_OK ){
		return	err;
	}

	/* ---------- ���ʉ�� ---------- */
	ntpserver_html( req );

	return	ESP_OK;
}

/* ---------------------------------------- /timer�̏��� ---------------------------------------- */
/* ----- �ݒ�p�T�u���[�`�� ----- */
/**
*	SELECT�^�O��OPTION�쐬(�^�C�}���[�h) TD�t��
*	 in	buf:�����񂪓���o�b�t�@�|�C���^�擪
*		name:���ږ�
*		sel:�I�����ꂽ�ʒu
*	out	�Ȃ�
*/
void	selmode( char *buf, char *name, int sel )
{
	char	str[64];

	sprintf( str, "<TD ALIGN=\"CENTER\" ROWSPAN=\"2\"><SELECT NAME=\"%s\" SIZE=\"3\">", name );
	strcat( buf, str );
	if( sel == 0 ){		/* OFF */
		strcat( buf, "<OPTION VALUE=\"0\" SELECTED>OFF</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"0\">OFF</OPTION>" );
	}
	if( sel == 1 ){		/* SINGLE */
		strcat( buf, "<OPTION VALUE=\"1\" SELECTED>SINGLE</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"1\">SINGLE</OPTION>" );
	}
	if( sel == 2 ){		/* CONTINUE */
		strcat( buf, "<OPTION VALUE=\"2\" SELECTED>CONTINUE</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"2\">CONTINUE</OPTION>" );
	}
	strcat( buf, "</SELECT></TD>" );
}

/**
*	SELECT�^�O��OPTION�쐬(�j��) TD�t��
*	 in	buf:�����񂪓���o�b�t�@�|�C���^�擪
*		name:���ږ�
*		sel:�I�����ꂽ�ʒu(�r�b�g)
*	out	�Ȃ�
*/
void	selwday( char *buf, char *name, byte sel )
{
	char	str[64];

	sprintf( str, "<TD><SELECT NAME=\"%s\" SIZE=\"3\" MULTIPLE>", name );
	strcat( buf, str );
	if( sel & 0x01 ){	/* �� */
		strcat( buf, "<OPTION VALUE=\"0\" SELECTED>SUN</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"0\">SUN</OPTION>" );
	}
	if( sel & 0x02 ){	/* �� */
		strcat( buf, "<OPTION VALUE=\"1\" SELECTED>MON</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"1\">MON</OPTION>" );
	}
	if( sel & 0x04 ){	/* �� */
		strcat( buf, "<OPTION VALUE=\"2\" SELECTED>TUE</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"2\">TUE</OPTION>" );
	}
	if( sel & 0x08 ){	/* �� */
		strcat( buf, "<OPTION VALUE=\"3\" SELECTED>WED</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"3\">WED</OPTION>" );
	}
	if( sel & 0x10 ){	/* �� */
		strcat( buf, "<OPTION VALUE=\"4\" SELECTED>THU</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"4\">THU</OPTION>" );
	}
	if( sel & 0x20 ){	/* �� */
		strcat( buf, "<OPTION VALUE=\"5\" SELECTED>FRI</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"5\">FRI</OPTION>" );
	}
	if( sel & 0x40 ){	/* �y */
		strcat( buf, "<OPTION VALUE=\"6\" SELECTED>SAT</OPTION>" );
	}
	else{
		strcat( buf, "<OPTION VALUE=\"6\">SAT</OPTION>" );
	}
	strcat( buf, "</SELECT></TD>" );
}

/**
*	SELECT�^�O��OPTION�쐬(�����b)
*	 in	buf:�����񂪓���o�b�t�@�|�C���^�擪
*		name:���ږ�
*		sel:�I�����ꂽ�ʒu
*		max:������
*	out	�Ȃ�
*/
void	seltime( char *buf, char *name, int sel, int num )
{
	int	i;
	char	str[64];

	sprintf( str, selecth, name );
	strcat( buf, str );
	for( i = 0; i < num; i++ ){
		if( i == sel ){
			sprintf( str, "<OPTION VALUE=\"%02d\" SELECTED>%02d</OPTION>", i, i );
		}
		else{
			sprintf( str, "<OPTION VALUE=\"%02d\">%02d</OPTION>", i, i );
		}
		strcat( buf, str );
	}
	strcat( buf, selectf );
}

/* ���M�{�^�� TD�t�� */
void	submit( char *buf, int num )
{
	char	str[128];

	sprintf( str, "<TD ALIGN=\"CENTER\" ROWSPAN=\"2\"><INPUT TYPE=\"SUBMIT\" VALUE=\"TIMER%d SET\"></TD></TR>", num );
	strcat( buf, str );
}

/* ----- �\���p�T�u���[�`�� ----- */
void	html_timer( httpd_req_t *req )
{
	char	*buf, str[128];
	int	i;
	const char	*timer1 = "TIMER SETTING</B><BR><BR><TABLE BORDER=\"1\"><TR><TH WIDTH=\"90\">TIMER</TH><TH WIDTH=\"110\">MODE</TH><TH WIDTH=\"70\">WDAY</TH><TH WIDTH=\"190\">TIME</TH><TH WIDTH=\"120\">SET</TH></TR>";

	buf = malloc( 41000 );		/* �ԐM�p�o�b�t�@���� */
	strcpy( buf, msg_head );	/* �w�b�_ */
	strcat( buf, timer1 );
	for( i = 0; i < 4; i++ ){
		strcat( buf, "<TR><FORM ACTION=\"timer.cgi\" METHOD=\"POST\">" );
		sprintf( str, "<TD ALIGN=\"CENTER\" ROWSPAN=\"2\">TIMER%d<INPUT TYPE=\"hidden\" NAME=\"TIMER\" VALUE=\"%d\"></TD>", i + 1, i + 1 );
		strcat( buf, str );
		selmode( buf, "TMD", timermode[i] );	/* MODE */
		selwday( buf, "ONW", on_time[i].wday );	/* ON WDAY */
		strcat( buf, "<TD ALIGN=\"CENTER\">ON TIME<BR>" );	/* ON HOUR */
		seltime( buf, "ONH", on_time[i].hour, 24 );
		strcat( buf, ":" );
		seltime( buf, "ONM", on_time[i].min, 60 );	/* ON MINUTE */
		strcat( buf, ":" );
		seltime( buf, "ONS", on_time[i].sec, 60 );	/* ON SECOND */
		strcat( buf, "</TD>" );
		submit( buf, i + 1 );		/* SUBMIT */
		strcat( buf, "<TR>" );		/* 2�i�� */
		selwday( buf, "OFW", off_time[i].wday );	/* OFF WDAY */
		strcat( buf, "<TD ALIGN=\"CENTER\">OFF TIME<BR>" );	/* OFF HOUR */
		seltime( buf, "OFH", off_time[i].hour, 24 );
		strcat( buf, ":" );
		seltime( buf, "OFM", off_time[i].min, 60 );	/* OFF MINUTE */
		strcat( buf, ":" );
		seltime( buf, "OFS", off_time[i].sec, 60 );	/* OFF SECOND */
		strcat( buf, "</TD>" );
		strcat( buf, "</TR></FORM>" );
	}
	/* ----- �I�[���� ----- */
	strcat( buf, "</TABLE>" );
	strcat( buf, msg_footer );	/* �t�b�^ */

	httpd_resp_send( req, buf, strlen( buf ));
}

/* ----- GET ----- */
esp_err_t	timer_get_handler( httpd_req_t *req )
{
	html_timer( req );

	return	ESP_OK;
}

/* ----- SET ----- */
esp_err_t	timer_set_handler( httpd_req_t *req )
{
	esp_err_t	ret;

	/* ---------- �f�[�^���� ---------- */
	ret = cgi_timer( req );
	if( ret != ESP_OK ){
		return	ret;
	}
	/* ---------- ���ʉ�� ---------- */
	html_timer( req );	/* �y�[�W */

	return	ESP_OK;
}

/* ---------------------------------------- /status�̏��� ---------------------------------------- */
esp_err_t	status_get_handler( httpd_req_t *req )
{
	const char	*status1 = "SYSTEM INFOMATION</B><BR><BR><TABLE BORDER=\"1\"><TR><TH WIDTH=\"200\">ITEM</TH><TH WIDTH=\"300\">VALUE</TH></TR>";

	char	*buf, str[32];
	uint8_t	mac[6];

	buf = malloc( 1000 );		/* �ԐM�p�o�b�t�@���� */

	strcpy( buf, msg_head );	/* �w�b�_ */
	strcat( buf, status1 );
	/* ----- IP ADDRESS ----- */
	strcat( buf, itemh );
	strcat( buf, "IP ADDRESS" );
	strcat( buf, itemm );
	sprintf( str, IPSTR, IP2STR( &ip_info.ip ));
	strcat( buf, str );
	strcat( buf, itemf );
	/* ----- NET MASK BIT ----- */
	strcat( buf, itemh );
	strcat( buf, "NET MASK BIT" );
	strcat( buf, itemm );
	sprintf( str, IPSTR, IP2STR( &ip_info.netmask ));
	strcat( buf, str );
	strcat( buf, itemf );
	/* ----- DEFAULT GATEWAY ----- */
	strcat( buf, itemh );
	strcat( buf, "DEFAULT GATEWAY" );
	strcat( buf, itemm );
	sprintf( str, IPSTR, IP2STR( &ip_info.gw ));
	strcat( buf, str );
	strcat( buf, itemf );

	/* ----- MAC ADDRESS ----- */
	if( wifi_mode == WIFI_STATION ){
		esp_wifi_get_mac( ESP_IF_WIFI_STA, mac );
	}
	else{
		esp_wifi_get_mac( ESP_IF_WIFI_AP, mac );
	}
	strcat( buf, itemh );
	strcat( buf, "MAC ADDRESS" );
	strcat( buf, itemm );
	sprintf( str, "" MACSTR "", MAC2STR( mac ));
	strcat( buf, str );
	strcat( buf, itemf );

	/* ----- WIFI MODE ----- */
	strcat( buf, itemh );
	strcat( buf, "WiFi MODE" );
	strcat( buf, itemm );
	if( wifi_mode == WIFI_STATION ){
		strcpy( str, "STATION MODE" );
	}
	else{
		strcpy( str, "ACCESS POINT MODE" );
	}
	strcat( buf, str );
	strcat( buf, itemf );

	/* ----- ADDRESS MODE ----- */
	strcat( buf, itemh );
	strcat( buf, "ADDRESS MODE" );
	strcat( buf, itemm );
	if( addr_mode == IP_DYNAMIC ){
		strcpy( str, "DHCP MODE" );
	}
	else{
		strcpy( str, "STATIC MODE" );
	}
	strcat( buf, str );
	strcat( buf, itemf );

	/* ----- FIRMWARE VERSION ----- */
	strcat( buf, itemh );
	strcat( buf, "FIRMWARE VERSION" );
	strcat( buf, itemm );
	strcat( buf, version );
	strcat( buf, itemf );

	/* ----- �I�[���� ----- */
	strcat( buf, "</TABLE>" );
	strcat( buf, msg_footer );	/* �t�b�^ */

	/* ----- �ԐM ----- */
	httpd_resp_send( req, buf, strlen( buf ));
	free( buf );			/* �J�� */

	return	ESP_OK;
}

/* -------------------- Web�T�[�o�[�̊J�n��URI�̓o�^ -------------------- */
httpd_uri_t	root = {	/* index.htm(�ǂݍ��݂̂�) */
	.uri       = "/",
	.method    = HTTP_GET,
	.handler   = index_get_handler,
	.user_ctx  = NULL
};

httpd_uri_t	setupg = {	/* Wi-Fi�ݒ�(�ǂݍ���) */
	.uri       = "/setup.cgi",
	.method    = HTTP_GET,
	.handler   = setup_get_handler,
	.user_ctx  = NULL
};

httpd_uri_t	setups = {	/* Wi-Fi�ݒ�(�ݒ�) */
	.uri       = "/setup.cgi",
	.method    = HTTP_POST,
	.handler   = setup_set_handler,
	.user_ctx  = NULL
};

httpd_uri_t	ntpserverg = {	/* (�ǂݍ���) */
	.uri       = "/ntpserver.cgi",
	.method    = HTTP_GET,
	.handler   = ntpserver_get_handler,
	.user_ctx  = NULL
};

httpd_uri_t	ntpservers = {	/* (�ݒ�) */
	.uri       = "/ntpserver.cgi",
	.method    = HTTP_POST,
	.handler   = ntpserver_set_handler,
	.user_ctx  = NULL
};

httpd_uri_t	timerg = {	/* (�ǂݍ���) */
	.uri       = "/timer.cgi",
	.method    = HTTP_GET,
	.handler   = timer_get_handler,
	.user_ctx  = NULL
};

httpd_uri_t	timers = {	/* (�ݒ�) */
	.uri       = "/timer.cgi",
	.method    = HTTP_POST,
	.handler   = timer_set_handler,
	.user_ctx  = NULL
};

httpd_uri_t	status = {	/* (�X�e�[�^�X�ǂݍ��݂̂�) */
	.uri       = "/status.cgi",
	.method    = HTTP_GET,
	.handler   = status_get_handler,
	.user_ctx  = NULL
};

/**
*	WEB�T�[�o�[�N��
*	 in	�Ȃ�
*	out	�n���h��
*/
httpd_handle_t	webserver_start( void )
{
	httpd_handle_t	server = NULL;
	httpd_config_t	config = HTTPD_DEFAULT_CONFIG( );
	esp_err_t	sts;

	config.lru_purge_enable = true;
	sts = httpd_start( &server, &config );
	/* ----- URI �o�^ ----- */
	if( sts == ESP_OK ){	
		httpd_register_uri_handler( server, &root );		/* �\��(GET) */
		httpd_register_uri_handler( server, &setupg );		/* �\��(GET) */
		httpd_register_uri_handler( server, &setups );		/* �ݒ�(POST) */
		httpd_register_uri_handler( server, &ntpserverg );	/* �\��(GET) */
		httpd_register_uri_handler( server, &ntpservers );	/* �ݒ�(POST) */
		httpd_register_uri_handler( server, &timerg );		/* �\��(GET) */
		httpd_register_uri_handler( server, &timers );		/* �ݒ�(POST) */
		httpd_register_uri_handler( server, &status );		/* �\��(GET) */
	}
	else{
		server = NULL;
	}

	return	server;
}

/* end of http.c */
