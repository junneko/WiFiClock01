/**
*	ユーザー定義ファイル
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
/*  ---------------------------------------------- 命名規則 -------------------------------------- 
	・定数(大きさ_種類_数値で構成)
	通常定数
		BYTE_DATA_xxxx	 8bit定数
		WORD_DATA_xxxx	16bit定数
		LONG_DATA_xxxx	32bit定数
		DWORD_DATA_xxxx	32bit定数
	ビット定数
		BYTE_BIT_x	 8bit bitxが1の定数
		WORD_BIT_xx	16bit bitxが1の定数
		LONG_BIT_xx	32bit bitxが1の定数
		DWORD_BIT_xx	32bit bitxが1の定数
	マスクビット定数
		1文字目 R:その1ビットだけ0にする(特殊)
		        L:下位側から変えたくないビット数
		        H:上位側から変えたくないビット数
		2.3文字目 Rは対象のビット,L,Hはビット数
		BYTE_MASK_xx
		WORD_MASK_xxx
		LONG_MASK_xxx
		DWORD_MASK_xxx
	・演算(大きさ_演算単位_数値または命令で構成)
	データ操作(AND,OR,INV) C:CALC
		x:演算する対象(格納先 ex. x | y -> x )
		y:定数または変数
		BYTE_CDAT_xxxx(x)
		WORD_CDAT_xxxx(x)
		LONG_CDAT_xxxx(x)
		DWORD_CDAT_xxxx(x)
		ex. BYTE_CDAT_XOR(x,y) : BYTE(8bit) xにyをxorしてxに入れる
		    BYTE_CDAT_NLINV : BYTE(8bit) ニブル(4bit) L(low側)を INV(反転)
	ビット操作
		x:演算する対象(格納先 ex. x | y -> x )
		y:ビット(0～ビット数-1)
		BYTE_CBIT_xxxx(x)
		WORD_CBIT_xxxx(x)
		LONG_CBIT_xxxx(x)
		DWORD_CBIT_xxxx(x)
    --------------------------------------------------------------------------------------------- */

/* ------------------------------------ 基本定義 ---------------------------- */
#define		USE_C99

/* ---------- 変数型定義 ---------- */
#ifndef		_INTEGER_DEFINED
typedef		long long	int64_t;
typedef		unsigned long long	uint64_t;
//typedef		unsigned char	bool;
typedef		unsigned char	byte;
typedef		unsigned short	word;
typedef		unsigned long	dword;
typedef		uint64_t	qword;
#define		_INTEGER_DEFINED
#endif

/* ---------- 列挙体定義 ---------- */
#ifndef		_BOOL_DEFINED
typedef	enum { off, on }	swbool;
typedef	enum { low, high }	levelbool;
#define		_BOOL_DEFINED
#endif

/* ---------- 制御構造 ---------- */
#define		_loop(n)	{int _i_;for(_i_=0;_i_<(n);_i_++){
#define		_endloop	}}
#define		_repeat		do
#define		_until(n)	while(!(n))	/* for do-while */
#define		_case		break; case
#define		_default	break; default
#define		_or		: case
#define		_forever	for(;;)

/* ---------- 外部定義用 ---------- */
#if defined(_GLOBAL_DEFINED)
#define		_Extern
#else
#define		_Extern		extern
#endif

/* ---------- 関数マクロ ---------- */
#define		_abs(ver)		(((ver) >= 0) ? (ver) : (-(ver)))	/* 絶対値化 */
#define		_invert(x)		(~x)					/* ビット反転 */
#define		_get_byte_low(x)	((byte)((x)&0xff))			/* 下位8bit取り出し */
#define		_get_byte_high(x)	((byte)(((x)>>8)&0xff))			/* 上位8bit取り出し */
#define		_get_word_low(x)	((word)((x)&0xffff))			/* 下位16bit取り出し */
#define		_get_word_high(x)	((word)(((x)>>16)&0xffff))		/* 上位16bit取り出し */
#define		_difftime(t1, t0)	(time_t)((time_t)(t1)-(time_t)(t0))	/* time_tの差 */
#define		_shift_left(x,y)	(x<<y)
#define		_shift_right(x,y)	(x>>y)

/* ------------------------------------ 定数 ---------------------------- */

/* --------- 文字コード定義 --------- */
#define		C_NULL		0x00
#define		C_SOH		0x01
#define		C_STX		0x02
#define		C_ETX		0x03
#define		C_EOT		0x04
#define		C_ENQ		0x05
#define		C_ACK		0x06
#define		C_BEL		0x07
#define		C_BS		0x08
#define		C_HT		0x09
#define		C_LF		0x0A
#define		C_VT		0x0B
#define		C_FF		0x0C
#define		C_CR		0x0D
#define		C_SO		0x0E
#define		C_SI		0x0F
#define		C_DLE		0x10
#define		C_DC1		0x11
#define		C_DC2		0x12
#define		C_DC3		0x13
#define		C_DC4		0x14
#define		C_NAK		0x15
#define		C_SYN		0x16
#define		C_ETB		0x17
#define		C_CAN		0x18
#define		C_EM		0x19
#define		C_SUB		0x1A
#define		C_ESC		0x1B
#define		C_FS		0x1C
#define		C_GS		0x1D
#define		C_RS		0x1E
#define		C_US		0x1F
#define		C_UB		0x7E
#define		C_DEL		0x7F

/* ---------- TRUE/FALSE ---------- */
#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define		OFF		0
#define		ON		1

/* ---------- 最大値 ---------- */
#define		BYTE_DATA_MAX	0xff		/* 8bit */
#define		WORD_DATA_MAX	0xffff		/* 16bit */
#define		DWORD_DATA_MAX	0xffffffff	/* 32bit */
#define		LONG_DATA_MAX	DWORD_DATA_MAX

#ifndef		_BITDEF_DEFINED
/* -------------------------- ビット定数 -------------------------- */

/* ---------- 8bit ---------- */
#define		BYTE_DATA_ZERO		0x00		/* 0 */
#define		BYTE_CDAT_NLINV(x)	x^0x0f		/* bit0-3 反転 */
#define		BYTE_CDAT_NHINV(x)	x^0xf0		/* bit4-7 反転 */
#define		BYTE_CDAT_XOR(x,y)	x^=y		/* XOR */

/* --- ビットn --- */
#define		BYTE_BIT_0		0x01
#define		BYTE_BIT_1		0x02
#define		BYTE_BIT_2		0x04
#define		BYTE_BIT_3		0x08
#define		BYTE_BIT_4		0x10
#define		BYTE_BIT_5		0x20
#define		BYTE_BIT_6		0x40
#define		BYTE_BIT_7		0x80
#define		BYTE_BIT_X(x)		(1<<x)		/* bitx */

/* ---------- 16bit ---------- */

#define		WORD_DATA_ZERO		0x0000		/* 0 */
#define		WORD_CDAT_NLLINV(x)	x^0x000f	/* bit00-03 反転 */
#define		WORD_CDAT_NLHINV(x)	x^0x00f0	/* bit04-07 反転 */
#define		WORD_CDAT_NHLINV(x)	x^0x0f00	/* bit08-11 反転 */
#define		WORD_CDAT_NHHINV(x)	x^0xf000	/* bit12-15 反転 */
#define		WORD_CDAT_XOR(x,y)	x^=y		/* XOR(yの立っている位置のビットを反転) */

