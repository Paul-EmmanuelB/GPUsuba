#ifndef _BTS_UTILS_H_
#define _BTS_UTILS_H_

#include <iostream>
#include <fstream>
#include "sboxE.h"
#include "sboxD.h"
#include "typedef.h"


using namespace std;

/***********************************************************************
                    Checking
************************************************************************/
static const char *_cudaGetErrorEnum(cudaError_t error) {
  return cudaGetErrorName(error);
}

template <typename T>
void check(T result, char const *const func, const char *const file,
           int const line) {
  if (result) {
    fprintf(stderr, "CUDA error at %s:%d code=%d(%s) \"%s\" \n", file, line,
            static_cast<unsigned int>(result), _cudaGetErrorEnum(result), func);
    exit(EXIT_FAILURE);
  }
}

#define checkCudaErrors(val) check((val), #val, __FILE__, __LINE__)

/***********************************************************************
                    Files functions
************************************************************************/

int fsize(char * filename) {
    std::ifstream file(filename, std::ifstream::binary);
	if(!file) {
		perror("Failed to open the file \n");
		exit(1);
	}
	file.seekg(0,file.end);
    int length = file.tellg();
    return length;
}



/***********************************************************************
                    Key scheduling
************************************************************************/
#define Nb 4			// number of columns in the state & expanded key
#define Nk 4			// number of columns in a key
#define Nr 10			// number of rounds in encryption

__constant__ uint32_t const_expkey[1408]; // The expended key to put into the constant memory


const uint32_t rcon[] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x0000001b, 0x00000036 
};


void ExpandKey (uint8 *key, uint8 *expkey) {
	uint8 tmp0, tmp1, tmp2, tmp3, tmp4;
	unsigned idx;

	memcpy (expkey, key, Nk * 4);

	for( idx = Nk; idx < Nb * (Nr + 1); idx++ ) {
		tmp0 = expkey[4*idx - 4];
		tmp1 = expkey[4*idx - 3];
		tmp2 = expkey[4*idx - 2];
		tmp3 = expkey[4*idx - 1];
		if( !(idx % Nk) ) {
			tmp4 = tmp3;
			tmp3 = sbox[tmp0];
			tmp0 = sbox[tmp1] ^ rcon[idx/Nk-1];
			tmp1 = sbox[tmp2];
			tmp2 = sbox[tmp4];
		} else if( Nk > 6 && idx % Nk == 4 ) {
			tmp0 = sbox[tmp0];
			tmp1 = sbox[tmp1];
			tmp2 = sbox[tmp2];
			tmp3 = sbox[tmp3];
		}

		expkey[4*idx+0] = expkey[4*idx - 4*Nk + 0] ^ tmp0;
		expkey[4*idx+1] = expkey[4*idx - 4*Nk + 1] ^ tmp1;
		expkey[4*idx+2] = expkey[4*idx - 4*Nk + 2] ^ tmp2;
		expkey[4*idx+3] = expkey[4*idx - 4*Nk + 3] ^ tmp3;
	}
}


unsigned char rconf(unsigned char in) {
        unsigned char c=1;
        if(in == 0)  
                return 0; 
        while(in != 1) {
		unsigned char b;
		b = c & 0x80;
		c <<= 1;
		if(b == 0x80) {
			c ^= 0x1b;
		}
                in--;
        }
        return c;
}


//Print functions
uint8_t byte_from_8_bits(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,
                         uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7,
                         int stateNum) {
  return
    (( (x0 >> stateNum) & 1) << 0) |
    (( (x1 >> stateNum) & 1) << 1) |
    (( (x2 >> stateNum) & 1) << 2) |
    (( (x3 >> stateNum) & 1) << 3) |
    (( (x4 >> stateNum) & 1) << 4) |
    (( (x5 >> stateNum) & 1) << 5) |
    (( (x6 >> stateNum) & 1) << 6) |
    (( (x7 >> stateNum) & 1) << 7);
}

void print_state_128(uint32_t state[128], int stateNum) {
  for (int i = 0; i < 16; i++) {
      printf("%02X ",byte_from_8_bits(state[i*8],state[i*8+1],state[i*8+2],state[i*8+3],
                                    state[i*8+4],state[i*8+5],state[i*8+6],state[i*8+7],
                                    stateNum));
  }
  printf("\n");
}

#endif // _BTS_UTILS_H_