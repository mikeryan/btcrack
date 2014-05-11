/*
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License 2
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License 2 for more details.

 You should have received a copy of the GNU General Public License 2
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 
 It grants to you the four following freedoms:
   0. The freedom to run the program for any purpose.
   1. The freedom to study how the program works and adapt it to your needs.
   2. The freedom to redistribute copies so you can help your neighbor.
   3. The freedom to improve the program and release your improvements to 
      the public, so that the whole community benefits.

 You must conspicuously and appropriately publish on each copy distributed an 
 appropriate copyright notice and disclaimer of warranty and keep intact all the 
 notices that refer to this License and to the absence of any warranty; and give 
 any other recipients of the Program a copy of the GNU General Public License 
 along with the Program. Any translation of the GNU General Public License must 
 be accompanied by the GNU General Public License.
 
 If you modify your copy or copies of the program or any portion of it, or 
 develop a program based upon it, you may distribute the resulting work provided
 you do so under the GNU General Public License. Any translation of the GNU General Public License
 must be accompanied by the GNU General Public License.
 
 If you copy or distribute the program, you must accompany it with the complete 
 corresponding machine-readable source code or with a written offer, valid for 
 at least three years, to furnish the complete corresponding machine-readable 
 source code.
  
 Contact : Thierry Zoller - Thierry@Zoller.lu
           http://secdev.zoller.lu
 Contact : Eric Sesterhen  snakebyte@gmx.de


 THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "btcrack.h"

static const unsigned char safer_ebox[256] = {
  1,  45, 226, 147, 190,  69,  21, 174, 120,	3, 135, 164, 184,  56, 207,  63, 
  8, 103,	9, 148, 235,  38, 168, 107, 189,  24,  52,  27, 187, 191, 114, 247, 
 64,  53,  72, 156,  81,  47,  59,  85, 227, 192, 159, 216, 211, 243, 141, 177, 
255, 167,  62, 220, 134, 119, 215, 166,  17, 251, 244, 186, 146, 145, 100, 131, 
241,  51, 239, 218,  44, 181, 178,  43, 136, 209, 153, 203, 140, 132,  29,  20, 
129, 151, 113, 202,  95, 163, 139,  87,  60, 130, 196,  82,  92,  28, 232, 160, 
  4, 180, 133,  74, 246,  19,  84, 182, 223,  12,  26, 142, 222, 224,  57, 252, 
 32, 155,  36,  78, 169, 152, 158, 171, 242,  96, 208, 108, 234, 250, 199, 217, 
  0, 212,  31, 110,  67, 188, 236,  83, 137, 254, 122,  93,  73, 201,  50, 194, 
249, 154, 248, 109,  22, 219,  89, 150,  68, 233, 205, 230,  70,  66, 143,  10, 
193, 204, 185, 101, 176, 210, 198, 172,  30,  65,  98,  41,  46,  14, 116,  80, 
  2,  90, 195,  37, 123, 138,  42,  91, 240,	6,  13,  71, 111, 112, 157, 126, 
 16, 206,  18,  39, 213,  76,  79, 214, 121,  48, 104,  54, 117, 125, 228, 237, 
128, 106, 144,  55, 162,  94, 118, 170, 197, 127,  61, 175, 165, 229,  25,  97, 
253,  77, 124, 183,  11, 238, 173,  75,  34, 245, 231, 115,  35,  33, 200,	5, 
225, 102, 221, 179,  88, 105,  99,  86,  15, 161,  49, 149,  23,	7,  58,  40
};

static const unsigned char safer_bias[33][16] = {
{  70, 151, 177, 186, 163, 183,  16,  10, 197,  55, 179, 201,  90,  40, 172, 100},
{ 236, 171, 170, 198, 103, 149,  88,  13, 248, 154, 246, 110, 102, 220,	5,  61},
{ 138, 195, 216, 137, 106, 233,  54,  73,  67, 191, 235, 212, 150, 155, 104, 160},
{  93,  87, 146,  31, 213, 113,  92, 187,  34, 193, 190, 123, 188, 153,  99, 148},
{  42,  97, 184,  52,  50,  25, 253, 251,  23,  64, 230,  81,  29,  65,  68, 143},
{ 221,	4, 128, 222, 231,  49, 214, 127,	1, 162, 247,  57, 218, 111,  35, 202},
{  58, 208,  28, 209,  48,  62,  18, 161, 205,  15, 224, 168, 175, 130,  89,  44},
{ 125, 173, 178, 239, 194, 135, 206, 117,	6,  19,	2, 144,  79,  46, 114,  51},
{ 192, 141, 207, 169, 129, 226, 196,  39,  47, 108, 122, 159,  82, 225,  21,  56},
{ 252,  32,  66, 199,	8, 228,	9,  85,  94, 140,  20, 118,  96, 255, 223, 215},
{ 250,  11,  33,	0,  26, 249, 166, 185, 232, 158,  98,  76, 217, 145,  80, 210},
{  24, 180,	7, 132, 234,  91, 164, 200,  14, 203,  72, 105,  75,  78, 156,  53},
{  69,  77,  84, 229,  37,  60,  12,  74, 139,  63, 204, 167, 219, 107, 174, 244},
{  45, 243, 124, 109, 157, 181,  38, 116, 242, 147,  83, 176, 240,  17, 237, 131},
{ 182,	3,  22, 115,  59,  30, 142, 112, 189, 134,  27,  71, 126,  36,  86, 241},
{ 136,  70, 151, 177, 186, 163, 183,  16,  10, 197,  55, 179, 201,  90,  40, 172},
{ 220, 134, 119, 215, 166,  17, 251, 244, 186, 146, 145, 100, 131, 241,  51, 239},
{  44, 181, 178,  43, 136, 209, 153, 203, 140, 132,  29,  20, 129, 151, 113, 202},
{ 163, 139,  87,  60, 130, 196,  82,  92,  28, 232, 160,	4, 180, 133,  74, 246},
{  84, 182, 223,  12,  26, 142, 222, 224,  57, 252,  32, 155,  36,  78, 169, 152},
{ 171, 242,  96, 208, 108, 234, 250, 199, 217,	0, 212,  31, 110,  67, 188, 236},
{ 137, 254, 122,  93,  73, 201,  50, 194, 249, 154, 248, 109,  22, 219,  89, 150},
{ 233, 205, 230,  70,  66, 143,  10, 193, 204, 185, 101, 176, 210, 198, 172,  30},
{  98,  41,  46,  14, 116,  80,	2,  90, 195,  37, 123, 138,  42,  91, 240,	6},
{  71, 111, 112, 157, 126,  16, 206,  18,  39, 213,  76,  79, 214, 121,  48, 104},
{ 117, 125, 228, 237, 128, 106, 144,  55, 162,  94, 118, 170, 197, 127,  61, 175},
{ 229,  25,  97, 253,  77, 124, 183,  11, 238, 173,  75,  34, 245, 231, 115,  35},
{ 200,	5, 225, 102, 221, 179,  88, 105,  99,  86,  15, 161,  49, 149,  23,	7},
{  40,	1,  45, 226, 147, 190,  69,  21, 174, 120,	3, 135, 164, 184,  56, 207},
{	8, 103,	9, 148, 235,  38, 168, 107, 189,  24,  52,  27, 187, 191, 114, 247},
{  53,  72, 156,  81,  47,  59,  85, 227, 192, 159, 216, 211, 243, 141, 177, 255},
{  62, 220, 134, 119, 215, 166,  17, 251, 244, 186, 146, 145, 100, 131, 241,  51}};

static const unsigned char safer_lbox[256] = {
128,	0, 176,	9,  96, 239, 185, 253,  16,  18, 159, 228, 105, 186, 173, 248,
192,  56, 194, 101,  79,	6, 148, 252,  25, 222, 106,  27,  93,  78, 168, 130,
112, 237, 232, 236, 114, 179,  21, 195, 255, 171, 182,  71,  68,	1, 172,  37, 
201, 250, 142,  65,  26,  33, 203, 211,  13, 110, 254,  38,  88, 218,  50,  15, 
 32, 169, 157, 132, 152,	5, 156, 187,  34, 140,  99, 231, 197, 225, 115, 198, 
175,  36,  91, 135, 102,  39, 247,  87, 244, 150, 177, 183,  92, 139, 213,  84, 
121, 223, 170, 246,  62, 163, 241,  17, 202, 245, 209,  23, 123, 147, 131, 188, 
189,  82,  30, 235, 174, 204, 214,  53,	8, 200, 138, 180, 226, 205, 191, 217,
208,  80,  89,  63,  77,  98,  52,  10,  72, 136, 181,  86,  76,  46, 107, 158, 
210,  61,  60,	3,  19, 251, 151,  81, 117,  74, 145, 113,  35, 190, 118,  42, 
 95, 249, 212,  85,  11, 220,  55,  49,  22, 116, 215, 119, 167, 230,	7, 219,
164,  47,  70, 243,  97,  69, 103, 227,  12, 162,  59,  28, 133,  24,	4,  29, 
 41, 160, 143, 178,  90, 216, 166, 126, 238, 141,  83,  75, 161, 154, 193,  14, 
122,  73, 165,  44, 129, 196, 199,  54,  43, 127,  67, 149,  51, 242, 108, 104, 
109, 240,	2,  40, 206, 221, 155, 234,  94, 153, 124,  20, 134, 207, 229,  66, 
184,  64, 120,  45,  58, 233, 100,  31, 146, 144, 125,  57, 111, 224, 137,  48
};

static void ROUND(unsigned char b[16], int i, symmetric_key *skey) {
	b[0]  = (safer_ebox[(b[0] ^ skey->saferp.K[i][0]) & 255] + skey->saferp.K[i+1][0]) & 255;
	b[1]  = safer_lbox[(b[1] + skey->saferp.K[i][1]) & 255] ^ skey->saferp.K[i+1][1];
	b[2]  = safer_lbox[(b[2] + skey->saferp.K[i][2]) & 255] ^ skey->saferp.K[i+1][2];
	b[3]  = (safer_ebox[(b[3] ^ skey->saferp.K[i][3]) & 255] + skey->saferp.K[i+1][3]) & 255;
	b[4]  = (safer_ebox[(b[4] ^ skey->saferp.K[i][4]) & 255] + skey->saferp.K[i+1][4]) & 255;
	b[5]  = safer_lbox[(b[5] + skey->saferp.K[i][5]) & 255] ^ skey->saferp.K[i+1][5];
	b[6]  = safer_lbox[(b[6] + skey->saferp.K[i][6]) & 255] ^ skey->saferp.K[i+1][6];
	b[7]  = (safer_ebox[(b[7] ^ skey->saferp.K[i][7]) & 255] + skey->saferp.K[i+1][7]) & 255;
	b[8]  = (safer_ebox[(b[8] ^ skey->saferp.K[i][8]) & 255] + skey->saferp.K[i+1][8]) & 255;
	b[9]  = safer_lbox[(b[9] + skey->saferp.K[i][9]) & 255] ^ skey->saferp.K[i+1][9];
	b[10] = safer_lbox[(b[10] + skey->saferp.K[i][10]) & 255] ^ skey->saferp.K[i+1][10];
	b[11] = (safer_ebox[(b[11] ^ skey->saferp.K[i][11]) & 255] + skey->saferp.K[i+1][11]) & 255;
	b[12] = (safer_ebox[(b[12] ^ skey->saferp.K[i][12]) & 255] + skey->saferp.K[i+1][12]) & 255;
	b[13] = safer_lbox[(b[13] + skey->saferp.K[i][13]) & 255] ^ skey->saferp.K[i+1][13];
	b[14] = safer_lbox[(b[14] + skey->saferp.K[i][14]) & 255] ^ skey->saferp.K[i+1][14];
	b[15] = (safer_ebox[(b[15] ^ skey->saferp.K[i][15]) & 255] + skey->saferp.K[i+1][15]) & 255;
}

static void PHT(unsigned char b[16]) {
	b[0]  = (b[0] + (b[1] = (b[1] + b[0]) & 255)) & 255;
	b[2]  = (b[2] + (b[3] = (b[3] + b[2]) & 255)) & 255;
	b[4]  = (b[4] + (b[5] = (b[5] + b[4]) & 255)) & 255;
	b[6]  = (b[6] + (b[7] = (b[7] + b[6]) & 255)) & 255;
	b[8]  = (b[8] + (b[9] = (b[9] + b[8]) & 255)) & 255;
	b[10] = (b[10] + (b[11] = (b[11] + b[10]) & 255)) & 255;
	b[12] = (b[12] + (b[13] = (b[13] + b[12]) & 255)) & 255;
	b[14] = (b[14] + (b[15] = (b[15] + b[14]) & 255)) & 255;
}

static void SHUF(unsigned char b[16], unsigned char b2[16]) {
	b2[0] = b[8]; b2[1] = b[11]; b2[2] = b[12]; b2[3] = b[15];
	b2[4] = b[2]; b2[5] = b[1]; b2[6] = b[6]; b2[7] = b[5];
	b2[8] = b[10]; b2[9] = b[9]; b2[10] = b[14]; b2[11] = b[13];
	b2[12] = b[0]; b2[13] = b[7]; b2[14] = b[4]; b2[15] = b[3];
}

static void LT(unsigned char b[16], unsigned char b2[16]) {
	PHT(b);  SHUF(b, b2);
	PHT(b2); SHUF(b2, b);
	PHT(b);  SHUF(b, b2);
	PHT(b2); 
}

void saferp_ecb_encrypt(const unsigned char *pt, unsigned char *ct, symmetric_key *skey)
{
	unsigned char b[16];

	memcpy(b, pt, 16);
	
	ROUND(b,  0, skey);  LT(b, ct);
	ROUND(ct, 2, skey);  LT(ct, b);
	ROUND(b,  4, skey);  LT(b, ct);
	ROUND(ct, 6, skey);  LT(ct, b);
	ROUND(b,  8, skey);  LT(b, ct);
	ROUND(ct, 10, skey); LT(ct, b);
	ROUND(b,  12, skey); LT(b, ct);
	ROUND(ct, 14, skey); LT(ct, b);
	

	ct[0] = b[0] ^ skey->saferp.K[16][0];
	ct[1] = (b[1] + skey->saferp.K[16][1]) & 255;
	ct[2] = (b[2] + skey->saferp.K[16][2]) & 255;
	ct[3] = b[3] ^ skey->saferp.K[16][3];
	ct[4] = b[4] ^ skey->saferp.K[16][4];
	ct[5] = (b[5] + skey->saferp.K[16][5]) & 255;
	ct[6] = (b[6] + skey->saferp.K[16][6]) & 255;
	ct[7] = b[7] ^ skey->saferp.K[16][7];
	ct[8] = b[8] ^ skey->saferp.K[16][8];
	ct[9] = (b[9] + skey->saferp.K[16][9]) & 255;
	ct[10] = (b[10] + skey->saferp.K[16][10]) & 255;
	ct[11] = b[11] ^ skey->saferp.K[16][11];
	ct[12] = b[12] ^ skey->saferp.K[16][12];
	ct[13] = (b[13] + skey->saferp.K[16][13]) & 255;
	ct[14] = (b[14] + skey->saferp.K[16][14]) & 255;
	ct[15] = b[15] ^ skey->saferp.K[16][15];
}


void BTSaferpEncrypt(const unsigned char *pt, unsigned char *ct, symmetric_key *skey)
{
	unsigned char b[16];

	memcpy(b, pt, 16);
	
	ROUND(b,  0, skey);  LT(b, ct); 
	ROUND(ct, 2, skey);  LT(ct, b); 

	b[0] ^= pt[0];
	b[1] += pt[1];
	b[2] += pt[2];
	b[3] ^= pt[3];
	b[4] ^= pt[4];
	b[5] += pt[5];
	b[6] += pt[6];
	b[7] ^= pt[7];
	b[8] ^= pt[8];
	b[9] += pt[9];
	b[10] += pt[10];
	b[11] ^= pt[11];
	b[12] ^= pt[12];
	b[13] += pt[13];
	b[14] += pt[14];
	b[15] ^= pt[15];

	ROUND(b,  4, skey);  LT(b, ct); 
	ROUND(ct, 6, skey);  LT(ct, b); 
	ROUND(b,  8, skey);  LT(b, ct); 
	ROUND(ct, 10, skey); LT(ct, b); 
	ROUND(b,  12, skey); LT(b, ct); 
	ROUND(ct, 14, skey); LT(ct, b); 

	ct[0] = b[0] ^ skey->saferp.K[16][0];
	ct[1] = (b[1] + skey->saferp.K[16][1]) & 255;
	ct[2] = (b[2] + skey->saferp.K[16][2]) & 255;
	ct[3] = b[3] ^ skey->saferp.K[16][3];
	ct[4] = b[4] ^ skey->saferp.K[16][4];
	ct[5] = (b[5] + skey->saferp.K[16][5]) & 255;
	ct[6] = (b[6] + skey->saferp.K[16][6]) & 255;
	ct[7] = b[7] ^ skey->saferp.K[16][7];
	ct[8] = b[8] ^ skey->saferp.K[16][8];
	ct[9] = (b[9] + skey->saferp.K[16][9]) & 255;
	ct[10] = (b[10] + skey->saferp.K[16][10]) & 255;
	ct[11] = b[11] ^ skey->saferp.K[16][11];
	ct[12] = b[12] ^ skey->saferp.K[16][12];
	ct[13] = (b[13] + skey->saferp.K[16][13]) & 255;
	ct[14] = (b[14] + skey->saferp.K[16][14]) & 255;
	ct[15] = b[15] ^ skey->saferp.K[16][15];
}


void BTSaferpInit(const unsigned char *key, symmetric_key *skey)
{
	unsigned z;
	int x, y;
	unsigned char t[33];
	/* also fits for keylen != 16 */
	static const unsigned char mod_table[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			11, 12, 13, 14, 15, 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			11, 12, 13, 14, 15, 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			11, 12, 13, 14, 15, 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			11, 12, 13, 14, 15, 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	y = 0;
	for (x = 15; x >= 0; x--) { 
		t[x] = key[x]; 
		y ^= key[x]; 
	}
	t[16] = y;
	memcpy(skey->saferp.K[0], t, 16);
	for (x = 1; x < 17; x++) {

		for (y = 16; y >= 0; y--) {
			/* ROL */
			t[y] = ((t[y]<<3)|(t[y]>>5)) & 255;
		}

		z = x;
		for (y = 0; y < 16; y++) {
			skey->saferp.K[x][y] = (t[mod_table[z]] + safer_bias[x-1][y]) & 255;
			z++;
		}
	}
	return;
}