/* --- ビットn --- */
#define		WORD_BIT_00		0x0001
#define		WORD_BIT_01		0x0002
#define		WORD_BIT_02		0x0004
#define		WORD_BIT_03		0x0008
#define		WORD_BIT_04		0x0010
#define		WORD_BIT_05		0x0020
#define		WORD_BIT_06		0x0040
#define		WORD_BIT_07		0x0080
#define		WORD_BIT_08		0x0100
#define		WORD_BIT_09		0x0200
#define		WORD_BIT_10		0x0400
#define		WORD_BIT_11		0x0800
#define		WORD_BIT_12		0x1000
#define		WORD_BIT_13		0x2000
#define		WORD_BIT_14		0x4000
#define		WORD_BIT_15		0x8000
#define		WORD_BIT_XX(x)		(1U<<x)

/* ---------- 32bit ---------- */
#define		DWORD_DATA_ZERO		0x00000000	/* 0 */
#define		DWORD_CDAT_NLLLLINV(x)	x^0x0000000f	/* bit00-03 反転 */
#define		DWORD_CDAT_NLLLHINV(x)	x^0x000000f0	/* bit04-07 反転 */
#define		DWORD_CDAT_NLLHLINV(x)	x^0x00000f00	/* bit08-11 反転 */
#define		DWORD_CDAT_NLLHHINV(x)	x^0x0000f000	/* bit12-15 反転 */
#define		DWORD_CDAT_NHHLLINV(x)	x^0x000f0000	/* bit16-19 反転 */
#define		DWORD_CDAT_NHHLHINV(x)	x^0x00f00000	/* bit20-23 反転 */
#define		DWORD_CDAT_NHHHLINV(x)	x^0x0f000000	/* bit24-27 反転 */
#define		DWORD_CDAT_NHHHHINV(x)	x^0xf0000000	/* bit28-32 反転 */
#define		DWORD_CDAT_XOR(x,y)	x^=y		/* XOR(yの立っている位置のビットを反転) */

#define		LONG_DATA_ZERO		DWORD_DATA_ZERO	/* 0 */
#define		LONG_CDAT_NLLLLINV(x)	DWORD_CDAT_NLLLLINV(x)
#define		LONG_CDAT_NLLLHINV(x)	DWORD_CDAT_NLLLHINV(x)
#define		LONG_CDAT_NLLHLINV(x)	DWORD_CDAT_NLLHLINV(x)
#define		LONG_CDAT_NLLHHINV(x)	DWORD_CDAT_NLLHHINV(x)
#define		LONG_CDAT_NHHLLINV(x)	DWORD_CDAT_NHHLLINV(x)
#define		LONG_CDAT_NHHLHINV(x)	DWORD_CDAT_NHHLHINV(x)
#define		LONG_CDAT_NHHHLINV(x)	DWORD_CDAT_NHHHLINV(x)
#define		LONG_CDAT_NHHHHINV(x)	DWORD_CDAT_NHHHHINV(x)
#define		LONG_CDAT_XOR(x,y)	DWORD_CDAT_XOR(x,y)

/* --- ビットn --- */
#define		DWORD_BIT_00		0x00000001
#define		DWORD_BIT_01		0x00000002
#define		DWORD_BIT_02		0x00000004
#define		DWORD_BIT_03		0x00000008
#define		DWORD_BIT_04		0x00000010
#define		DWORD_BIT_05		0x00000020
#define		DWORD_BIT_06		0x00000040
#define		DWORD_BIT_07		0x00000080
#define		DWORD_BIT_08		0x00000100
#define		DWORD_BIT_09		0x00000200
#define		DWORD_BIT_10		0x00000400
#define		DWORD_BIT_11		0x00000800
#define		DWORD_BIT_12		0x00001000
#define		DWORD_BIT_13		0x00002000
#define		DWORD_BIT_14		0x00004000
#define		DWORD_BIT_15		0x00008000
#define		DWORD_BIT_16		0x00010000
#define		DWORD_BIT_17		0x00020000
#define		DWORD_BIT_18		0x00040000
#define		DWORD_BIT_19		0x00080000
#define		DWORD_BIT_20		0x00100000
#define		DWORD_BIT_21		0x00200000
#define		DWORD_BIT_22		0x00400000
#define		DWORD_BIT_23		0x00800000
#define		DWORD_BIT_24		0x01000000
#define		DWORD_BIT_25		0x02000000
#define		DWORD_BIT_26		0x04000000
#define		DWORD_BIT_27		0x08000000
#define		DWORD_BIT_28		0x10000000
#define		DWORD_BIT_29		0x20000000
#define		DWORD_BIT_30		0x40000000
#define		DWORD_BIT_31		0x80000000
#define		DWORD_BIT_XX(x)		(1UL<<x)

#define		LONG_BIT_00		DWORD_BIT_00
#define		LONG_BIT_01		DWORD_BIT_01
#define		LONG_BIT_02		DWORD_BIT_02
#define		LONG_BIT_03		DWORD_BIT_03
#define		LONG_BIT_04		DWORD_BIT_04
#define		LONG_BIT_05		DWORD_BIT_05
#define		LONG_BIT_06		DWORD_BIT_06
#define		LONG_BIT_07		DWORD_BIT_07
#define		LONG_BIT_08		DWORD_BIT_08
#define		LONG_BIT_09		DWORD_BIT_09
#define		LONG_BIT_10		DWORD_BIT_10
#define		LONG_BIT_11		DWORD_BIT_11
#define		LONG_BIT_12		DWORD_BIT_12
#define		LONG_BIT_13		DWORD_BIT_13
#define		LONG_BIT_14		DWORD_BIT_14
#define		LONG_BIT_15		DWORD_BIT_15
#define		LONG_BIT_16		DWORD_BIT_16
#define		LONG_BIT_17		DWORD_BIT_17
#define		LONG_BIT_18		DWORD_BIT_18
#define		LONG_BIT_19		DWORD_BIT_19
#define		LONG_BIT_20		DWORD_BIT_20
#define		LONG_BIT_21		DWORD_BIT_21
#define		LONG_BIT_22		DWORD_BIT_22
#define		LONG_BIT_23		DWORD_BIT_23
#define		LONG_BIT_24		DWORD_BIT_24
#define		LONG_BIT_25		DWORD_BIT_25
#define		LONG_BIT_26		DWORD_BIT_26
#define		LONG_BIT_27		DWORD_BIT_27
#define		LONG_BIT_28		DWORD_BIT_28
#define		LONG_BIT_29		DWORD_BIT_29
#define		LONG_BIT_30		DWORD_BIT_30
#define		LONG_BIT_31		DWORD_BIT_31
#define		LONG_BIT_XX(x)		DWORD_BIT_XX(x)

/* -------------------------- ビットマスク定数 -------------------------- */

/* ----- 8bit bitnのマスク ----- */
#define		BYTE_MASK_B0		0xfe
#define		BYTE_MASK_B1		0xfd
#define		BYTE_MASK_B2		0xfb
#define		BYTE_MASK_B3		0xf7
#define		BYTE_MASK_B4		0xef
#define		BYTE_MASK_B5		0xdf
#define		BYTE_MASK_B6		0xbf
#define		BYTE_MASK_B7		0x7f
#define		BYTE_MASK_BX(x)		_invert(BYTE_BIT_X(x))	/* bitxのマスク */

/* ----- 8bit 下位からxbitマスク ----- */
#define		BYTE_MASK_L1		0x01
#define		BYTE_MASK_L2		0x03
#define		BYTE_MASK_L3		0x07
#define		BYTE_MASK_L4		0x0f
#define		BYTE_MASK_L5		0x1f
#define		BYTE_MASK_L6		0x3f
#define		BYTE_MASK_L7		0x7f

/* ----- 8bit 上位からxbitマスク ----- */
#define		BYTE_MASK_U1		0x80
#define		BYTE_MASK_U2		0xc0
#define		BYTE_MASK_U3		0xe0
#define		BYTE_MASK_U4		0xf0
#define		BYTE_MASK_U5		0xf8
#define		BYTE_MASK_U6		0xfc
#define		BYTE_MASK_U7		0xfe

