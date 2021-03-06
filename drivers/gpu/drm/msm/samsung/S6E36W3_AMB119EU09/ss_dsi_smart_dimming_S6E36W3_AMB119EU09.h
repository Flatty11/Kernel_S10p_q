/*
 * =================================================================
 *
 *       Filename:  ss_dsi_smart_dimming_S6E36W3_AMB119EU09.h
 *
 *    Description:  Smart dimming algorithm implementation
 *
 *        Company:  Samsung Electronics
 *
 * ================================================================
 */
/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) 2012, Samsung Electronics. All rights reserved.

*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *
*/
#ifndef _SS_DSI_SMART_DIMMING_S6E36W3_AMB119EU09_H_
#define _SS_DSI_SMART_DIMMING_S6E36W3_AMB119EU09_H_

#include "ss_dsi_panel_common.h"
#include "ss_dsi_smart_dimming_common.h"

static int id1, id2, id3;

#define LUMINANCE_MAX 66
#define HMT_LUMINANCE_MAX 37
#define GAMMA_SET_MAX 34
#define GRAY_SCALE_MAX 256
#define RGB_COMPENSATION 27

/* BIT_SHIFT is used for right bit shfit */
#define BIT_SHIFT 22
#define BIT_SHFIT_MUL 4194304 // pow(2,BIT_SHIFT)

#define VREG0_REF_6P8 28521267	/* 6.8*4194304 */
#define VREG0_REF_6P5 27262976	/* 6.5*4194304 */
#define VREG0_REF_6P4 26843546	/* 6.4*4194304 */
#define VREG0_REF_6P3 26424115	/* 6.3*4194304 */
#define VREG0_REF_6P2 26004685	/* 6.2*4194304 */

#define HBM_INTERPOLATION_STEP 60

enum {
	GAMMA_CURVE_1P9 = 0,
	GAMMA_CURVE_2P15,
	GAMMA_CURVE_2P2,
	GAMMA_CURVE_2P2_400CD,
};

/* TP */
enum {
	VT = 0,
	V0,
	V1,
	V7,
	V11,
	V23,
	V35,
	V51,
	V87,
	V151,
	V203,
	V255,
	V_MAX,
};

/* RGB */
enum {
	R = 0,
	G = 1,
	B = 2,
	RGB_MAX,
};

static char V_LIST[V_MAX][5] = {
	"VT",
	"V0",
	"V1",
	"V7",
	"V11",
	"V23",
	"V35",
	"V51",
	"V87",
	"V151",
	"V203",
	"V255",
};

struct illuminance_table {
	int lux;
	char gamma_setting[GAMMA_SET_MAX];
} __packed;

struct SMART_DIM {
	/* read C8h 1st ~ 35th */
	char MTP_ORIGN[GAMMA_SET_MAX];

	/* copy MTP_ORIGN -> MTP */
	int MTP[V_MAX][RGB_MAX];

	/* TP's gamma voltage */
	int RGB_OUTPUT[V_MAX][RGB_MAX];

	/* GRAY (0~255) */
	int GRAY[GRAY_SCALE_MAX][RGB_MAX];

	/* Because of AID funtion, below members are added*/
	int lux_table_max;
	int *plux_table;
	struct illuminance_table gen_table[LUMINANCE_MAX];

	int brightness_level;
	int ldi_revision;
	int vregout_voltage;
	char panel_revision;

	/* HBM interpolation */
	struct illuminance_table hbm_interpolation_table[HBM_INTERPOLATION_STEP+1];
	char *hbm_payload;
	int hbm_brightness_level;

	struct illuminance_table hmt_gen_table[HMT_LUMINANCE_MAX];
} __packed;

/* V0,V1,V7,V11,V23,V35,V51,V87,V151,V203,V255 */
static int INFLECTION_VOLTAGE_ARRAY[V_MAX] = {0, 0, 1, 7, 11, 23, 35, 51, 87, 151, 203, 255};

static int vt_coefficient[] = {
	0,  12,  24,  36,
	48,  60,  72,  84,
	96, 108, 138, 148,
	158, 168, 178, 186
};

static int v0_coefficient[] = {
	0, 6, 12, 18,
	24, 30, 36, 42,
	48, 54, 60, 66,
	72, 78, 84, 90
};

