/**
*	SPI関係
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

/* ----- 7セグ用定数 ----- .GFEDCBA */
/* --- 7SEG DATA 0:点灯 ---      0     1     2     3     4     5     6     7     8     9   OFF */
const uint8_t	segdata[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff };
/* --- 7SEG STS 0:点灯 ---      A     V     P     i     U     t */
const uint8_t	segadj[] = { 0x88, 0xE3, 0x8C, 0xFB, 0xC1, 0x87 };

/**
*	数値を7SEG DATAに(unsigned char,ゼロサプレスしない)
*	 in	num:データ
*		buf:格納先バッファ
*	out	なし
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
*	数字をアスキー文字列に(unsigned short,ゼロサプレスしない)
*	 in	num:データ
*		buf:格納先文字列
*	out	なし
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
*	7セグ消灯
*/
void	seg7_dispoff( void )
{
	int	i;

	for( i = 0; i < 16; i++ ){
		segbuf[i] = 0xFF;		// OFF:0xFF
	}
	esperr = spi_device_transmit( spi, &trnst );	// 送信
	assert( esperr == ESP_OK );		// Should have had no issues.
}

/**
*	7セグ初期化
*/
void	seg7_initialize( void )
{
	memset( &trnst, 0, sizeof( trnst ));	// 構造体ゼロクリア
	trnst.length = 128;			// 転送ビット数(8bit*16=128)
	trnst.tx_buffer = segbuf;		// バッファ指定
	trnst.user = (void*)1;			// D/C needs to be set to 1

	seg7_dispoff( );			/* 消す */
}

/**
*	7SEGに日付時刻表示
*	 in	時刻構造体ポインタ
*	out	なし
*	ビットは0で点灯 PGFEDCBA
*	ラッチはHで確定
*	下位から先に送る、日時を先にすることにより
*	日付時刻と時刻だけの２通り使い方ができる
*/
void	time_display( struct tm *time )
{
	byte	buf[8];

	/* ----- 日付 ----- */
	ucto7seg( time->tm_mday, buf );		/* 日 */
	segbuf[0] = buf[2];
	segbuf[1] = buf[1];
	ucto7seg( time->tm_mon + 1, buf );	/* 月 */
	segbuf[2] = buf[2] & BYTE_MASK_B7;
	segbuf[3] = buf[1];
	usto7seg( time->tm_year + 1900U, buf );	/* 年 */
	segbuf[4] = buf[4] & BYTE_MASK_B7;
	segbuf[5] = buf[3];
	segbuf[6] = buf[2];
	segbuf[7] = buf[1];
	/* ----- ステータス ----- */
	segbuf[8] = timerstatus;		/* タイマステータス */
	/* ----- 時刻 ----- */
	ucto7seg( time->tm_sec, buf );		/* 秒 */
	segbuf[9] = buf[2];
	segbuf[10] = buf[1];
	ucto7seg( time->tm_min, buf );		/* 分 */
	segbuf[11] = buf[2] & BYTE_MASK_B7;
	segbuf[12] = buf[1];
	if( tdispmode == MODE12H ){	/* 12h */
		ucto7seg( time->tm_hour % 12, buf );	/* 時 */
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
		ucto7seg( time->tm_hour, buf );	/* 時 */
		segbuf[13] = buf[2] & BYTE_MASK_B7;
		segbuf[14] = buf[1];
		segbuf[15] = 0xff;		/* AM/PM */
	}
	/* ----- 転送 ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEGにバージョン表示
*	 in	なし
*	out	なし
*	ビットは0で点灯 PGFEDCBA
*	ラッチはHで確定
*/
void	version_display( void )
{
	int	i;

	for( i = 0; i < 16; i++ ){
		segbuf[i] = 0xFF;		// OFF:0xFF
	}
	/* ----- 上段 ----- */
	segbuf[7] = 0x8E;			/* F */
	segbuf[6] = 0xFB;			/* i */
	segbuf[5] = 0xAF;			/* r */
	segbuf[4] = 0xC8;			/* m */
	segbuf[3] = 0xD5;			/* W */
	segbuf[2] = 0x88;			/* A */
	segbuf[1] = 0xAF;			/* r */
	segbuf[0] = 0x86;			/* E */
	/* ----- 下段 ----- */
	segbuf[14] = 0xE3;			/* v */
	segbuf[13] = 0x86;			/* E */
	segbuf[12] = 0x2F;			/* r. */
	segbuf[11] = segdata[UVER] & 0x7F;	/* V+. */
	segbuf[10] = segdata[MVER];		/* V */
	segbuf[9] =  segdata[LVER];		/* V */
	/* ----- 転送 ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );		// Should have had no issues.
}

/**
*	7SEGにAPMODE表示
*	 in	なし
*	out	なし
*	ビットは0で点灯 PGFEDCBA
*/
void	apmode_display( void )
{
	int	i;

	for( i = 0; i < 16; i++ ){
		segbuf[i] = 0xFF;	// OFF:0xFF
	}
	/* ----- 上段 ----- */
	segbuf[7] = 0x88;	/* A */
	segbuf[6] = 0x8C;	/* P */
	segbuf[4] = 0xC8;	/* M */
	segbuf[3] = 0xA3;	/* o */
	segbuf[2] = 0xA1;	/* d */
	segbuf[1] = 0x86;	/* E */
	/* ----- 下段 ----- */
	segbuf[14] = 0x92;	/* S */
	segbuf[13] = 0x86;	/* E */
	segbuf[12] = 0x87;	/* t */
	segbuf[10] = 0xC1;	/* U */
	segbuf[9] = 0x8C;	/* P */
	/* ----- 転送 ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEGにIP ADDRESS表示
*	 in	なし
*	out	なし
*	ビットは0で点灯 PGFEDCBA
*/
void	ipaddress_display( void )
{
	ulb_t	addr;
	byte	buf[4];

	addr.data = ip_info.ip.addr;

	/* ----- 上段 ----- */
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
	/* ----- 下段 ----- */
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
	/* ----- 転送 ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEGにNET MASK表示
*	 in	なし
*	out	なし
*	ビットは0で点灯 PGFEDCBA
*/
void	netmask_display( void )
{
	ulb_t	addr;
	byte	buf[4];

	addr.data = ip_info.netmask.addr;

	/* ----- 上段 ----- */
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
	/* ----- 下段 ----- */
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
	/* ----- 転送 ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/**
*	7SEGにGW ADDRESS表示
*	 in	なし
*	out	なし
*	ビットは0で点灯 PGFEDCBA
*/
void	gateway_display( void )
{
	ulb_t	addr;
	byte	buf[4];

	addr.data = ip_info.gw.addr;

	/* ----- 上段 ----- */
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
	/* ----- 下段 ----- */
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
	/* ----- 転送 ----- */
	esperr = spi_device_transmit( spi, &trnst );	// Transmit!
	assert( esperr == ESP_OK );	// Should have had no issues.
}

/* end of segdisplay.c */