/* ----- 16bit bitnのマスク ----- */
#define		WORD_MASK_B00		0xfffe
#define		WORD_MASK_B01		0xfffd
#define		WORD_MASK_B02		0xfffb
#define		WORD_MASK_B03		0xfff7
#define		WORD_MASK_B04		0xffef
#define		WORD_MASK_B05		0xffdf
#define		WORD_MASK_B06		0xffbf
#define		WORD_MASK_B07		0xff7f
#define		WORD_MASK_B08		0xfeff
#define		WORD_MASK_B09		0xfdff
#define		WORD_MASK_B10		0xfbff
#define		WORD_MASK_B11		0xf7ff
#define		WORD_MASK_B12		0xefff
#define		WORD_MASK_B13		0xdfff
#define		WORD_MASK_B14		0xbfff
#define		WORD_MASK_B15		0x7fff
#define		WORD_MASK_BXX(x)	_invert(WORD_BIT_XX(x))

/* ----- 16bit 下位からxbitマスク ----- */
#define		WORD_MASK_L01		0x0001
#define		WORD_MASK_L02		0x0003
#define		WORD_MASK_L03		0x0007
#define		WORD_MASK_L04		0x000f
#define		WORD_MASK_L05		0x001f
#define		WORD_MASK_L06		0x003f
#define		WORD_MASK_L07		0x007f
#define		WORD_MASK_L08		0x00ff
#define		WORD_MASK_L09		0x01ff
#define		WORD_MASK_L10		0x03ff
#define		WORD_MASK_L11		0x07ff
#define		WORD_MASK_L12		0x0fff
#define		WORD_MASK_L13		0x1fff
#define		WORD_MASK_L14		0x3fff
#define		WORD_MASK_L15		0x7fff

/* ----- 16bit 上位からxbitマスク ----- */
#define		WORD_MASK_U01		0x8000
#define		WORD_MASK_U02		0xc000
#define		WORD_MASK_U03		0xe000
#define		WORD_MASK_U04		0xf000
#define		WORD_MASK_U05		0xf800
#define		WORD_MASK_U06		0xfc00
#define		WORD_MASK_U07		0xfe00
#define		WORD_MASK_U08		0xff00
#define		WORD_MASK_U09		0xff80
#define		WORD_MASK_U10		0xffc0
#define		WORD_MASK_U11		0xffe0
#define		WORD_MASK_U12		0xfff0
#define		WORD_MASK_U13		0xfff8
#define		WORD_MASK_U14		0xfffc
#define		WORD_MASK_U15		0xfffe

/* ----- 32bit bitnのマスク ----- */
#define		DWORD_MASK_B00		0xfffffffe
#define		DWORD_MASK_B01		0xfffffffd
#define		DWORD_MASK_B02		0xfffffffb
#define		DWORD_MASK_B03		0xfffffff7
#define		DWORD_MASK_B04		0xffffffef
#define		DWORD_MASK_B05		0xffffffdf
#define		DWORD_MASK_B06		0xffffffbf
#define		DWORD_MASK_B07		0xffffff7f
#define		DWORD_MASK_B08		0xfffffeff
#define		DWORD_MASK_B09		0xfffffdff
#define		DWORD_MASK_B10		0xfffffbff
#define		DWORD_MASK_B11		0xfffff7ff
#define		DWORD_MASK_B12		0xffffefff
#define		DWORD_MASK_B13		0xffffdfff
#define		DWORD_MASK_B14		0xffffbfff
#define		DWORD_MASK_B15		0xffff7fff
#define		DWORD_MASK_B16		0xfffeffff
#define		DWORD_MASK_B17		0xfffdffff
#define		DWORD_MASK_B18		0xfffbffff
#define		DWORD_MASK_B19		0xfff7ffff
#define		DWORD_MASK_B20		0xffefffff
#define		DWORD_MASK_B21		0xffdfffff
#define		DWORD_MASK_B22		0xffbfffff
#define		DWORD_MASK_B23		0xff7fffff
#define		DWORD_MASK_B24		0xfeffffff
#define		DWORD_MASK_B25		0xfdffffff
#define		DWORD_MASK_B26		0xfbffffff
#define		DWORD_MASK_B27		0xf7ffffff
#define		DWORD_MASK_B28		0xefffffff
#define		DWORD_MASK_B29		0xdfffffff
#define		DWORD_MASK_B30		0xbfffffff
#define		DWORD_MASK_B31		0x7fffffff
#define		DWORD_MASK_BXX(x)	_invert(DWORD_BIT_XX(x))

#define		LONG_MASK_B00		DWORD_MASK_R00
#define		LONG_MASK_B01		DWORD_MASK_R01
#define		LONG_MASK_B02		DWORD_MASK_R02
#define		LONG_MASK_B03		DWORD_MASK_R03
#define		LONG_MASK_B04		DWORD_MASK_R04
#define		LONG_MASK_B05		DWORD_MASK_R05
#define		LONG_MASK_B06		DWORD_MASK_R06
#define		LONG_MASK_B07		DWORD_MASK_R07
#define		LONG_MASK_B08		DWORD_MASK_R08
#define		LONG_MASK_B09		DWORD_MASK_R09
#define		LONG_MASK_B10		DWORD_MASK_R10
#define		LONG_MASK_B11		DWORD_MASK_R11
#define		LONG_MASK_B12		DWORD_MASK_R12
#define		LONG_MASK_B13		DWORD_MASK_R13
#define		LONG_MASK_B14		DWORD_MASK_R14
#define		LONG_MASK_B15		DWORD_MASK_R15
#define		LONG_MASK_B16		DWORD_MASK_R16
#define		LONG_MASK_B17		DWORD_MASK_R17
#define		LONG_MASK_B18		DWORD_MASK_R18
#define		LONG_MASK_B19		DWORD_MASK_R19
#define		LONG_MASK_B20		DWORD_MASK_R20
#define		LONG_MASK_B21		DWORD_MASK_R21
#define		LONG_MASK_B22		DWORD_MASK_R22
#define		LONG_MASK_B23		DWORD_MASK_R23
#define		LONG_MASK_B24		DWORD_MASK_R24
#define		LONG_MASK_B25		DWORD_MASK_R25
#define		LONG_MASK_B26		DWORD_MASK_R26
#define		LONG_MASK_B27		DWORD_MASK_R27
#define		LONG_MASK_B28		DWORD_MASK_R28
#define		LONG_MASK_B29		DWORD_MASK_R29
#define		LONG_MASK_B30		DWORD_MASK_R30
#define		LONG_MASK_B31		DWORD_MASK_R31
#define		LONG_MASK_BXX(x)	DWORD_MASK_RXX(x)

/* ----- 32bit 下位からxbitマスク ----- */
#define		DWORD_MASK_L01		0x00000001
#define		DWORD_MASK_L02		0x00000003
#define		DWORD_MASK_L03		0x00000007
#define		DWORD_MASK_L04		0x0000000f
#define		DWORD_MASK_L05		0x0000001f
#define		DWORD_MASK_L06		0x0000003f
#define		DWORD_MASK_L07		0x0000007f
#define		DWORD_MASK_L08		0x000000ff
#define		DWORD_MASK_L09		0x000001ff
#define		DWORD_MASK_L10		0x000003ff
#define		DWORD_MASK_L11		0x000007ff
#define		DWORD_MASK_L12		0x00000fff
#define		DWORD_MASK_L13		0x00001fff
#define		DWORD_MASK_L14		0x00003fff
#define		DWORD_MASK_L15		0x00007fff
#define		DWORD_MASK_L16		0x0000ffff
#define		DWORD_MASK_L17		0x0001ffff
#define		DWORD_MASK_L18		0x0003ffff
#define		DWORD_MASK_L19		0x0007ffff
#define		DWORD_MASK_L20		0x000fffff
#define		DWORD_MASK_L21		0x001fffff
#define		DWORD_MASK_L22		0x003fffff
#define		DWORD_MASK_L23		0x007fffff
#define		DWORD_MASK_L24		0x00ffffff
#define		DWORD_MASK_L25		0x01ffffff
#define		DWORD_MASK_L26		0x03ffffff
#define		DWORD_MASK_L27		0x07ffffff
#define		DWORD_MASK_L28		0x0fffffff
#define		DWORD_MASK_L29		0x1fffffff
#define		DWORD_MASK_L30		0x3fffffff
#define		DWORD_MASK_L31		0x7fffffff