/* center (max) gamma value (Hex) */
static int center_gamma[V_MAX][RGB_MAX] = {
	{0x0, 0x0, 0x0},	/* VT */
	{0x0, 0x0, 0x0},	/* V0 */
	{0x80, 0x80, 0x80},	/* V1 */
	{0x80, 0x80, 0x80},	/* V7 */
	{0x80, 0x80, 0x80},	/* V11 */
	{0x80, 0x80, 0x80},	/* V23 */
	{0x80, 0x80, 0x80},	/* V35 */
	{0x80, 0x80, 0x80},	/* V51 */
	{0x80, 0x80, 0x80},	/* V87 */
	{0x80, 0x80, 0x80},	/* V151 */
	{0x80, 0x80, 0x80},	/* V203 */
	{0x100, 0x100, 0x100},	/* V255 */
};

/* fraction for gamma code */
static int fraction[V_MAX][2] = {
/* {numerator, denominator} */
	{0,  860},		/* VT */
	{0,  860},		/* V0 */
	{0,  256},		/* V1 */
	{64, 320},		/* V7 */
	{64, 320},		/* V11 */
	{64, 320},		/* V23 */
	{64, 320},		/* V35 */
	{64, 320},		/* V51 */
	{64, 320},		/* V87 */
	{64, 320},		/* V151 */
	{64, 320},		/* V203 */
	{129, 860},		/* V255 */
};

static int hbm_interpolation_candela_table_revA[HBM_INTERPOLATION_STEP] = {426, 433, 438, 445, 452, 458, 465, 469, 476, 483, 
 									489, 495, 502, 508, 515, 521, 528, 533, 540, 547, 
 									553, 560, 564, 571, 578, 584, 591, 596, 603, 610, 
 									616, 622, 629, 635, 642, 648, 655, 659, 666, 673, 
 									679, 686, 691, 698, 705, 711, 718, 723, 730, 737, 
 									743, 750, 754, 761, 768, 774, 780, 787, 793, 800};

static const unsigned int base_luminance_revA[LUMINANCE_MAX][2] = {
	{10, 104},
	{11, 104},
	{12, 104},
	{13, 104},
	{14, 104},
	{15, 104},
	{16, 104},
	{17, 104},
	{19, 104},
	{20, 104},
	{21, 104},
	{22, 104},
	{24, 104},
	{25, 104},
	{27, 104},
	{29, 104},
	{30, 104},
	{32, 104},
	{34, 104},
	{37, 104},
	{39, 104},
	{41, 104},
	{44, 104},
	{47, 104},
	{50, 104},
	{53, 104},
	{56, 104},
	{60, 104},
	{64, 104},
	{68, 107},
	{72, 112},
	{77, 118},
	{82, 125},
	{87, 131},
	{93, 142},
	{98, 149},
	{105, 161},
	{111, 168},
	{119, 179},
	{126, 190},
	{134, 202},
	{143, 215},
	{152, 228},
	{162, 238},
	{172, 252},
	{183, 263},
	{195, 263},
	{207, 263},
	{220, 263},
	{234, 263},
	{249, 271},
	{265, 292},
	{282, 310},
	{300, 328},
	{316, 348},
	{333, 364},
	{350, 385},
	{357, 388},
	{365, 399},
	{372, 399},
	{380, 399},
	{387, 399},
	{395, 399},
	{403, 406},
	{412, 416},
	{420, 420},
};