void BTCrackStep2(unsigned char *in_rand, unsigned char *in_bdaddr, unsigned char *out_link_key)
{

	unsigned char key[16];
	unsigned char rand[16];
	int i;

	symmetric_key skey;

	for (i = 0; i < 16; i++) {
		key[i] = in_rand[i];
		rand[i] = ((char *) (in_bdaddr))[i % 6];
	}

	key[15] ^= 6;

	BTSaferpInit(key, &skey);
	BTSaferpEncrypt(rand, out_link_key, &skey);
	return;
}


void BTCrackStep3(unsigned char *in_key, unsigned char *in_rand, int pinlenprime, unsigned char *out_k) 
{
	unsigned char rand[16];
	symmetric_key skey;

	memcpy(rand, in_rand, 16);

	rand[15] ^= pinlenprime;

	BTSaferpInit(in_key, &skey);
	BTSaferpEncrypt(rand, out_k, &skey);
	return;
}


void BTCrackStep1(unsigned char *in_kab, unsigned char *in_rand, unsigned char *in_bdaddr,
	unsigned char *out_sres, unsigned char *out_aco)
{

	unsigned char kab_offset[16];
	unsigned char ar_result[16];
	unsigned char sresaco[16];
	int i;
	symmetric_key skey;

	BTSaferpInit(in_kab, &skey);
	saferp_ecb_encrypt(in_rand, ar_result, &skey);

	for (i = 0; i < 16; i++) {
		ar_result[i] ^= in_rand[i];
		ar_result[i] += in_bdaddr[i % 6];
	}

	kab_offset[0] = in_kab[0] + 233;
	kab_offset[1] = in_kab[1] ^ 229;
	kab_offset[2] = in_kab[2] + 223;
	kab_offset[3] = in_kab[3] ^ 193;
	kab_offset[4] = in_kab[4] + 179;
	kab_offset[5] = in_kab[5] ^ 167;
	kab_offset[6] = in_kab[6] + 149;
	kab_offset[7] = in_kab[7] ^ 131;
	kab_offset[8] = in_kab[8] ^ 233;
	kab_offset[9] = in_kab[9] + 229;
	kab_offset[10] = in_kab[10] ^ 223;
	kab_offset[11] = in_kab[11] + 193;
	kab_offset[12] = in_kab[12] ^ 179;
	kab_offset[13] = in_kab[13] + 167;
	kab_offset[14] = in_kab[14] ^ 149;
	kab_offset[15] = in_kab[15] + 131;

	BTSaferpInit(kab_offset, &skey);
	BTSaferpEncrypt(ar_result, sresaco, &skey);

	memcpy(out_sres, sresaco, 4);
	memcpy(out_aco, sresaco+4, 12);

	return;
}