#define		LONG_MASK_L01		DWORD_MASK_L01
#define		LONG_MASK_L02		DWORD_MASK_L02
#define		LONG_MASK_L03		DWORD_MASK_L03
#define		LONG_MASK_L04		DWORD_MASK_L04
#define		LONG_MASK_L05		DWORD_MASK_L05
#define		LONG_MASK_L06		DWORD_MASK_L06
#define		LONG_MASK_L07		DWORD_MASK_L07
#define		LONG_MASK_L08		DWORD_MASK_L08
#define		LONG_MASK_L09		DWORD_MASK_L09
#define		LONG_MASK_L10		DWORD_MASK_L10
#define		LONG_MASK_L11		DWORD_MASK_L11
#define		LONG_MASK_L12		DWORD_MASK_L12
#define		LONG_MASK_L13		DWORD_MASK_L13
#define		LONG_MASK_L14		DWORD_MASK_L14
#define		LONG_MASK_L15		DWORD_MASK_L15
#define		LONG_MASK_L16		DWORD_MASK_L16
#define		LONG_MASK_L17		DWORD_MASK_L17
#define		LONG_MASK_L18		DWORD_MASK_L18
#define		LONG_MASK_L19		DWORD_MASK_L19
#define		LONG_MASK_L20		DWORD_MASK_L20
#define		LONG_MASK_L21		DWORD_MASK_L21
#define		LONG_MASK_L22		DWORD_MASK_L22
#define		LONG_MASK_L23		DWORD_MASK_L23
#define		LONG_MASK_L24		DWORD_MASK_L24
#define		LONG_MASK_L25		DWORD_MASK_L25
#define		LONG_MASK_L26		DWORD_MASK_L26
#define		LONG_MASK_L27		DWORD_MASK_L27
#define		LONG_MASK_L28		DWORD_MASK_L28
#define		LONG_MASK_L29		DWORD_MASK_L29
#define		LONG_MASK_L30		DWORD_MASK_L30
#define		LONG_MASK_L31		DWORD_MASK_L31

/* ----- 32bit 上位からxbitマスク ----- */
#define		DWORD_MASK_U01		0x80000000
#define		DWORD_MASK_U02		0xc0000000
#define		DWORD_MASK_U03		0xe0000000
#define		DWORD_MASK_U04		0xf0000000
#define		DWORD_MASK_U05		0xf8000000
#define		DWORD_MASK_U06		0xfc000000
#define		DWORD_MASK_U07		0xfe000000
#define		DWORD_MASK_U08		0xff000000
#define		DWORD_MASK_U09		0xff800000
#define		DWORD_MASK_U10		0xffc00000
#define		DWORD_MASK_U11		0xffe00000
#define		DWORD_MASK_U12		0xfff00000
#define		DWORD_MASK_U13		0xfff80000
#define		DWORD_MASK_U14		0xfffc0000
#define		DWORD_MASK_U15		0xfffe0000
#define		DWORD_MASK_U16		0xffff0000
#define		DWORD_MASK_U17		0xffff8000
#define		DWORD_MASK_U18		0xffffc000
#define		DWORD_MASK_U19		0xffffe000
#define		DWORD_MASK_U20		0xfffff000
#define		DWORD_MASK_U21		0xfffff800
#define		DWORD_MASK_U22		0xfffffc00
#define		DWORD_MASK_U23		0xfffffe00
#define		DWORD_MASK_U24		0xffffff00
#define		DWORD_MASK_U25		0xffffff80
#define		DWORD_MASK_U26		0xffffffc0
#define		DWORD_MASK_U27		0xffffffe0
#define		DWORD_MASK_U28		0xfffffff0
#define		DWORD_MASK_U29		0xfffffff8
#define		DWORD_MASK_U30		0xfffffffc
#define		DWORD_MASK_U31		0xfffffffe

#define		LONG_MASK_U01		DWORD_MASK_U01
#define		LONG_MASK_U02		DWORD_MASK_U02
#define		LONG_MASK_U03		DWORD_MASK_U03
#define		LONG_MASK_U04		DWORD_MASK_U04
#define		LONG_MASK_U05		DWORD_MASK_U05
#define		LONG_MASK_U06		DWORD_MASK_U06
#define		LONG_MASK_U07		DWORD_MASK_U07
#define		LONG_MASK_U08		DWORD_MASK_U08
#define		LONG_MASK_U09		DWORD_MASK_U09
#define		LONG_MASK_U10		DWORD_MASK_U10
#define		LONG_MASK_U11		DWORD_MASK_U11
#define		LONG_MASK_U12		DWORD_MASK_U12
#define		LONG_MASK_U13		DWORD_MASK_U13
#define		LONG_MASK_U14		DWORD_MASK_U14
#define		LONG_MASK_U15		DWORD_MASK_U15
#define		LONG_MASK_U16		DWORD_MASK_U16
#define		LONG_MASK_U17		DWORD_MASK_U17
#define		LONG_MASK_U18		DWORD_MASK_U18
#define		LONG_MASK_U19		DWORD_MASK_U19
#define		LONG_MASK_U20		DWORD_MASK_U20
#define		LONG_MASK_U21		DWORD_MASK_U21
#define		LONG_MASK_U22		DWORD_MASK_U22
#define		LONG_MASK_U23		DWORD_MASK_U23
#define		LONG_MASK_U24		DWORD_MASK_U24
#define		LONG_MASK_U25		DWORD_MASK_U25
#define		LONG_MASK_U26		DWORD_MASK_U26
#define		LONG_MASK_U27		DWORD_MASK_U27
#define		LONG_MASK_U28		DWORD_MASK_U28
#define		LONG_MASK_U29		DWORD_MASK_U29
#define		LONG_MASK_U30		DWORD_MASK_U30
#define		LONG_MASK_U31		DWORD_MASK_U31

/* -------------------- ビット制御 -------------------- */

/* --- 8bit xのビットnを反転する --- */
#define		BYTE_BIT0_INV(x)	x^=BYTE_BIT_0
#define		BYTE_BIT1_INV(x)	x^=BYTE_BIT_1
#define		BYTE_BIT2_INV(x)	x^=BYTE_BIT_2
#define		BYTE_BIT3_INV(x)	x^=BYTE_BIT_3
#define		BYTE_BIT4_INV(x)	x^=BYTE_BIT_4
#define		BYTE_BIT5_INV(x)	x^=BYTE_BIT_5
#define		BYTE_BIT6_INV(x)	x^=BYTE_BIT_6
#define		BYTE_BIT7_INV(x)	x^=BYTE_BIT_7
#define		BYTE_BITX_INV(x,y)	x^=BYTE_BIT_X(y)