static const int gradation_offset_revA[LUMINANCE_MAX][9] = {
	/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 4, 4, 10, 16, 20, 23, 28, 29},
	{0, 3, 2, 8, 13, 18, 21, 26, 27},
	{0, 4, 3, 9, 13, 17, 21, 26, 27},
	{0, 2, 1, 7, 12, 16, 19, 24, 25},
	{0, 3, 2, 7, 11, 15, 18, 23, 24},
	{0, 2, 1, 6, 10, 14, 17, 22, 23},
	{0, 3, 1, 6, 10, 14, 16, 21, 22},
	{0, 2, 1, 5, 9, 13, 15, 20, 21},
	{0, 2, 0, 4, 7, 11, 14, 18, 20},
	{0, 1, 1, 4, 7, 11, 14, 17, 18},
	{0, 1, 0, 3, 7, 10, 13, 17, 18},
	{0, 2, 0, 3, 6, 10, 12, 17, 18},
	{0, 2, 0, 3, 5, 9, 11, 15, 17},
	{0, 1, 0, 3, 5, 9, 11, 15, 16},
	{0, 2, 0, 3, 5, 8, 10, 14, 16},
	{0, 1, -1, 2, 4, 7, 9, 13, 14},
	{0, 1, 0, 2, 4, 7, 9, 13, 14},
	{0, 1, 0, 2, 4, 6, 8, 12, 14},
	{0, 1, 0, 1, 3, 6, 7, 12, 13},
	{0, 1, 0, 1, 3, 5, 7, 11, 12},
	{0, 2, 0, 1, 3, 5, 6, 10, 12},
	{0, 0, -1, 1, 2, 4, 6, 8, 9},
	{0, 1, -1, 1, 2, 4, 5, 9, 10},
	{0, 0, -2, 0, 2, 3, 5, 8, 9},
	{0, 2, -1, 0, 2, 3, 4, 8, 9},
	{0, 1, -1, 0, 1, 3, 4, 7, 8},
	{0, 0, -2, 0, 1, 2, 3, 6, 7},
	{0, 0, -1, 0, 1, 2, 3, 5, 6},
	{0, 0, -2, -1, 0, 2, 2, 5, 6},
	{0, 0, -1, -1, 0, 1, 2, 5, 5},
	{0, 0, -2, -1, 0, 1, 2, 5, 6},
	{0, 2, -2, 0, 0, 1, 2, 4, 5},
	{0, 1, -2, 0, 1, 1, 2, 4, 6},
	{0, 3, -1, 0, 0, 2, 2, 4, 5},
	{0, 0, -1, -1, 0, 1, 2, 4, 5},
	{0, 2, -1, 0, 0, 1, 2, 4, 5},
	{0, 1, 0, -1, 0, 0, 1, 3, 5},
	{0, 2, 0, 0, 0, 1, 2, 3, 5},
	{0, 0, 0, 1, 1, 1, 2, 4, 4},
	{0, 1, 2, 1, 0, 1, 2, 3, 4},
	{0, 1, 0, 0, 0, 1, 2, 4, 6},
	{0, 0, 1, 0, 0, 1, 1, 3, 5},
	{0, 1, 1, 1, 0, 0, 2, 3, 5},
	{0, 1, 3, 0, 0, 1, 1, 3, 5},
	{0, 2, 3, 1, 0, 1, 2, 4, 5},
	{0, 2, 2, 1, 0, 0, 1, 3, 5},
	{0, 1, 3, 2, 1, 1, 2, 2, 3},
	{2, 2, 3, 1, 0, 0, 0, 2, 4},
	{2, 2, 2, 1, -1, 0, 0, 1, 2},
	{0, 2, 2, 1, -1, -1, 0, 0, 2},
	{0, 1, 2, 1, 0, 0, 0, 0, 1},
	{0, 0, 1, -1, -1, 0, -1, 1, 2},
	{0, 0, 0, -1, -1, -1, 0, -1, 1},
	{0, 0, 0, -1, -1, -1, -1, 0, 1},
	{0, 0, 0, -1, -1, -1, 0, 0, 1},
	{0, 0, 0, -2, -2, -1, -1, 0, 1},
	{0, -2, -1, -2, -2, -1, -1, 0, 1},
	{0, -1, 0, -2, -1, -1, 0, 0, 1},
	{0, -2, -1, -2, -2, -1, -1, 0, 1},
	{0, -2, -1, -2, -2, -1, -1, -1, 1},
	{0, -2, -1, -2, -2, -1, -1, -1, 1},
	{0, 0, -1, -2, -2, -1, -1, -1, 0},
	{0, -2, -1, -2, -2, -1, -2, -1, 0},
	{0, -3, -1, -3, -2, -2, -1, -1, 0},
	{-1, -2, -2, -3, -2, -1, -1, -1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},

};

