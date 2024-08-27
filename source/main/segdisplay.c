/**
*	SPI�֌W
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

/* ----- 7�Z�O�p�萔 ----- .GFEDCBA */
/* --- 7SEG DATA 0:�_�� ---      0     1     2     3     4     5     6     7     8     9   OFF */
const uint8_t	segdata[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff };
/* --- 7SEG STS 0:�_�� ---      A     V     P     i     U     t */
const uint8_t	segadj[] = { 0x88, 0xE3, 0x8C, 0xFB, 0xC1, 0x87 };

/**
*	���l��7SEG DATA��(unsigned char,�[���T�v���X���Ȃ�)
*	 in	num:�f�[�^
*		buf:�i�[��o�b�t�@
*	out	�Ȃ�
*/
static void	*ucto7seg( byte num, byte *buf )
{
	byte	n, i, j, *p;

	p = buf;
	n = 100;
	for( i = 0; i < 3; i++ ){
		j = (byte)( num / n );
		num %= n;
		*p = segdata[j];
		n /= 10;
		p++;
	}

	return	buf;
}

/**
*	�������A�X�L�[�������(unsigned short,�[���T�v���X���Ȃ�)
*	 in	num:�f�[�^
*		buf:�i�[�敶����
*	out	�Ȃ�
*/
static void	*usto7seg( word num, byte *buf )
{
	word	n;
	byte	i, j, *p;

	p = buf;
	n = 10000U;
	for( i = 0; i < 5; i++ ){
		j = (byte)( num / n );
		num %= n;
		*p = segdata[j];
		n /= 10U;
		p++;
	}

	return	buf;
}

/**
*	7�Z�O����
*/
void	seg7_dispoff( void )
{
	int	i;

	for( i = 0; i < 16; i++ ){
		segbuf[i] = 0xFF;		// OFF:0xFF
	}
	esperr = spi_device_transmit( spi, &trnst );	// ���M
	assert( esperr == ESP_OK );		// Should have had no issues.
}

/**
*	7�Z�O������
*/
void	seg7_initialize( void )
{
	memset( &trnst, 0, sizeof( trnst ));	// �\���̃[���N���A
	trnst.length = 128;			// �]���r�b�g��(8bit*16=128)
	trnst.tx_buffer = segbuf;		// �o�b�t�@�w��
	trnst.user = (void*)1;			// D/C needs to be set to 1

	seg7_dispoff( );			/* ���� */
}