/* --- 8bit xのビットnを1にする --- */
#define		BYTE_BIT0_ON(x)		x|=BYTE_BIT_0
#define		BYTE_BIT1_ON(x)		x|=BYTE_BIT_1
#define		BYTE_BIT2_ON(x)		x|=BYTE_BIT_2
#define		BYTE_BIT3_ON(x)		x|=BYTE_BIT_3
#define		BYTE_BIT4_ON(x)		x|=BYTE_BIT_4
#define		BYTE_BIT5_ON(x)		x|=BYTE_BIT_5
#define		BYTE_BIT6_ON(x)		x|=BYTE_BIT_6
#define		BYTE_BIT7_ON(x)		x|=BYTE_BIT_7
#define		BYTE_BITX_ON(x,y)	x|=BYTE_BIT_X(y)

/* --- 8bit xのビットnを0にする --- */
#define		BYTE_BIT0_OFF(x)	x&=BYTE_MASK_B0
#define		BYTE_BIT1_OFF(x)	x&=BYTE_MASK_B1
#define		BYTE_BIT2_OFF(x)	x&=BYTE_MASK_B2
#define		BYTE_BIT3_OFF(x)	x&=BYTE_MASK_B3
#define		BYTE_BIT4_OFF(x)	x&=BYTE_MASK_B4
#define		BYTE_BIT5_OFF(x)	x&=BYTE_MASK_B5
#define		BYTE_BIT6_OFF(x)	x&=BYTE_MASK_B6
#define		BYTE_BIT7_OFF(x)	x&=BYTE_MASK_B7
#define		BYTE_BITX_OFF(x,y)	x&=BYTE_MASK_BX(x)

/* --- 16bit xのビットnを反転する --- */
#define		WORD_BIT00_INV(x)	x^=WORD_BIT_00
#define		WORD_BIT01_INV(x)	x^=WORD_BIT_01
#define		WORD_BIT02_INV(x)	x^=WORD_BIT_02
#define		WORD_BIT03_INV(x)	x^=WORD_BIT_03
#define		WORD_BIT04_INV(x)	x^=WORD_BIT_04
#define		WORD_BIT05_INV(x)	x^=WORD_BIT_05
#define		WORD_BIT06_INV(x)	x^=WORD_BIT_06
#define		WORD_BIT07_INV(x)	x^=WORD_BIT_07
#define		WORD_BIT08_INV(x)	x^=WORD_BIT_08
#define		WORD_BIT09_INV(x)	x^=WORD_BIT_09
#define		WORD_BIT10_INV(x)	x^=WORD_BIT_10
#define		WORD_BIT11_INV(x)	x^=WORD_BIT_11
#define		WORD_BIT12_INV(x)	x^=WORD_BIT_12
#define		WORD_BIT13_INV(x)	x^=WORD_BIT_13
#define		WORD_BIT14_INV(x)	x^=WORD_BIT_14
#define		WORD_BIT15_INV(x)	x^=WORD_BIT_15
#define		WORD_BITXX_INV(x,y)	x^=WORD_BIT_XX(y)

/* --- 16bit xのビットnを1にする --- */
#define		WORD_BIT00_ON(x)	x|=WORD_BIT_00
#define		WORD_BIT01_ON(x)	x|=WORD_BIT_01
#define		WORD_BIT02_ON(x)	x|=WORD_BIT_02
#define		WORD_BIT03_ON(x)	x|=WORD_BIT_03
#define		WORD_BIT04_ON(x)	x|=WORD_BIT_04
#define		WORD_BIT05_ON(x)	x|=WORD_BIT_05
#define		WORD_BIT06_ON(x)	x|=WORD_BIT_06
#define		WORD_BIT07_ON(x)	x|=WORD_BIT_07
#define		WORD_BIT08_ON(x)	x|=WORD_BIT_08
#define		WORD_BIT09_ON(x)	x|=WORD_BIT_09
#define		WORD_BIT10_ON(x)	x|=WORD_BIT_10
#define		WORD_BIT11_ON(x)	x|=WORD_BIT_11
#define		WORD_BIT12_ON(x)	x|=WORD_BIT_12
#define		WORD_BIT13_ON(x)	x|=WORD_BIT_13
#define		WORD_BIT14_ON(x)	x|=WORD_BIT_14
#define		WORD_BIT15_ON(x)	x|=WORD_BIT_15
#define		WORD_BITXX_ON(x,y)	x|=WORD_BIT_XX(y)

/* --- 16bit xのビットnを0にする --- */
#define		WORD_BIT00_OFF(x)	x&=WORD_MASK_R00
#define		WORD_BIT01_OFF(x)	x&=WORD_MASK_R01
#define		WORD_BIT02_OFF(x)	x&=WORD_MASK_R02
#define		WORD_BIT03_OFF(x)	x&=WORD_MASK_R03
#define		WORD_BIT04_OFF(x)	x&=WORD_MASK_R04
#define		WORD_BIT05_OFF(x)	x&=WORD_MASK_R05
#define		WORD_BIT06_OFF(x)	x&=WORD_MASK_R06
#define		WORD_BIT07_OFF(x)	x&=WORD_MASK_R07
#define		WORD_BIT08_OFF(x)	x&=WORD_MASK_R08
#define		WORD_BIT09_OFF(x)	x&=WORD_MASK_R09
#define		WORD_BIT10_OFF(x)	x&=WORD_MASK_R10
#define		WORD_BIT11_OFF(x)	x&=WORD_MASK_R11
#define		WORD_BIT12_OFF(x)	x&=WORD_MASK_R12
#define		WORD_BIT13_OFF(x)	x&=WORD_MASK_R13
#define		WORD_BIT14_OFF(x)	x&=WORD_MASK_R14
#define		WORD_BIT15_OFF(x)	x&=WORD_MASK_R15
#define		WORD_BITXX_OFF(x,y)	x&=WORD_MASK_RXX(y)

/* --- 32bit xのビットnを反転する --- */
#define		DWORD_BIT00_INV(x)	x^=DWORD_BIT_00
#define		DWORD_BIT01_INV(x)	x^=DWORD_BIT_01
#define		DWORD_BIT02_INV(x)	x^=DWORD_BIT_02
#define		DWORD_BIT03_INV(x)	x^=DWORD_BIT_03
#define		DWORD_BIT04_INV(x)	x^=DWORD_BIT_04
#define		DWORD_BIT05_INV(x)	x^=DWORD_BIT_05
#define		DWORD_BIT06_INV(x)	x^=DWORD_BIT_06
#define		DWORD_BIT07_INV(x)	x^=DWORD_BIT_07
#define		DWORD_BIT08_INV(x)	x^=DWORD_BIT_08
#define		DWORD_BIT09_INV(x)	x^=DWORD_BIT_09
#define		DWORD_BIT10_INV(x)	x^=DWORD_BIT_10
#define		DWORD_BIT11_INV(x)	x^=DWORD_BIT_11
#define		DWORD_BIT12_INV(x)	x^=DWORD_BIT_12
#define		DWORD_BIT13_INV(x)	x^=DWORD_BIT_13
#define		DWORD_BIT14_INV(x)	x^=DWORD_BIT_14
#define		DWORD_BIT15_INV(x)	x^=DWORD_BIT_15
#define		DWORD_BIT16_INV(x)	x^=DWORD_BIT_16
#define		DWORD_BIT17_INV(x)	x^=DWORD_BIT_17
#define		DWORD_BIT18_INV(x)	x^=DWORD_BIT_18
#define		DWORD_BIT19_INV(x)	x^=DWORD_BIT_19
#define		DWORD_BIT20_INV(x)	x^=DWORD_BIT_20
#define		DWORD_BIT21_INV(x)	x^=DWORD_BIT_21
#define		DWORD_BIT22_INV(x)	x^=DWORD_BIT_22
#define		DWORD_BIT23_INV(x)	x^=DWORD_BIT_23
#define		DWORD_BIT24_INV(x)	x^=DWORD_BIT_24
#define		DWORD_BIT25_INV(x)	x^=DWORD_BIT_25
#define		DWORD_BIT26_INV(x)	x^=DWORD_BIT_26
#define		DWORD_BIT27_INV(x)	x^=DWORD_BIT_27
#define		DWORD_BIT28_INV(x)	x^=DWORD_BIT_28
#define		DWORD_BIT29_INV(x)	x^=DWORD_BIT_29
#define		DWORD_BIT30_INV(x)	x^=DWORD_BIT_30
#define		DWORD_BIT31_INV(x)	x^=DWORD_BIT_31
#define		DWORD_BITXX_INV(x,y)	x^=DWORD_BIT_XX(y)

