/**
*	�O���[�o���ϐ���`
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/

/* ----- �ݒ�f�[�^ ----- */
_Extern	byte		timezone;	/* �^�C���]�[�� 1 */
_Extern	byte		tdispmode;	/* �\�����ԃ��[�h 0:12h 1:24h 2:28h 3:32h 1 */

_Extern	byte		timermode[4];	/* �^�C�}���[�h 4 */
_Extern	timer_st	on_time[4];	/* �^�C�}�ɂ��ON���� 16 */
_Extern	timer_st	off_time[4];	/* �^�C�}�ɂ��OFF���� 16 */

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

/* ----- ���� ----- */
_Extern	time_t	nowtime;		/* �ŐV���� */
_Extern	struct tm	cmptime;	/* ��r�p�������(28,30���Ԑ��p) */
_Extern	struct tm	timeinfo;	/* ������� */

/* ----- IP�A�h���X ----- */
_Extern	esp_netif_ip_info_t	ip_info;	/* WiFi�Ŏ擾�����A�h���X��� */

_Extern byte	singleon[4];		/* �V���O�����[�hON������ */

/* ----- ���[�h ----- */
_Extern	int	wifi_mode;
_Extern	int	addr_mode;		/* �A�h���X�擾���@ */

/* -----  ----- */
_Extern	uint8_t	timerstatus;

/* ----- �\�� ----- */
_Extern uint8_t	segbuf[16];		/* 7SEG�]���p�o�b�t�@ �N����8���A�����b8�� */
_Extern int	dispenflg;		/* 7SEG�ɓ]�����邩 */
_Extern int	dispflg;		/* 7SEG�ɕ\�����邩 */
_Extern int	modepin;		/* ���샂�[�h�s����� */

/* -----  ----- */
extern const char	*version;

/* end of file */
