/**
*	標準構造体定義
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/

#ifndef	_STRUCT_STD_DEFINED
#define	_STRUCT_STD_DEFINED

#include	"userdef.h"

/* 64bit → 32bit×2 リトルエンディアン */
union QWORD_T{
	qword	data;
	struct{
		dword	lo;
		dword	hi;
	} dwords;
};
typedef union QWORD_T qword_t;

/* int64bit → 32bit×2 リトルエンディアン */
union QSWORD_T{
	int64_t	data;
	struct{
		dword	lo;
		dword	hi;
	} dwords;
};
typedef union QSWORD_T qsword_t;

/* 64bit → 16bit×4 リトルエンディアン */
/*union	U64_T {
	qword	data;
	struct{
		word	ll;
		word	lh;
		word	hl;
		word	hh;
	} words;
};
typedef union U64_T	u64_t;*/

/* 32bit → 16bit×2 リトルエンディアン */
union DWORD_T{
	dword	data;
	struct{
		word	lo;
		word	hi;
	} words;
};
typedef union DWORD_T dword_t;

/* 32bit → 8bit×4 リトルエンディアン */
union	UL_T {
	dword	data;
	struct{
		byte	ll;
		byte	lh;
		byte	hl;
		byte	hh;
	} bytes;
};
typedef union UL_T	ul_t;

/* 32bit → 8bit×4 ビッグエンディアン */
union	ULB_T {
	dword	data;
	struct{
		byte	hh;
		byte	hl;
		byte	lh;
		byte	ll;
	} bytes;
};
typedef union ULB_T	ulb_t;

/* 16bit → 8bit×2 リトルエンディアン */
union	UI_T{
	word	data;
	struct{
		byte	lo;
		byte	hi;
	} bytes;
};
typedef union UI_T	ui_t;

/* 32bit → 4bit×8 リトルエンディアン */
union DWNBL_T{
	dword	data;
	struct{
		byte	lll : 4;
		byte	llh : 4;
		byte	lhl : 4;
		byte	lhh : 4;
		byte	hll : 4;
		byte	hlh : 4;
		byte	hhl : 4;
		byte	hhh : 4;
	} dwnbl;
};
typedef union DWNBL_T	dwnbl_t;

/* 16bit → 4bit×4 リトルエンディアン */
union WNBL_T{
	word	data;
	struct{
		byte	ll : 4;
		byte	lh : 4;
		byte	hl : 4;
		byte	hh : 4;
	} wnbl;
};
typedef union WNBL_T	wnbl_t;

/* 8bit → 4bit×2 リトルエンディアン */
union NBL_T{
	byte	data;
	struct{
		byte	lo : 4;
		byte	hi : 4;
	} nbl;
};
typedef union NBL_T	nbl_t;

/* 64bit → 32bit×2 ビッグエンディアン */
union QWORDBE_T{
	qword	data;
	struct{
		dword	hi;
		dword	lo;
	} dwords;
};
typedef union QWORDBE_T qwordbe_t;

/* int64bit → 32bit×2 ビッグエンディアン */
union QSWORDBE_T{
	int64_t	data;
	struct{
		dword	hi;
		dword	lo;
	} dwords;
};
typedef union QSWORDBE_T qswordbe_t;

/* 64bit → 16bit×4 ビッグエンディアン */
union	U64BE_T {
	qword	data;
	struct{
		word	hh;
		word	hl;
		word	lh;
		word	ll;
	} words;
};
typedef union U64BE_T	u64be_t;

/* 32bit → 16bit×2 ビッグエンディアン */
union DWORDBE_T{
	dword	data;
	struct{
		word	hi;
		word	lo;
	} words;
};
typedef union DWORDBE_T dwordbe_t;

/* 32bit → 8bit×4 ビッグエンディアン */
union	ULBE_T {
	dword	data;
	struct{
		byte	hh;
		byte	hl;
		byte	lh;
		byte	ll;
	} bytes;
};
typedef union ULBE_T	ulbe_t;

/* 16bit → 8bit×2 ビッグエンディアン */
union	UIBE_T{
	word	data;
	struct{
		byte	hi;
		byte	lo;
	} bytes;
};
typedef union UIBE_T	uibe_t;

/* 32bit → 4bit×8 ビッグエンディアン */
union DWNBLBE_T{
	dword	data;
	struct{
		byte	hhh : 4;
		byte	hhl : 4;
		byte	hlh : 4;
		byte	hll : 4;
		byte	lhh : 4;
		byte	lhl : 4;
		byte	llh : 4;
		byte	lll : 4;
	} dwnbl;
};
typedef union DWNBLBE_T	dwnblbe_t;

/* 16bit → 4bit×4 ビッグエンディアン */
union WNBLBE_T{
	word	data;
	struct{
		byte	hh : 4;
		byte	hl : 4;
		byte	lh : 4;
		byte	ll : 4;
	} wnbl;
};
typedef union WNBLBE_T	wnblbe_t;

/* 8bit → 4bit×2 ビッグエンディアン */
union NBLBE_T{
	byte	data;
	struct{
		byte	hi : 4;
		byte	lo : 4;
	} nbl;
};
typedef union NBLBE_T	nblbe_t;

#endif

/* end of struct_std.h */