#define		LONG_BIT00_INV(x)	DWORD_BIT00_INV(x)
#define		LONG_BIT01_INV(x)	DWORD_BIT01_INV(x)
#define		LONG_BIT02_INV(x)	DWORD_BIT02_INV(x)
#define		LONG_BIT03_INV(x)	DWORD_BIT03_INV(x)
#define		LONG_BIT04_INV(x)	DWORD_BIT04_INV(x)
#define		LONG_BIT05_INV(x)	DWORD_BIT05_INV(x)
#define		LONG_BIT06_INV(x)	DWORD_BIT06_INV(x)
#define		LONG_BIT07_INV(x)	DWORD_BIT07_INV(x)
#define		LONG_BIT08_INV(x)	DWORD_BIT08_INV(x)
#define		LONG_BIT09_INV(x)	DWORD_BIT09_INV(x)
#define		LONG_BIT10_INV(x)	DWORD_BIT10_INV(x)
#define		LONG_BIT11_INV(x)	DWORD_BIT11_INV(x)
#define		LONG_BIT12_INV(x)	DWORD_BIT12_INV(x)
#define		LONG_BIT13_INV(x)	DWORD_BIT13_INV(x)
#define		LONG_BIT14_INV(x)	DWORD_BIT14_INV(x)
#define		LONG_BIT15_INV(x)	DWORD_BIT15_INV(x)
#define		LONG_BIT16_INV(x)	DWORD_BIT16_INV(x)
#define		LONG_BIT17_INV(x)	DWORD_BIT17_INV(x)
#define		LONG_BIT18_INV(x)	DWORD_BIT18_INV(x)
#define		LONG_BIT19_INV(x)	DWORD_BIT19_INV(x)
#define		LONG_BIT20_INV(x)	DWORD_BIT20_INV(x)
#define		LONG_BIT21_INV(x)	DWORD_BIT21_INV(x)
#define		LONG_BIT22_INV(x)	DWORD_BIT22_INV(x)
#define		LONG_BIT23_INV(x)	DWORD_BIT23_INV(x)
#define		LONG_BIT24_INV(x)	DWORD_BIT24_INV(x)
#define		LONG_BIT25_INV(x)	DWORD_BIT25_INV(x)
#define		LONG_BIT26_INV(x)	DWORD_BIT26_INV(x)
#define		LONG_BIT27_INV(x)	DWORD_BIT27_INV(x)
#define		LONG_BIT28_INV(x)	DWORD_BIT28_INV(x)
#define		LONG_BIT29_INV(x)	DWORD_BIT29_INV(x)
#define		LONG_BIT30_INV(x)	DWORD_BIT30_INV(x)
#define		LONG_BIT31_INV(x)	DWORD_BIT31_INV(x)
#define		LONG_BITXX_INV(x,y)	DWORD_BITXX_INV(x,y)

/* --- 32bit xのビットnを1にする --- */
#define		DWORD_BIT00_ON(x)	x|=DWORD_BIT_00
#define		DWORD_BIT01_ON(x)	x|=DWORD_BIT_01
#define		DWORD_BIT02_ON(x)	x|=DWORD_BIT_02
#define		DWORD_BIT03_ON(x)	x|=DWORD_BIT_03
#define		DWORD_BIT04_ON(x)	x|=DWORD_BIT_04
#define		DWORD_BIT05_ON(x)	x|=DWORD_BIT_05
#define		DWORD_BIT06_ON(x)	x|=DWORD_BIT_06
#define		DWORD_BIT07_ON(x)	x|=DWORD_BIT_07
#define		DWORD_BIT08_ON(x)	x|=DWORD_BIT_08
#define		DWORD_BIT09_ON(x)	x|=DWORD_BIT_09
#define		DWORD_BIT10_ON(x)	x|=DWORD_BIT_10
#define		DWORD_BIT11_ON(x)	x|=DWORD_BIT_11
#define		DWORD_BIT12_ON(x)	x|=DWORD_BIT_12
#define		DWORD_BIT13_ON(x)	x|=DWORD_BIT_13
#define		DWORD_BIT14_ON(x)	x|=DWORD_BIT_14
#define		DWORD_BIT15_ON(x)	x|=DWORD_BIT_15
#define		DWORD_BIT16_ON(x)	x|=DWORD_BIT_16
#define		DWORD_BIT17_ON(x)	x|=DWORD_BIT_17
#define		DWORD_BIT18_ON(x)	x|=DWORD_BIT_18
#define		DWORD_BIT19_ON(x)	x|=DWORD_BIT_19
#define		DWORD_BIT20_ON(x)	x|=DWORD_BIT_20
#define		DWORD_BIT21_ON(x)	x|=DWORD_BIT_21
#define		DWORD_BIT22_ON(x)	x|=DWORD_BIT_22
#define		DWORD_BIT23_ON(x)	x|=DWORD_BIT_23
#define		DWORD_BIT24_ON(x)	x|=DWORD_BIT_24
#define		DWORD_BIT25_ON(x)	x|=DWORD_BIT_25
#define		DWORD_BIT26_ON(x)	x|=DWORD_BIT_26
#define		DWORD_BIT27_ON(x)	x|=DWORD_BIT_27
#define		DWORD_BIT28_ON(x)	x|=DWORD_BIT_28
#define		DWORD_BIT29_ON(x)	x|=DWORD_BIT_29
#define		DWORD_BIT30_ON(x)	x|=DWORD_BIT_30
#define		DWORD_BIT31_ON(x)	x|=DWORD_BIT_31
#define		DWORD_BITXX_ON(x,y)	x|=DWORD_BIT_XX(y)

#define		LONG_BIT00_ON(x)	DWORD_BIT00_ON(x)
#define		LONG_BIT01_ON(x)	DWORD_BIT01_ON(x)
#define		LONG_BIT02_ON(x)	DWORD_BIT02_ON(x)
#define		LONG_BIT03_ON(x)	DWORD_BIT03_ON(x)
#define		LONG_BIT04_ON(x)	DWORD_BIT04_ON(x)
#define		LONG_BIT05_ON(x)	DWORD_BIT05_ON(x)
#define		LONG_BIT06_ON(x)	DWORD_BIT06_ON(x)
#define		LONG_BIT07_ON(x)	DWORD_BIT07_ON(x)
#define		LONG_BIT08_ON(x)	DWORD_BIT08_ON(x)
#define		LONG_BIT09_ON(x)	DWORD_BIT09_ON(x)
#define		LONG_BIT10_ON(x)	DWORD_BIT10_ON(x)
#define		LONG_BIT11_ON(x)	DWORD_BIT11_ON(x)
#define		LONG_BIT12_ON(x)	DWORD_BIT12_ON(x)
#define		LONG_BIT13_ON(x)	DWORD_BIT13_ON(x)
#define		LONG_BIT14_ON(x)	DWORD_BIT14_ON(x)
#define		LONG_BIT15_ON(x)	DWORD_BIT15_ON(x)
#define		LONG_BIT16_ON(x)	DWORD_BIT16_ON(x)
#define		LONG_BIT17_ON(x)	DWORD_BIT17_ON(x)
#define		LONG_BIT18_ON(x)	DWORD_BIT18_ON(x)
#define		LONG_BIT19_ON(x)	DWORD_BIT19_ON(x)
#define		LONG_BIT20_ON(x)	DWORD_BIT20_ON(x)
#define		LONG_BIT21_ON(x)	DWORD_BIT21_ON(x)
#define		LONG_BIT22_ON(x)	DWORD_BIT22_ON(x)
#define		LONG_BIT23_ON(x)	DWORD_BIT23_ON(x)
#define		LONG_BIT24_ON(x)	DWORD_BIT24_ON(x)
#define		LONG_BIT25_ON(x)	DWORD_BIT25_ON(x)
#define		LONG_BIT26_ON(x)	DWORD_BIT26_ON(x)
#define		LONG_BIT27_ON(x)	DWORD_BIT27_ON(x)
#define		LONG_BIT28_ON(x)	DWORD_BIT28_ON(x)
#define		LONG_BIT29_ON(x)	DWORD_BIT29_ON(x)
#define		LONG_BIT30_ON(x)	DWORD_BIT30_ON(x)
#define		LONG_BIT31_ON(x)	DWORD_BIT31_ON(x)
#define		LONG_BITXX_ON(x,y)	DWORD_BITXX_ON(x,y)

