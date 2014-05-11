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
  
 Contact : Thierry Zoller  Thierry.zoller@nruns.com / Thierry@Zoller.lu
 Contact : Eric Sesterhen  snakebyte@gmx.de
 n.runs AG - http://www.nruns.com

 THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.

***********************************************************
 * DEFINES 
 ***********************************************************
 */
 
#define MAX_LENGTH 16
#define MIN_LENGTH 1
#define MAX_THREADS 16 
#define APPNAME "BTCrack"
#define VERSION "1.00"


struct saferp_key {
   unsigned char K[33][16];
   long rounds;
};
typedef union Symmetric_key {
   struct saferp_key   saferp;
   void   *data;
} symmetric_key;

/***********************************************************
 * BT STRUCT
 ***********************************************************/
struct BTCrackStruct {
	unsigned char bdaddr_m[6];	/* BD_ADDR of the Master */
	unsigned char bdaddr_s[6];	/* BD_ADDR of the Slave */
	unsigned char in_rand[16];	/* LMP_in_rand value, usually from Master */
	unsigned char comb_key_m[16];	/* LMP_comb_key from Master */
	unsigned char comb_key_s[16];	/* LMP_comb_key from Slave */
	unsigned char au_rand_m[16];	/* LMP_au_rand from Master */
	unsigned char sres_s[4];	/* LMP_sres from Slave */
	unsigned char au_rand_s[16];	/* LMP_au_rand from Slave */
	unsigned char sres_m[4];	/* LMP_sres from Master */

	unsigned char kinit[16];	/* Returned from E22 */
	unsigned char lk_rand_m[16];	/* COMB_KEY_a XOR K_init */
	unsigned char lk_rand_s[16];	/* COMB_KEY_b XOR K_init */
	unsigned char kab[16];		/* Returned from E21 */
	unsigned char calc_sres_m[4];	/* Returned from E1, 4 MSB */
	unsigned char calc_aco_m[12];	/* Returned from E1, 12 LSB */
	unsigned char calc_sres_s[4];	/* Returned from E1, 4 MSB */
	unsigned char calc_aco_s[12];	/* Returned from E1, 12 LSB */
	unsigned char pin[17];		/* PIN result as a string */
	int pinlen;			/* Length of PIN */
	unsigned long pin_start;
	unsigned long pin_end;
	unsigned long pins_per_sec;
	unsigned char link_key[64];
	int  pin_solved;
};


/***********************************************************
 * FUNCTION PROTOTYPES
 ***********************************************************/
int ProcessPIN(struct BTCrackStruct *bp, unsigned char *pin, int pinlen);

/***********************************************************
 * GLOBALS
 ***********************************************************/

enum {
   CRYPT_OK=0,             /* Result OK */
   CRYPT_ERROR,            /* Generic Error */
   CRYPT_NOP,              /* Not a failure but no operation was performed */

   CRYPT_INVALID_KEYSIZE,  /* Invalid key size given */
   CRYPT_INVALID_ROUNDS,   /* Invalid number of rounds */
   CRYPT_FAIL_TESTVECTOR,  /* Algorithm failed test vectors */

   CRYPT_BUFFER_OVERFLOW,  /* Not enough space for output */
   CRYPT_INVALID_PACKET,   /* Invalid input packet given */

   CRYPT_INVALID_PRNGSIZE, /* Invalid number of bits for a PRNG */
   CRYPT_ERROR_READPRNG,   /* Could not read enough from PRNG */

   CRYPT_INVALID_CIPHER,   /* Invalid cipher specified */
   CRYPT_INVALID_HASH,     /* Invalid hash specified */
   CRYPT_INVALID_PRNG,     /* Invalid PRNG specified */

   CRYPT_MEM,              /* Out of memory */

   CRYPT_PK_TYPE_MISMATCH, /* Not equivalent types of PK keys */
   CRYPT_PK_NOT_PRIVATE,   /* Requires a private PK key */

   CRYPT_INVALID_ARG,      /* Generic invalid argument */
   CRYPT_FILE_NOTFOUND,    /* File Not Found */

   CRYPT_PK_INVALID_TYPE,  /* Invalid type of PK key */
   CRYPT_PK_INVALID_SYSTEM,/* Invalid PK system specified */
   CRYPT_PK_DUP,           /* Duplicate key already in key ring */
   CRYPT_PK_NOT_FOUND,     /* Key not found in keyring */
   CRYPT_PK_INVALID_SIZE,  /* Invalid size input for PK parameters */

   CRYPT_INVALID_PRIME_SIZE/* Invalid size of prime requested */
};

