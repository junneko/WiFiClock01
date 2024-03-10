/**
*	ÉvÉçÉgÉ^ÉCÉvêÈåæ
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ÇiÇtÇmîL)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
/* ----- spi.c ----- */
void	spi_initialize( void );

/* ----- segdisplay.c ----- */
void	seg7_initialize( void );
void	off7seg_display( void );
void	seg7_display( struct tm *time );
void	version_display( void );
void	apmode_display( void );
void	ipaddress_display( void );
void	netmask_display( void );
void	gateway_display( void );

/* ----- WiFi.c ----- */
void	initialize_wifi_softap( void );
void	initialize_wifi_station( void );

/* ----- sntp.c ----- */
void	sntp_initialize( void );

/* ----- staticip.c ----- */

void	set_static_ip( esp_netif_t *netif );

/* ----- cgi.c ----- */
esp_err_t	cgi_setup( httpd_req_t *req );
esp_err_t	cgi_ntpserver( httpd_req_t *req );
esp_err_t	cgi_timer( httpd_req_t *req );

/* ----- http.c ----- */
httpd_handle_t	webserver_start( void );

/* ----- timer.c ----- */
void	timer_sequence( void );

/* end of prototype.h */