/* --- 32bit xのビットnを0にする --- */
#define		DWORD_BIT00_OFF(x)	x&=DWORD_MASK_R00
#define		DWORD_BIT01_OFF(x)	x&=DWORD_MASK_R01
#define		DWORD_BIT02_OFF(x)	x&=DWORD_MASK_R02
#define		DWORD_BIT03_OFF(x)	x&=DWORD_MASK_R03
#define		DWORD_BIT04_OFF(x)	x&=DWORD_MASK_R04
#define		DWORD_BIT05_OFF(x)	x&=DWORD_MASK_R05
#define		DWORD_BIT06_OFF(x)	x&=DWORD_MASK_R06
#define		DWORD_BIT07_OFF(x)	x&=DWORD_MASK_R07
#define		DWORD_BIT08_OFF(x)	x&=DWORD_MASK_R08
#define		DWORD_BIT09_OFF(x)	x&=DWORD_MASK_R09
#define		DWORD_BIT10_OFF(x)	x&=DWORD_MASK_R10
#define		DWORD_BIT11_OFF(x)	x&=DWORD_MASK_R11
#define		DWORD_BIT12_OFF(x)	x&=DWORD_MASK_R12
#define		DWORD_BIT13_OFF(x)	x&=DWORD_MASK_R13
#define		DWORD_BIT14_OFF(x)	x&=DWORD_MASK_R14
#define		DWORD_BIT15_OFF(x)	x&=DWORD_MASK_R15
#define		DWORD_BIT16_OFF(x)	x&=DWORD_MASK_R16
#define		DWORD_BIT17_OFF(x)	x&=DWORD_MASK_R17
#define		DWORD_BIT18_OFF(x)	x&=DWORD_MASK_R18
#define		DWORD_BIT19_OFF(x)	x&=DWORD_MASK_R19
#define		DWORD_BIT20_OFF(x)	x&=DWORD_MASK_R20
#define		DWORD_BIT21_OFF(x)	x&=DWORD_MASK_R21
#define		DWORD_BIT22_OFF(x)	x&=DWORD_MASK_R22
#define		DWORD_BIT23_OFF(x)	x&=DWORD_MASK_R23
#define		DWORD_BIT24_OFF(x)	x&=DWORD_MASK_R24
#define		DWORD_BIT25_OFF(x)	x&=DWORD_MASK_R25
#define		DWORD_BIT26_OFF(x)	x&=DWORD_MASK_R26
#define		DWORD_BIT27_OFF(x)	x&=DWORD_MASK_R27
#define		DWORD_BIT28_OFF(x)	x&=DWORD_MASK_R28
#define		DWORD_BIT29_OFF(x)	x&=DWORD_MASK_R29
#define		DWORD_BIT30_OFF(x)	x&=DWORD_MASK_R30
#define		DWORD_BIT31_OFF(x)	x&=DWORD_MASK_R31
#define		DWORD_BITXX_OFF(x,y)	x&=DWORD_MASK_RXX(y)

#define		LONG_BIT00_OFF(x)	DWORD_BIT00_OFF(x)
#define		LONG_BIT01_OFF(x)	DWORD_BIT01_OFF(x)
#define		LONG_BIT02_OFF(x)	DWORD_BIT02_OFF(x)
#define		LONG_BIT03_OFF(x)	DWORD_BIT03_OFF(x)
#define		LONG_BIT04_OFF(x)	DWORD_BIT04_OFF(x)
#define		LONG_BIT05_OFF(x)	DWORD_BIT05_OFF(x)
#define		LONG_BIT06_OFF(x)	DWORD_BIT06_OFF(x)
#define		LONG_BIT07_OFF(x)	DWORD_BIT07_OFF(x)
#define		LONG_BIT08_OFF(x)	DWORD_BIT08_OFF(x)
#define		LONG_BIT09_OFF(x)	DWORD_BIT09_OFF(x)
#define		LONG_BIT10_OFF(x)	DWORD_BIT10_OFF(x)
#define		LONG_BIT11_OFF(x)	DWORD_BIT11_OFF(x)
#define		LONG_BIT12_OFF(x)	DWORD_BIT12_OFF(x)
#define		LONG_BIT13_OFF(x)	DWORD_BIT13_OFF(x)
#define		LONG_BIT14_OFF(x)	DWORD_BIT14_OFF(x)
#define		LONG_BIT15_OFF(x)	DWORD_BIT15_OFF(x)
#define		LONG_BIT16_OFF(x)	DWORD_BIT16_OFF(x)
#define		LONG_BIT17_OFF(x)	DWORD_BIT17_OFF(x)
#define		LONG_BIT18_OFF(x)	DWORD_BIT18_OFF(x)
#define		LONG_BIT19_OFF(x)	DWORD_BIT19_OFF(x)
#define		LONG_BIT20_OFF(x)	DWORD_BIT20_OFF(x)
#define		LONG_BIT21_OFF(x)	DWORD_BIT21_OFF(x)
#define		LONG_BIT22_OFF(x)	DWORD_BIT22_OFF(x)
#define		LONG_BIT23_OFF(x)	DWORD_BIT23_OFF(x)
#define		LONG_BIT24_OFF(x)	DWORD_BIT24_OFF(x)
#define		LONG_BIT25_OFF(x)	DWORD_BIT25_OFF(x)
#define		LONG_BIT26_OFF(x)	DWORD_BIT26_OFF(x)
#define		LONG_BIT27_OFF(x)	DWORD_BIT27_OFF(x)
#define		LONG_BIT28_OFF(x)	DWORD_BIT28_OFF(x)
#define		LONG_BIT29_OFF(x)	DWORD_BIT29_OFF(x)
#define		LONG_BIT30_OFF(x)	DWORD_BIT30_OFF(x)
#define		LONG_BIT31_OFF(x)	DWORD_BIT31_OFF(x)
#define		LONG_BITXX_OFF(x,y)	DWORD_BITXX_OFF(x,y)

/* -------------------- ビット確認 -------------------- */

/* --- 8bit xのビットnが1か --- */
#define		BYTE_BIT0_CHK(x)	(x&BYTE_BIT_0)
#define		BYTE_BIT1_CHK(x)	(x&BYTE_BIT_1)
#define		BYTE_BIT2_CHK(x)	(x&BYTE_BIT_2)
#define		BYTE_BIT3_CHK(x)	(x&BYTE_BIT_3)
#define		BYTE_BIT4_CHK(x)	(x&BYTE_BIT_4)
#define		BYTE_BIT5_CHK(x)	(x&BYTE_BIT_5)
#define		BYTE_BIT6_CHK(x)	(x&BYTE_BIT_6)
#define		BYTE_BIT7_CHK(x)	(x&BYTE_BIT_7)
#define		BYTE_BITX_CHK(x,y)	(x&BYTE_BIT_X(y))

