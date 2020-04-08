#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <fstream>
#include "sboxE.h"

using namespace std;

/***********************************************************************
                    Files functions
************************************************************************/

unsigned int filesize(char * filename) {
    unsigned int size;
    streampos begin,end;
    ifstream myfile ("filename", ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    myfile.close();
    size = (unsigned int)(end-begin);
    return size;
}



/***********************************************************************
                    Key scheduling
************************************************************************/
#  define AES_MAXNR 14

struct aes_key_st {
#  ifdef AES_LONG
    unsigned long rd_key[4 * (AES_MAXNR + 1)];
#  else
    unsigned int rd_key[4 * (AES_MAXNR + 1)];
#  endif
    int rounds;
};
typedef struct aes_key_st AES_KEY;

const uint32_t rcon[] = {
    0x00000001U, 0x00000002U, 0x00000004U, 0x00000008U,
    0x00000010U, 0x00000020U, 0x00000040U, 0x00000080U,
    0x0000001bU, 0x00000036U 
};


#define GETU32(p) (*((uint32_t*)(p)))

int AES_cuda_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key) {
	uint32_t *rk;
   	int i = 0;
	uint32_t temp;

	if (!userKey || !key) return -1;

	if (bits != 128 && bits != 192 && bits != 256) return -2;

	rk = key->rd_key;

	if (bits==128) key->rounds = 10;
	else if (bits==192) key->rounds = 12;
	else key->rounds = 14;

	rk[0] = GETU32(userKey     );
	rk[1] = GETU32(userKey +  4);
	rk[2] = GETU32(userKey +  8);
	rk[3] = GETU32(userKey + 12);
	if (bits == 128) {
		while (1) {
			temp  = rk[3];
			rk[4] = rk[0] ^ (const_sm_sbox[(temp >>  8) & 0xff]      ) ^ (const_sm_sbox[(temp >> 16) & 0xff] <<  8) ^
					(const_sm_sbox[(temp >> 24)       ] << 16) ^ (const_sm_sbox[(temp      ) & 0xff] << 24) ^ rcon[i];
			rk[5] = rk[1] ^ rk[4];
			rk[6] = rk[2] ^ rk[5];
			rk[7] = rk[3] ^ rk[6];
			if (++i == 10) return 0;
			rk += 4;
		}
	}
	rk[4] = GETU32(userKey + 16);
	rk[5] = GETU32(userKey + 20);
	if (bits == 192) {
		while (1) {
			temp  = rk[5];
			rk[6] = rk[0] ^ (const_sm_sbox[(temp >>  8) & 0xff]      ) ^ (const_sm_sbox[(temp >> 16) & 0xff] <<  8) ^
					(const_sm_sbox[(temp >> 24)       ] << 16) ^ (const_sm_sbox[(temp      ) & 0xff] << 24) ^ rcon[i];
			rk[7] = rk[1] ^ rk[6];
			rk[8] = rk[2] ^ rk[7];
			rk[9] = rk[3] ^ rk[8];
			if (++i == 8) return 0;
			rk[10] = rk[ 4] ^ rk[ 9];
			rk[11] = rk[ 5] ^ rk[10];
			rk += 6;
		}
	}
	rk[6] = GETU32(userKey + 24);
	rk[7] = GETU32(userKey + 28);
	if (bits == 256) {
		while (1) {
			temp = rk[7];
			rk[ 8] = rk[0] ^ (const_sm_sbox[(temp >>  8) & 0xff]      ) ^ (const_sm_sbox[(temp >> 16) & 0xff] <<  8) ^
					(const_sm_sbox[(temp >> 24)       ] << 16) ^ (const_sm_sbox[(temp      ) & 0xff] << 24) ^ rcon[i];
			rk[ 9] = rk[1] ^ rk[ 8];
			rk[10] = rk[2] ^ rk[ 9];
			rk[11] = rk[3] ^ rk[10];
			if (++i == 7) return 0;
			temp = rk[11];
			rk[12]=rk[ 4] ^ (const_sm_sbox[(temp      ) & 0xff]      ) ^ (const_sm_sbox[(temp >>  8) & 0xff] <<  8) ^
					(const_sm_sbox[(temp >> 16) & 0xff] << 16) ^ (const_sm_sbox[(temp >> 24)       ] << 24);
			rk[13]=rk[ 5] ^ rk[12];
			rk[14]=rk[ 6] ^ rk[13];
			rk[15]=rk[ 7] ^ rk[14];

			rk += 8;
        	}
	}
	return 0;
}

int AES_cuda_set_decrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key) {
        uint32_t *rk;
	int i, j, status;
	uint32_t temp;

	/* first, start with an encryption schedule */
	status = AES_cuda_set_encrypt_key(userKey, bits, key);
	if (status < 0)
		return status;

	rk = key->rd_key;

	/* invert the order of the round keys: */
	for (i = 0, j = 4*(key->rounds); i < j; i += 4, j -= 4) {
		temp = rk[i    ]; rk[i    ] = rk[j    ]; rk[j    ] = temp;
		temp = rk[i + 1]; rk[i + 1] = rk[j + 1]; rk[j + 1] = temp;
		temp = rk[i + 2]; rk[i + 2] = rk[j + 2]; rk[j + 2] = temp;
		temp = rk[i + 3]; rk[i + 3] = rk[j + 3]; rk[j + 3] = temp;
	}

	/* apply the inverse MixColumn transform to all round keys but the first and the last: */
	for (i = 1; i < (key->rounds); i++) {
		rk += 4;

		rk[0] =
			Td0_cpu[Te2_cpu[(rk[0]      ) & 0xff] & 0xff] ^
			Td1_cpu[Te2_cpu[(rk[0] >>  8) & 0xff] & 0xff] ^
			Td2_cpu[Te2_cpu[(rk[0] >> 16) & 0xff] & 0xff] ^
			Td3_cpu[Te2_cpu[(rk[0] >> 24)       ] & 0xff];
		rk[1] =
			Td0_cpu[Te2_cpu[(rk[1]      ) & 0xff] & 0xff] ^
			Td1_cpu[Te2_cpu[(rk[1] >>  8) & 0xff] & 0xff] ^
			Td2_cpu[Te2_cpu[(rk[1] >> 16) & 0xff] & 0xff] ^
			Td3_cpu[Te2_cpu[(rk[1] >> 24)       ] & 0xff];
		rk[2] =
			Td0_cpu[Te2_cpu[(rk[2]      ) & 0xff] & 0xff] ^
			Td1_cpu[Te2_cpu[(rk[2] >>  8) & 0xff] & 0xff] ^
			Td2_cpu[Te2_cpu[(rk[2] >> 16) & 0xff] & 0xff] ^
			Td3_cpu[Te2_cpu[(rk[2] >> 24)       ] & 0xff];
		rk[3] =
			Td0_cpu[Te2_cpu[(rk[3]      ) & 0xff] & 0xff] ^
			Td1_cpu[Te2_cpu[(rk[3] >>  8) & 0xff] & 0xff] ^
			Td2_cpu[Te2_cpu[(rk[3] >> 16) & 0xff] & 0xff] ^
			Td3_cpu[Te2_cpu[(rk[3] >> 24)       ] & 0xff];
	}
	return 0;
}


#endif // _UTILS_H_