static const int rgb_offset_revA[LUMINANCE_MAX][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{-1, 0, -2, 0, 1, 0, -3, -1, -3, -4, 1, -8, -8, -2, -22, -2, 2, -14, -1, 0, -14, -1, -1, -12, 1, 2, -1},
	{0, 0, -1, -2, 0, -2, -1, 1, -1, -5, 1, -9, -6, 0, -18, -5, -2, -18, -1, 1, -14, -1, 0, -11, 0, 2, -1},
	{0, 0, -1, -2, 0, -2, 0, 1, -1, -7, -1, -10, -4, 1, -16, -4, 0, -15, -6, -3, -20, -1, 0, -12, 2, 1, 0},
	{0, 0, -1, -1, 0, -1, 0, 1, -1, -6, -1, -10, -7, -1, -19, -5, -1, -15, 0, 1, -16, -2, -1, -14, 2, 2, 1},
	{-1, 0, -1, 0, 0, -1, -2, 1, -2, -5, 0, -8, -5, 0, -16, -4, 0, -15, -1, 1, -15, -2, -2, -16, 1, 2, 0},
	{0, 0, -1, -1, 0, -1, -1, 0, -2, -6, 0, -9, -5, 0, -15, -4, -1, -17, -1, 0, -14, -3, -2, -17, 2, 0, -1},
	{-1, 0, -1, 0, 0, -1, 0, 1, -1, -6, -1, -9, -7, -1, -17, -4, -1, -15, 1, 4, -11, -2, -2, -17, 2, 1, -2},
	{0, 0, -1, -1, 0, -1, 0, 0, -1, -6, 0, -8, -7, -1, -16, -5, -1, -15, 1, 4, -13, -4, -2, -17, 2, 2, 1},
	{0, 0, -1, -1, 0, -1, -1, 1, -1, -4, -1, -7, -5, 1, -14, -3, 0, -14, -4, -1, -17, 0, 2, -14, -4, -2, -7},
	{2, 2, 2, -1, 0, 0, -1, 0, -1, -4, 1, -6, -4, 1, -12, -4, 0, -14, -4, -2, -20, -2, 2, -17, 2, 2, 1},
	{1, 0, 0, -2, 0, -2, 0, 1, 0, -3, 0, -5, -8, -3, -16, 0, 3, -11, -5, -3, -19, -1, 1, -17, 2, 2, 0},
	{0, 0, -1, 0, 0, 0, -1, 1, -1, -3, 0, -5, -5, 0, -11, -6, -2, -16, -1, 2, -16, -5, -4, -22, 3, 1, 1},
	{1, 0, 0, -1, 0, -1, -1, 1, -1, -4, -1, -6, -2, 2, -8, -6, -2, -16, -3, 1, -16, 1, 1, -15, -2, -4, -8},
	{0, 0, -1, -1, 0, -1, -1, 0, -1, -3, -1, -5, -3, 2, -7, -7, -3, -18, -4, 0, -17, 0, 1, -17, 4, 2, 0},
	{0, 0, -1, 0, 0, 0, 0, 1, 0, -5, -2, -7, -3, 1, -7, -3, 1, -15, -3, -1, -16, 0, 2, -14, -3, -4, -7},
	{1, 0, 0, -1, 0, -1, 0, 0, 0, -3, -1, -5, -3, 1, -7, -4, 0, -14, -5, -1, -16, 0, 2, -17, 4, 1, -1},
	{0, 0, -1, 0, 0, 0, -1, 0, -1, -2, 0, -4, -4, 1, -8, -4, -1, -13, -5, -1, -18, -1, 2, -17, 4, 1, -1},
	{0, 0, -1, 0, 0, 0, -1, 0, -1, -3, 0, -4, -5, -1, -10, 0, 4, -7, -4, -1, -17, -2, 1, -17, -3, -4, -9},
	{0, 0, -1, 0, 0, 0, -1, 0, -1, -1, 1, -2, -4, 0, -7, -5, -3, -15, 1, 4, -9, -8, -6, -27, 2, 1, -1},
	{1, 0, 0, -1, 0, -1, 0, 0, 0, -2, 1, -2, -4, -1, -8, -2, 0, -10, -5, -2, -16, -3, 0, -21, 2, 1, -1},
	{0, 0, 0, 0, 0, -1, -1, 0, -1, -2, 0, -2, -3, -1, -7, -3, 0, -10, 0, 3, -8, -5, -1, -24, -4, -4, -9},
	{2, 2, 2, 1, 1, 1, 0, 1, 0, -2, 0, -2, -2, 1, -5, -2, 1, -9, -5, -2, -15, 1, 4, -17, 1, 3, -1},
	{1, 0, 0, -1, 0, -1, 1, 0, 1, -3, -1, -4, -2, 1, -5, -4, -1, -10, -1, 3, -8, -7, -3, -25, 3, 2, -1},
	{1, 0, 0, 0, 0, 0, -1, 1, 0, -1, 0, -3, -5, -2, -7, 1, 3, -4, -6, -4, -16, -2, 2, -20, 2, 2, -2},
	{1, 0, 0, -1, 0, -1, 1, 0, 1, -1, 0, -1, -5, -2, -7, 1, 3, -4, -2, 1, -9, -10, -5, -28, 3, 2, -1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -2, 1, -4, -3, -2, -9, -4, -1, -10, -4, 1, -21, 2, 2, -1},
	{1, 0, 0, 0, 0, 0, 1, 1, 1, -3, -1, -4, -2, 0, -4, 1, 3, -2, -3, 0, -9, -4, 1, -22, 2, 1, -2},
	{1, 2, 1, 1, 1, 1, 1, 0, 0, 0, 1, -1, -3, -1, -5, 1, 2, -1, -5, -2, -12, -5, 0, -24, 0, -1, -3},
	{1, 0, 0, 0, 0, -1, 0, 1, 1, -1, 0, -1, -1, 0, -2, -4, -3, -7, 0, 3, -4, -7, -2, -25, 0, 0, -2},
	{1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 1, 0, -1, 1, -2, -5, -4, -7, 1, 3, -3, -9, -3, -26, 5, 2, 4},
	{1, 1, 0, 1, 0, 1, 0, -1, -1, 0, 1, -1, -2, 0, -3, 0, 1, -2, -1, 1, -4, -9, -4, -28, -1, -1, -4},
	{2, 1, 0, -1, 0, 0, 1, 0, 1, -3, -1, -4, 0, 1, -1, -2, -2, -5, -2, 0, -6, -4, 2, -20, 5, 2, 3},
	{2, 1, 0, 0, 0, 0, 0, 1, 1, -2, -1, -3, -3, 0, -4, 3, 2, 0, -3, 0, -6, -6, 0, -21, -1, -2, -6},
	{3, 3, 2, 0, -1, 0, 1, 1, 1, -1, 0, -1, 0, 2, -1, -3, -2, -5, 3, 4, 0, -8, -3, -25, -2, -2, -7},
	{1, 1, 0, 0, 0, 1, 0, 0, 0, -1, 0, -1, -1, 2, -2, 0, 0, -3, -5, -3, -8, -2, 3, -17, -2, -1, -6},
	{2, 0, 0, 0, 1, 0, 1, 0, 2, -2, -1, -3, -1, 0, -2, -1, -1, -4, 0, 2, -3, -4, 1, -17, -2, -1, -6},
	{1, 0, 0, 1, 0, 1, -2, 0, -1, 1, 0, 0, -3, 0, -4, 2, 2, 0, -1, 1, -3, -5, 1, -18, -3, -2, -8},
	{2, 1, 0, -1, -1, -1, 1, 0, 2, -1, 0, -1, -3, -1, -5, 1, 1, -1, -3, -1, -6, 0, 5, -13, -4, -2, -8},
	{3, 3, 2, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -3, -1, 1, -3, 0, 2, -1, -6, -1, -21, 2, 2, -2},
	{1, 2, 1, 0, 0, 0, -1, -1, 0, 1, 0, 0, 0, 1, -1, 2, 4, 1, -6, -5, -9, -6, 0, -22, 3, 3, 0},
	{2, 0, 1, -1, 0, -1, 0, 0, -1, 1, 1, 1, -3, -1, -4, -1, 1, -1, -1, 0, -4, -5, 0, -17, -3, -1, -10},
	{2, 0, 1, 1, 1, 0, 0, 0, 0, -1, 0, 0, -1, 0, -2, -2, -1, -3, 2, 3, 1, -6, -1, -18, -4, -1, -11},
	{1, 0, 1, 0, 0, -1, -1, 1, 0, 0, 0, 0, -1, 0, -2, 1, 1, -1, -3, -3, -6, -2, 3, -12, -4, -2, -12},
	{3, 1, 2, -1, 0, -2, 0, 0, 0, 1, 1, 1, -2, -1, -3, 0, 1, -2, -1, 0, -2, -4, 2, -14, -4, -3, -14},
	{1, 1, 1, 0, 0, -1, 0, 0, 1, 0, 0, 0, -1, 0, -2, -2, 0, -3, -2, -1, -4, -6, -1, -16, 1, 3, -6},
	{2, 1, 1, 0, 0, 0, 1, 0, 0, -2, -1, -1, -1, 0, -1, 0, 1, -1, -3, -2, -5, -7, -2, -17, 1, 3, -6},
	{3, 3, 2, 0, 0, 0, 0, 0, 0, 1, 0, 1, -1, -2, -2, 2, 3, 1, -4, -3, -6, -2, 3, -11, -2, -1, -8},
	{1, 0, 1, 0, 0, 0, 1, 0, 1, -1, 0, -1, -1, -1, -2, 0, 1, 0, 1, 1, 1, -8, -4, -17, -2, 0, -8},
	{1, 0, 1, 0, 0, -1, 1, 0, 1, -2, -1, -1, 1, 2, 2, -3, -2, -4, 1, 1, 1, -2, 1, -10, 1, 1, 0},
	{2, 1, 1, 0, 0, -1, 1, 0, 1, -2, -1, -1, 0, 1, 1, 2, 2, 1, -4, -4, -4, 0, 3, -4, -4, -3, -10},
	{2, 1, 1, 1, 0, 0, -1, 0, -1, -1, -1, -1, 0, 1, 0, 0, -1, -1, 0, -1, 0, -1, 2, -4, 1, 0, -2},
	{2, 1, 1, 0, 0, 1, -1, 0, -1, 1, 0, 1, 0, 0, -1, -3, -2, -3, 5, 3, 5, -8, -3, -14, 0, 0, -1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, -3, -3, -3, -4, -2, -8, -3, -1, -7},
	{2, 0, 1, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 1, 1, -1, -1, -1, 1, 0, 1, -4, -2, -9, -2, -1, -3},
	{2, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 1, -2, -1, -1, -1, 0, -1, 1, 0, 0, -1, 2, -3, -2, -2, -4},
	{2, 1, 1, -1, 0, -1, 1, 0, 0, -1, 0, 1, 0, 1, -1, 0, 1, 1, 0, -1, 0, -2, 0, -5, -3, -2, -4},
	{0, 0, 0, 1, 0, 1, 0, 1, 0, -1, -1, 0, 1, 1, 1, -2, -1, -2, 4, 3, 4, -3, 0, -5, -3, -3, -5},
	{2, 1, 0, 1, 0, 2, 0, 0, -1, 0, 0, 0, -2, -1, -2, 1, 2, 2, -3, -3, -4, 3, 4, 3, -4, -3, -5},
	{1, 1, 1, 1, 0, 1, 0, 0, -1, 0, 0, 1, -1, 0, -1, -3, -3, -4, 2, 1, 1, -3, -2, -5, 3, 3, 2},
	{1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, -1, -1, -1, -2, -2, -3, 1, 1, 1, 2, 3, 2, -4, -3, -6},
	{1, 1, 1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 0, -1, 0, -3, -2, -4, 1, 0, 0, 3, 4, 2, -5, -4, -7},
	{2, 1, 1, -1, 0, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -2, -2, -3, 1, 0, 0, 2, 3, 1, 1, 0, 2},
	{1, 1, 1, 1, 0, 1, 0, 0, -1, 0, 0, 1, -2, -1, -1, -1, -2, -3, 5, 4, 7, -4, -3, -5, 1, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0, -1, 1, 1, -2, -4, -3, 0, 1, 0, 0, 0, 1},
	{0, 0, 0, 0, 1, 0, 1, 1, 1, 2, 1, 2, 0, 0, 0, -1, 0, -1, 1, 0, 2, -3, -2, -3, -8, -6, -14},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/************
 *	  HMT   *
 ************/
static unsigned int base_luminance_reverse_hmt_single[][2] = {
	{10, 49},
	{11, 54},
	{12, 60},
	{13, 65},
	{14, 69},
	{15, 74},
	{16, 77},
	{17, 83},
	{19, 92},
	{20, 97},
	{21, 101},
	{22, 105},
	{23, 109},
	{25, 117},
	{27, 126},
	{29, 133},
	{31, 144},
	{33, 152},
	{35, 162},
	{37, 171},
	{39, 178},
	{41, 188},
	{44, 201},
	{47, 213},
	{50, 227},
	{53, 237},
	{56, 251},
	{60, 266},
	{64, 280},
	{68, 292},
	{72, 308},
	{77, 235},
	{82, 252},
	{87, 262},
	{93, 278},
	{99, 291},
	{105, 304},
};

static int gradation_offset_reverse_hmt_single[][9] = {
	/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 1, 1 , 2, 4, 5, 7, 10, 10},
	{0, 0, 1 , 2, 3, 6, 7, 9, 12},
	{0, 0, 1 , 1, 4, 5, 7, 9, 10},
	{0, 0, 2 , 2, 3, 5, 7, 9, 12},
	{0, -1, 2 , 2, 4, 5, 6, 9, 10},
	{0, -1, 2 , 2, 4, 5, 7, 9, 10},
	{0, 0, 1 , 2, 4, 5, 7, 9, 12},
	{0, 0, 1 , 1, 4, 4, 6, 8, 9},
	{0, 1, 1 , 1, 3, 5, 7, 9, 10},
	{0, 1, 2 , 1, 3, 4, 7, 9, 10},
	{0, 0, 1 , 1, 3, 4, 6, 9, 10},
	{0, 2, 2 , 2, 4, 5, 6, 9, 10},
	{0, 1, 1 , 2, 3, 4, 6, 9, 10},
	{0, 2, 2 , 1, 3, 5, 6, 8, 10},
	{0, 3, 3 , 2, 3, 5, 6, 8, 12},
	{0, 3, 2 , 2, 3, 4, 6, 8, 10},
	{0, 3, 3 , 3, 3, 4, 6, 8, 9},
	{0, 2, 4 , 3, 4, 4, 6, 9, 9},
	{0, 2, 2 , 2, 3, 4, 5, 8, 9},
	{0, 1, 3 , 3, 3, 4, 5, 8, 9},
	{0, 2, 4 , 4, 3, 4, 6, 8, 9},
	{0, 1, 3 , 3, 3, 4, 5, 8, 10},
	{0, 1, 4 , 3, 3, 4, 6, 8, 10},
	{0, 1, 4 , 4, 3, 4, 5, 7, 9},
	{0, 2, 4 , 4, 3, 3, 5, 8, 9},
	{0, 2, 4 , 4, 3, 4, 5, 8, 9},
	{0, 3, 5 , 4, 3, 4, 5, 8, 9},
	{0, 2, 5 , 4, 3, 3, 4, 7, 9},
	{0, 4, 6 , 5, 4, 5, 5, 7, 9},
	{0, 5, 6 , 4, 4, 4, 4, 7, 9},
	{0, 5, 6 , 5, 4, 4, 5, 8, 9},
	{0, 1, 4 , 2, 1, 2, 2, 4, 5},
	{0, 1, 3 , 2, 1, 2, 3, 4, 5},
	{0, 2, 5 , 2, 1, 1, 2, 4, 5},
	{0, 3, 4 , 3, 2, 2, 2, 4, 6},
	{0, 4, 6 , 4, 2, 2, 2, 4, 6},
	{0, 5, 6 , 4, 2, 2, 2, 4, 5},
};