/**
*	7SEG�ɓ��t�����\��
*	 in	�����\���̃|�C���^
*	out	�Ȃ�
*	�r�b�g��0�œ_�� PGFEDCBA
*	���b�`��H�Ŋm��
*	���ʂ����ɑ���A�������ɂ��邱�Ƃɂ��
*	���t�����Ǝ��������̂Q�ʂ�g�������ł���
*/
void	time_display( struct tm *time )
{
	byte	buf[8];

	/* ----- ���t ----- */
	ucto7seg( time->tm_mday, buf );		/* �� */
	segbuf[0] = buf[2];
	segbuf[1] = buf[1];
	ucto7seg( time->tm_mon + 1, buf );	/* �� */
	segbuf[2] = buf[2] & BYTE_MASK_B7;
	segbuf[3] = buf[1];
	usto7seg( time->tm_year + 1900U, buf );	/* �N */
	segbuf[4] = buf[4] & BYTE_MASK_B7;
	segbuf[5] = buf[3];
	segbuf[6] = buf[2];
	segbuf[7] = buf[1];
	/* ----- �X�e�[�^�X ----- */
	segbuf[8] = timerstatus;		/* �^�C�}�X�e�[�^�X */
	/* ----- ���� ----- */
	ucto7seg( time->tm_sec, buf );		/* �b */
	segbuf[9] = buf[2];
	segbuf[10] = buf[1];
	ucto7seg( time->tm_min, buf );		/* �� */
	segbuf[11] = buf[2] & BYTE_MASK_B7;
	segbuf[12] = buf[1];
	if( tdispmode == MODE12H ){	/* 12h */
		ucto7seg( time->tm_hour % 12, buf );	/* �� */
		segbuf[13] = buf[2] & BYTE_MASK_B7;
		segbuf[14] = buf[1];
		if( time->tm_hour < 12 ){	/* AM */
			segbuf[15] = 0x08;
		}
		else{				/* PM */
			segbuf[15] = 0x0C;
		}
	}
	else{			/* 24,28,30h */
		ucto7seg( time->tm_hour, buf );	/* �� */
		segbuf[13] = buf[2] & BYTE_MASK_B7;
		segbuf[14] = buf[1];
		segbuf[15] = 0xff;		/* AM/PM */
	}
	/* ----- �]�� ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEG�Ƀo�[�W�����\��
*	 in	�Ȃ�
*	out	�Ȃ�
*	�r�b�g��0�œ_�� PGFEDCBA
*	���b�`��H�Ŋm��
*/
void	version_display( void )
{
	int	i;

	for( i = 0; i < 16; i++ ){
		segbuf[i] = 0xFF;		// OFF:0xFF
	}
	/* ----- ��i ----- */
	segbuf[7] = 0x8E;			/* F */
	segbuf[6] = 0xFB;			/* i */
	segbuf[5] = 0xAF;			/* r */
	segbuf[4] = 0xC8;			/* m */
	segbuf[3] = 0xD5;			/* W */
	segbuf[2] = 0x88;			/* A */
	segbuf[1] = 0xAF;			/* r */
	segbuf[0] = 0x86;			/* E */
	/* ----- ���i ----- */
	segbuf[14] = 0xE3;			/* v */
	segbuf[13] = 0x86;			/* E */
	segbuf[12] = 0x2F;			/* r. */
	segbuf[11] = segdata[UVER] & 0x7F;	/* V+. */
	segbuf[10] = segdata[MVER];		/* V */
	segbuf[9] =  segdata[LVER];		/* V */
	/* ----- �]�� ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );		// Should have had no issues.
}

/**
*	7SEG��APMODE�\��
*	 in	�Ȃ�
*	out	�Ȃ�
*	�r�b�g��0�œ_�� PGFEDCBA
*/
void	apmode_display( void )
{
	int	i;

	for( i = 0; i < 16; i++ ){
		segbuf[i] = 0xFF;	// OFF:0xFF
	}
	/* ----- ��i ----- */
	segbuf[7] = 0x88;	/* A */
	segbuf[6] = 0x8C;	/* P */
	segbuf[4] = 0xC8;	/* M */
	segbuf[3] = 0xA3;	/* o */
	segbuf[2] = 0xA1;	/* d */
	segbuf[1] = 0x86;	/* E */
	/* ----- ���i ----- */
	segbuf[14] = 0x92;	/* S */
	segbuf[13] = 0x86;	/* E */
	segbuf[12] = 0x87;	/* t */
	segbuf[10] = 0xC1;	/* U */
	segbuf[9] = 0x8C;	/* P */
	/* ----- �]�� ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEG��IP ADDRESS�\��
*	 in	�Ȃ�
*	out	�Ȃ�
*	�r�b�g��0�œ_�� PGFEDCBA
*/
void	ipaddress_display( void )
{
	ulb_t	addr;
	byte	buf[4];

	addr.data = ip_info.ip.addr;

	/* ----- ��i ----- */
	segbuf[0] = 0xCF;		/* I */
	segbuf[1] = 0xFF;
	ucto7seg( addr.bytes.hl, buf );	/* dig2 */
	segbuf[2] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[3] = buf[1];
	segbuf[4] = buf[0];
	ucto7seg( addr.bytes.hh, buf );	/* dig1 */
	segbuf[5] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[6] = buf[1];
	segbuf[7] = buf[0];
	/* ----- ���i ----- */
	segbuf[8] = 0x8C;		/* P */
	segbuf[9] = 0xFF;
	ucto7seg( addr.bytes.ll, buf );	/* dig4 */
	segbuf[10] = buf[2];
	segbuf[11] = buf[1];
	segbuf[12] = buf[0];
	ucto7seg( addr.bytes.lh, buf );	/* dig3 */
	segbuf[13] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[14] = buf[1];
	segbuf[15] = buf[0];
	if( addr_mode == IP_STATIC ){
		segbuf[0] &= BYTE_MASK_B7;
		segbuf[8] &= BYTE_MASK_B7;
	}
	/* ----- �]�� ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEG��NET MASK�\��
*	 in	�Ȃ�
*	out	�Ȃ�
*	�r�b�g��0�œ_�� PGFEDCBA
*/
void	netmask_display( void )
{
	ulb_t	addr;
	byte	buf[4];

	addr.data = ip_info.netmask.addr;

	/* ----- ��i ----- */
	segbuf[0] = 0xAB;		/* n */
	segbuf[1] = 0xFF;
	ucto7seg( addr.bytes.hl, buf );	/* dig2 */
	segbuf[2] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[3] = buf[1];
	segbuf[4] = buf[0];
	ucto7seg( addr.bytes.hh, buf );	/* dig1 */
	segbuf[5] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[6] = buf[1];
	segbuf[7] = buf[0];
	/* ----- ���i ----- */
	segbuf[8] = 0xC8;		/* M */
	segbuf[9] = 0xFF;
	ucto7seg( addr.bytes.ll, buf );	/* dig4 */
	segbuf[10] = buf[2];
	segbuf[11] = buf[1];
	segbuf[12] = buf[0];
	ucto7seg( addr.bytes.lh, buf );	/* dig3 */
	segbuf[13] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[14] = buf[1];
	segbuf[15] = buf[0];

	if( addr_mode == IP_STATIC ){
		segbuf[0] &= BYTE_MASK_B7;
		segbuf[8] &= BYTE_MASK_B7;
	}
	/* ----- �]�� ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEG��GW ADDRESS�\��
*	 in	�Ȃ�
*	out	�Ȃ�
*	�r�b�g��0�œ_�� PGFEDCBA
*/
void	gateway_display( void )
{
	ulb_t	addr;
	byte	buf[4];

	addr.data = ip_info.gw.addr;

	/* ----- ��i ----- */
	segbuf[0] = 0xC2;		/* G */
	segbuf[1] = 0xFF;
	ucto7seg( addr.bytes.hl, buf );	/* dig2 */
	segbuf[2] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[3] = buf[1];
	segbuf[4] = buf[0];
	ucto7seg( addr.bytes.hh, buf );	/* dig1 */
	segbuf[5] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[6] = buf[1];
	segbuf[7] = buf[0];
	/* ----- ���i ----- */
	segbuf[8] = 0xD5;		/* W */
	segbuf[9] = 0xFF;
	ucto7seg( addr.bytes.ll, buf );	/* dig4 */
	segbuf[10] = buf[2];
	segbuf[11] = buf[1];
	segbuf[12] = buf[0];
	ucto7seg( addr.bytes.lh, buf );	/* dig3 */
	segbuf[13] = buf[2] & BYTE_MASK_B7;	/* with . */
	segbuf[14] = buf[1];
	segbuf[15] = buf[0];

	if( addr_mode == IP_STATIC ){
		segbuf[0] &= BYTE_MASK_B7;
		segbuf[8] &= BYTE_MASK_B7;
	}
	/* ----- �]�� ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/* end of segdisplay.c */