/* --- 16bit xのビットnが1か --- */
#define		WORD_BIT00_CHK(x)	(x&WORD_BIT_00)
#define		WORD_BIT01_CHK(x)	(x&WORD_BIT_01)
#define		WORD_BIT02_CHK(x)	(x&WORD_BIT_02)
#define		WORD_BIT03_CHK(x)	(x&WORD_BIT_03)
#define		WORD_BIT04_CHK(x)	(x&WORD_BIT_04)
#define		WORD_BIT05_CHK(x)	(x&WORD_BIT_05)
#define		WORD_BIT06_CHK(x)	(x&WORD_BIT_06)
#define		WORD_BIT07_CHK(x)	(x&WORD_BIT_07)
#define		WORD_BIT08_CHK(x)	(x&WORD_BIT_08)
#define		WORD_BIT09_CHK(x)	(x&WORD_BIT_09)
#define		WORD_BIT10_CHK(x)	(x&WORD_BIT_10)
#define		WORD_BIT11_CHK(x)	(x&WORD_BIT_11)
#define		WORD_BIT12_CHK(x)	(x&WORD_BIT_12)
#define		WORD_BIT13_CHK(x)	(x&WORD_BIT_13)
#define		WORD_BIT14_CHK(x)	(x&WORD_BIT_14)
#define		WORD_BIT15_CHK(x)	(x&WORD_BIT_15)
#define		WORD_BITXX_CHK(x,y)	(x&WORD_BIT_XX(y))

/* --- 32bit xのビットnが1か --- */
#define		DWORD_BIT00_CHK(x)	(x&DWORD_BIT_00)
#define		DWORD_BIT01_CHK(x)	(x&DWORD_BIT_01)
#define		DWORD_BIT02_CHK(x)	(x&DWORD_BIT_02)
#define		DWORD_BIT03_CHK(x)	(x&DWORD_BIT_03)
#define		DWORD_BIT04_CHK(x)	(x&DWORD_BIT_04)
#define		DWORD_BIT05_CHK(x)	(x&DWORD_BIT_05)
#define		DWORD_BIT06_CHK(x)	(x&DWORD_BIT_06)
#define		DWORD_BIT07_CHK(x)	(x&DWORD_BIT_07)
#define		DWORD_BIT08_CHK(x)	(x&DWORD_BIT_08)
#define		DWORD_BIT09_CHK(x)	(x&DWORD_BIT_09)
#define		DWORD_BIT10_CHK(x)	(x&DWORD_BIT_10)
#define		DWORD_BIT11_CHK(x)	(x&DWORD_BIT_11)
#define		DWORD_BIT12_CHK(x)	(x&DWORD_BIT_12)
#define		DWORD_BIT13_CHK(x)	(x&DWORD_BIT_13)
#define		DWORD_BIT14_CHK(x)	(x&DWORD_BIT_14)
#define		DWORD_BIT15_CHK(x)	(x&DWORD_BIT_15)
#define		DWORD_BIT16_CHK(x)	(x&DWORD_BIT_16)
#define		DWORD_BIT17_CHK(x)	(x&DWORD_BIT_17)
#define		DWORD_BIT18_CHK(x)	(x&DWORD_BIT_18)
#define		DWORD_BIT19_CHK(x)	(x&DWORD_BIT_19)
#define		DWORD_BIT20_CHK(x)	(x&DWORD_BIT_20)
#define		DWORD_BIT21_CHK(x)	(x&DWORD_BIT_21)
#define		DWORD_BIT22_CHK(x)	(x&DWORD_BIT_22)
#define		DWORD_BIT23_CHK(x)	(x&DWORD_BIT_23)
#define		DWORD_BIT24_CHK(x)	(x&DWORD_BIT_24)
#define		DWORD_BIT25_CHK(x)	(x&DWORD_BIT_25)
#define		DWORD_BIT26_CHK(x)	(x&DWORD_BIT_26)
#define		DWORD_BIT27_CHK(x)	(x&DWORD_BIT_27)
#define		DWORD_BIT28_CHK(x)	(x&DWORD_BIT_28)
#define		DWORD_BIT29_CHK(x)	(x&DWORD_BIT_29)
#define		DWORD_BIT30_CHK(x)	(x&DWORD_BIT_30)
#define		DWORD_BIT31_CHK(x)	(x&DWORD_BIT_31)
#define		DWORD_BITXX_CHK(x,y)	(x&DWORD_BIT_XX(y))

#define		LONG_BIT00_CHK(x)	DWORD_BIT00_CHK(x)
#define		LONG_BIT01_CHK(x)	DWORD_BIT01_CHK(x)
#define		LONG_BIT02_CHK(x)	DWORD_BIT02_CHK(x)
#define		LONG_BIT03_CHK(x)	DWORD_BIT03_CHK(x)
#define		LONG_BIT04_CHK(x)	DWORD_BIT04_CHK(x)
#define		LONG_BIT05_CHK(x)	DWORD_BIT05_CHK(x)
#define		LONG_BIT06_CHK(x)	DWORD_BIT06_CHK(x)
#define		LONG_BIT07_CHK(x)	DWORD_BIT07_CHK(x)
#define		LONG_BIT08_CHK(x)	DWORD_BIT08_CHK(x)
#define		LONG_BIT09_CHK(x)	DWORD_BIT09_CHK(x)
#define		LONG_BIT10_CHK(x)	DWORD_BIT10_CHK(x)
#define		LONG_BIT11_CHK(x)	DWORD_BIT11_CHK(x)
#define		LONG_BIT12_CHK(x)	DWORD_BIT12_CHK(x)
#define		LONG_BIT13_CHK(x)	DWORD_BIT13_CHK(x)
#define		LONG_BIT14_CHK(x)	DWORD_BIT14_CHK(x)
#define		LONG_BIT15_CHK(x)	DWORD_BIT15_CHK(x)
#define		LONG_BIT16_CHK(x)	DWORD_BIT16_CHK(x)
#define		LONG_BIT17_CHK(x)	DWORD_BIT17_CHK(x)
#define		LONG_BIT18_CHK(x)	DWORD_BIT18_CHK(x)
#define		LONG_BIT19_CHK(x)	DWORD_BIT19_CHK(x)
#define		LONG_BIT20_CHK(x)	DWORD_BIT20_CHK(x)
#define		LONG_BIT21_CHK(x)	DWORD_BIT21_CHK(x)
#define		LONG_BIT22_CHK(x)	DWORD_BIT22_CHK(x)
#define		LONG_BIT23_CHK(x)	DWORD_BIT23_CHK(x)
#define		LONG_BIT24_CHK(x)	DWORD_BIT24_CHK(x)
#define		LONG_BIT25_CHK(x)	DWORD_BIT25_CHK(x)
#define		LONG_BIT26_CHK(x)	DWORD_BIT26_CHK(x)
#define		LONG_BIT27_CHK(x)	DWORD_BIT27_CHK(x)
#define		LONG_BIT28_CHK(x)	DWORD_BIT28_CHK(x)
#define		LONG_BIT29_CHK(x)	DWORD_BIT29_CHK(x)
#define		LONG_BIT30_CHK(x)	DWORD_BIT30_CHK(x)
#define		LONG_BIT31_CHK(x)	DWORD_BIT31_CHK(x)
#define		LONG_BITXX_CHK(x,y)	DWORD_BITXX_CHK(x,y)

#define		_BITDEF_DEFINED
#endif

/* end of userdef.h */