static int rgb_offset_reverse_hmt_single[][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{0, 0, 0, 0, 0, 0, -1, 0, -1, -2, 2, -4, -7, 3, -7, -8, 4, -8, -10, 4, -9, -7, 3, -7, -2, 1, -3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 2, -5, -7, 3, -7, -6, 2, -6, -9, 3, -8, -11, 5, -12, -1, 0, -1},
	{0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 1, -4, -5, 3, -6, -9, 3, -8, -8, 4, -8, -9, 5, -11, -3, 1, -3},
	{-1, 0, 0, 0, 0, 0, -1, 0, 0, -1, 1, -3, -6, 3, -7, -6, 3, -6, -10, 4, -10, -11, 5, -11, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 1, -3, -6, 3, -7, -7, 3, -7, -11, 4, -10, -10, 5, -12, -1, 1, -2},
	{0, 0, 0, -1, 0, 0, 0, 0, -1, -1, 1, -3, -6, 2, -6, -6, 3, -6, -8, 4, -9, -12, 6, -12, -1, 1, -3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, -3, -6, 3, -6, -6, 2, -6, -9, 4, -9, -13, 6, -13, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -3, -6, 2, -6, -6, 3, -6, -9, 4, -9, -13, 6, -13, -1, 1, -3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -3, -5, 2, -5, -6, 2, -6, -9, 4, -8, -13, 6, -13, -3, 1, -4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, -2, -5, 2, -6, -6, 2, -6, -8, 4, -8, -13, 6, -12, -3, 1, -4},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, -3, -6, 2, -5, -7, 2, -6, -8, 4, -8, -12, 6, -12, -4, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -4, 3, -6, -7, 2, -6, -9, 4, -9, -11, 6, -12, -4, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0, -2, -3, 2, -6, -7, 3, -6, -8, 4, -8, -13, 6, -13, -4, 1, -4},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -4, 3, -6, -5, 2, -5, -9, 3, -8, -14, 6, -14, -4, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -4, 3, -6, -6, 1, -4, -8, 4, -8, -16, 6, -14, 0, 0, -1},
	{0, 0, 1, 0, 0, 0, -1, 0, -1, 0, 0, 0, -3, 3, -6, -5, 1, -4, -9, 4, -8, -14, 7, -14, -3, 1, -3},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, -2, 2, -5, -4, 2, -4, -8, 3, -7, -16, 7, -15, -5, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -3, 2, -5, -4, 2, -4, -7, 3, -7, -13, 6, -13, -6, 2, -6},
	{2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -4, 2, -5, -4, 2, -5, -7, 3, -8, -13, 6, -13, -6, 2, -5},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 2, -4, -4, 2, -4, -7, 4, -8, -12, 6, -12, -6, 2, -5},
	{0, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 2, -5, -3, 2, -4, -7, 3, -6, -14, 7, -14, -7, 2, -6},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 1, -4, -3, 2, -4, -5, 3, -6, -16, 7, -15, -4, 1, -4},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, -1, -2, 1, -3, -3, 2, -4, -6, 2, -6, -17, 7, -16, -4, 1, -4},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 1, -3, -3, 1, -4, -6, 3, -6, -16, 7, -16, -5, 1, -4},
	{0, 0, 0, 1, 0, 0, 0, 0, 0, -2, 0, 0, 0, 1, -2, -2, 1, -4, -7, 3, -7, -15, 6, -14, -4, 2, -6},
	{1, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 1, -2, -2, 2, -4, -7, 2, -6, -13, 7, -14, -6, 2, -6},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 1, -2, -2, 2, -4, -6, 3, -6, -13, 6, -14, -6, 2, -6},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, -2, -2, 2, -4, -6, 3, -6, -13, 6, -14, -5, 2, -6},
	{1, 0, 2, 0, 0, 0, -1, 0, -1, -1, 0, 0, -2, 1, -2, -2, 2, -4, -6, 2, -6, -15, 7, -14, -4, 2, -5},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, -1, -1, 2, -4, -7, 2, -6, -14, 6, -14, -5, 2, -6},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, -2, 2, -4, -6, 2, -6, -12, 6, -12, -7, 2, -6},
	{0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -2, -1, 1, -2, -3, 2, -4, -11, 6, -14, -6, 3, -6},
	{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -2, 0, 1, -2, -2, 1, -3, -12, 7, -15, -5, 3, -6},
	{0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -2, -1, 1, -2, -3, 1, -3, -10, 6, -13, -6, 4, -8},
	{1, 0, 2, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1, 1, -3, -2, 1, -3, -10, 6, -13, -5, 2, -6},
	{1, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, -2, 2, -4, -2, 1, -2, -10, 6, -12, -4, 2, -6},
	{1, 0, 2, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, -1, -2, 2, -4, -2, 1, -2, -10, 6, -12, -4, 2, -6},
};

#endif