int KeyInit(unsigned char in_pinlen, unsigned char *in_pin, unsigned char *in_bdaddr,
					 unsigned char *in_rand, unsigned char *out_kinit)
{
	unsigned char pinprime[16];
	unsigned char pinlenprime;
	int i, j;


	for (i = 0; i < 16;) {
		for (j = 0; (j < in_pinlen) && (i < 16); j++) {
			pinprime[i] = in_pin[j];
			i++;
		}
		for (j = 0; (j < 6) && (i < 16); j++) {
			pinprime[i] = in_bdaddr[j];
			i++;
		}
	}

	pinlenprime = (in_pinlen + 6 < 16) ? in_pinlen + 6 : 16;

	BTCrackStep3(pinprime, in_rand, pinlenprime, out_kinit);
	return 0;
}


void GetMasterKey(unsigned char in_key[16], unsigned char in_rand[16], unsigned char *out_k)
{
	BTCrackStep3(in_key, in_rand, 16, out_k);
}


void GetLRand(unsigned char *in_lkcombrand, unsigned char *in_kinit, unsigned char *out_lkrand) {
	int i;

	for (i=0; i < 16; i++) {
		out_lkrand[i] = in_lkcombrand[i] ^ in_kinit[i];
	}

	return;
}


int GetPIN(unsigned char *in_bdaddr_a, unsigned char *in_bdaddr_b, unsigned char *in_lkrand_a,
	unsigned char *in_lkrand_b, unsigned char *out_kab) {

	unsigned char lk_ka[16], lk_kb[16];
	int i;

	BTCrackStep2(in_lkrand_a, in_bdaddr_a, lk_ka);
	BTCrackStep2(in_lkrand_b, in_bdaddr_b, lk_kb);

	for (i=0; i < 16; i++) {
		out_kab[i] = lk_ka[i] ^ lk_kb[i];
	}

	return 0;
}


void GetSRES(unsigned char *in_kab, unsigned char *in_aurand, unsigned char *in_bdaddr,
	unsigned char *out_sres, unsigned char *out_aco) {

	BTCrackStep1(in_kab, in_aurand, in_bdaddr, out_sres, out_aco);
}

int ProcessPIN(struct BTCrackStruct *bp, unsigned char *pin, int pinlen)
{

	if (pinlen > 16)
		return -1;

	if (KeyInit(pinlen, pin, bp->bdaddr_s, bp->in_rand, bp->kinit) != 0) {
		return -1;
	}

	GetLRand(bp->comb_key_m, bp->kinit, bp->lk_rand_m);
	GetLRand(bp->comb_key_s, bp->kinit, bp->lk_rand_s);

	if (GetPIN(bp->bdaddr_m, bp->bdaddr_s, bp->lk_rand_m, bp->lk_rand_s, bp->kab) != 0) {
		return -3;
	}

	GetSRES(bp->kab, bp->au_rand_m, bp->bdaddr_s, bp->calc_sres_s, bp->calc_aco_m);

	if (memcmp(bp->calc_sres_s, bp->sres_s, 4) == 0) {
		GetSRES(bp->kab, bp->au_rand_s, bp->bdaddr_m, bp->calc_sres_m, bp->calc_aco_s); //carl.dunhamm@hotmail.com
		if (memcmp(bp->calc_sres_m, bp->sres_m, 4) == 0) {
			return 0;
		}
	}
	/* WRONG PIN */
	return 1;
}
