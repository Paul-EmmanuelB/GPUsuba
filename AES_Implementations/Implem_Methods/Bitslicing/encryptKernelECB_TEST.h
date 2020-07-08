#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

//#define Opti

#include "BtsUtils.h"
#include "addRoundKey.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "mixColumns.h"

//* Return a byte from 8 32-bit registers
__device__ uint8_t dev_byte_from_8_bits(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,
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

//* Print the state of a transposed state
__device__ void dev_print_state_128(uint32_t state[128], int stateNum) {
  for (int i = 0; i < 16; i++) {
      printf("%02X ",dev_byte_from_8_bits(state[i*8],state[i*8+1],state[i*8+2],state[i*8+3],
                                    state[i*8+4],state[i*8+5],state[i*8+6],state[i*8+7],
                                    stateNum));
  }
  printf("\n");
} 


//__device__ inline void addRoundKey(uint32_t shared[128], uint32_t sm_key[128])  __attribute__((always_inline));
//__device__ inline void shiftRows(uint32_t shared[128])  __attribute__((always_inline));
//__device__ inline void Mixcl(uint32_t shared[]) __attribute__((always_inline));
//__device__ inline void Sbox(uint32_t *shared) __attribute__((always_inline));


#ifdef Opti
__global__ void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, size_t inputSize)
{
    //* Index calculations
    int tid             = threadIdx.x;                           //** localID
    int laneID          = tid%warpSize;                          //** laneID 
    int global_warpID   = (blockIdx.x*blockDim.x+tid)/warpSize;  //** global warpID
    int warps           = (blockDim.x*gridDim.x)/warpSize;       //** Warps in the grid
    int elements        = inputSize/16384;                       //** One warp treat 16384 bytes
    
    //* store the expended key in shared memory.
    //__shared__ uint32_t sm_key[1408];
    //extern __shared__ uint32_t plain[]; 
    

    //** LOADING Expended key into shared memory
    //#pragma unroll
    //for(int i = tid; i < 1408; i += blockDim.x) {
    //    //load the extended key into the shared memory
    //    sm_key[i]   = const_expkey[i];
    //}

    //if(tid < 64){
    //    for(int j = 0; j < 22; j++){
    //        sm_key[64*j+tid]   = const_expkey[64*j+tid];
    //    }
    //}
    //__syncthreads();


    for(int i = global_warpID; i < elements; i += warps) {
        //* Loading plaintext
        register uint32_t r[128];

        //PRINT
        //if(blockIdx.x*blockDim.x+tid==0){
        //    printf("\nState\n");
        //    dev_print_state_128(r,0);
        //    /*for(int a=0; a<128; a++){
        //        if(a%32==0)
        //            printf("\n");
        //        printf("%x ",r[a]);
        //    }
        //}
       
        #pragma unroll
        for(int k=0; k<128; k++){
			r[k] = dev_input[i*4096+k*32+laneID];
            addRoundKey(r[k], const_expkey[k]);
        }

        //BIG LOOP
        for (int j = 1; j < 10; j++) {

            //** SubBytes + Shift
            #pragma unroll(4)
            for(int l=0; l<4; l++){
                Sbox( &r[8+l*32] );
            }
            /*          
            Sbox( &r[0  ] );
            Sbox( &r[8  ] );
            Sbox( &r[40 ] );
            Sbox( &r[72 ] );
            Sbox( &r[104] ); */
            shiftFirstRow(  &r[8  ], &r[40 ], &r[72 ], &r[104]);

            #pragma unroll(4)
            for(int l=0; l<4; l++){
                Sbox( &r[16+l*32] );
            }
         /*   
            Sbox( &r[16 ] );
            Sbox( &r[32 ] );
            Sbox( &r[48 ] );
            Sbox( &r[64 ] );
            Sbox( &r[80 ] );
            Sbox( &r[112] );    */
            shiftSecondRow( &r[16 ], &r[80 ], &r[48 ], &r[112]);

            #pragma unroll(4)
            for(int l=0; l<4; l++){
                Sbox( &r[24+l*32] );
            }
            /*      
            Sbox( &r[96 ] );
            Sbox( &r[24 ] );
            Sbox( &r[56 ] );
            Sbox( &r[88 ] );
            Sbox( &r[120] );    */
            shiftThirdRow(  &r[24 ], &r[120], &r[88 ], &r[56 ]);

            #pragma unroll(4)
            for(int l=0; l<4; l++){
                Sbox( &r[l*32] );
            }

            //** Mix + RoundKey
            //#pragma unroll(4)
            //for(int l=0; l<4; l++){
            //    Mixcl( &r[l*32 ] );
            //    addRoundKey( &r[l*32 ],  &const_expkey[128*j+l*32]);
            //}            
            Mixcl( &r[0] );
            addRoundKey( &r[0 ], &const_expkey[128*j+0]);
            Mixcl( &r[32] );
            addRoundKey( &r[32 ], &const_expkey[128*j+32]);
            Mixcl( &r[64] );
            addRoundKey( &r[64 ], &const_expkey[128*j+64]);
            Mixcl( &r[96] );
            addRoundKey( &r[96 ], &const_expkey[128*j+96]);
            
            //* addRoundKey
            //addRoundKey(r, &const_expkey[128*j]);
        }

        //** FINAL ROUND
        //#pragma unroll(4)
        for(int l=0; l<4; l++){
            Sbox( &r[l*32] );
        }
        /*
        Sbox( &r[0  ] );
        Sbox( &r[32 ] );
        Sbox( &r[64 ] );
        Sbox( &r[96 ] ); */

        //#pragma unroll
        for(int k=0; k<4; k++){
            #pragma unroll(8)
            for(int l=0; l<8; l++){
                int m = k*32+l;
                addRoundKey(r[m], const_expkey[1280+m]);
			    dev_output[i*4096+m*32+laneID] = r[m];        
            }
		}

        //#pragma unroll(4)
        for(int l=0; l<4; l++){
            Sbox( &r[8+l*32] );
        }
        /* Sbox( &r[8  ] );
        Sbox( &r[40 ] );
        Sbox( &r[72 ] );
        Sbox( &r[104] ); */
        shiftFirstRow(  &r[8  ], &r[40 ], &r[72 ], &r[104]);

        //#pragma unroll
        for(int k=0; k<4; k++){
            #pragma unroll(8)
            for(int l=0; l<8; l++){
                int m = k*32+8+l;
                addRoundKey(r[m], const_expkey[1280+m]);
			    dev_output[i*4096+m*32+laneID] = r[m];        
            }
		}

        //#pragma unroll(4)
        for(int l=0; l<4; l++){
            Sbox( &r[16+l*32] );
        }
        /* Sbox( &r[16 ] );
        Sbox( &r[80 ] );
        Sbox( &r[48 ] );
        Sbox( &r[112] ); */
        shiftSecondRow( &r[16 ], &r[80 ], &r[48 ], &r[112]);

        //#pragma unroll
        for(int k=0; k<4; k++){
            #pragma unroll(8)
            for(int l=0; l<8; l++){
                int m = k*32+16+l;
                addRoundKey(r[m], const_expkey[1280+m]);
			    dev_output[i*4096+m*32+laneID] = r[m];        
            }
		}

        //#pragma unroll(4)
        for(int l=0; l<4; l++){
            Sbox( &r[24+l*32] );
        }
        /* Sbox( &r[24 ] );
        Sbox( &r[56 ] );
        Sbox( &r[88 ] );
        Sbox( &r[120] ); */
        shiftThirdRow(  &r[24 ], &r[120], &r[88 ], &r[56 ]);

        //#pragma unroll
        for(int k=0; k<4; k++){
            #pragma unroll(8)
            for(int l=0; l<8; l++){
                int m = k*32+24+l;
                addRoundKey(r[m], const_expkey[1280+m]);
			    dev_output[i*4096+m*32+laneID] = r[m];        
            }
		}
    
    }
}

#else
__global__ void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, size_t inputSize)
{

    //* Index calculations
    int tid             = threadIdx.x;                           //** localID
    int laneID          = tid%warpSize;                          //** laneID 
    int global_warpID   = (blockIdx.x*blockDim.x+tid)/warpSize;  //** global warpID
    int warps           = (blockDim.x*gridDim.x)/warpSize;       //** Warps in the grid
    int elements        = inputSize/16384;                       //** One warp treat 16384 bytes
    
    for(int i = global_warpID; i < elements; i += warps) {
        //* Loading plaintext
        register uint32_t r[128];
        //* Input
        #pragma unroll
        for(int k=0; k<128; k++){
            r[k] = dev_input[i*4096+k*32+laneID];        
            addRoundKey(r[k], const_expkey[k]);
        }
        printf("\nState\n");
        //PRINT
        if(blockIdx.x*blockDim.x+tid==0){
            printf("\nState\n");
            dev_print_state_128(r,0);
            /*for(int a=0; a<128; a++){
                if(a%32==0)
                    printf("\n");
                printf("%x ",r[a]);
            }*/

        //* Round transformation: a set of table lookups operations.
        for (int j = 1; j < 10; j++) {
            //* subBytes
            Sbox( &r[0  ] );
            Sbox( &r[8  ] );
            Sbox( &r[16 ] );
            Sbox( &r[24 ] );
            Sbox( &r[32 ] );
            Sbox( &r[40 ] );
            Sbox( &r[48 ] );
            Sbox( &r[56 ] );
            Sbox( &r[64 ] );
            Sbox( &r[72 ] );
            Sbox( &r[80 ] );
            Sbox( &r[88 ] );
            Sbox( &r[96 ] );
            Sbox( &r[104] );
            Sbox( &r[112] );
            Sbox( &r[120] );
            
            //* shiftRows
            shiftRows( r );

            //* mixcolums
            Mixcl( &r[0 ] );
            Mixcl( &r[32] );
            Mixcl( &r[64] );
            Mixcl( &r[96] );
            
            //* addRoundKey
            //addRoundKey(r, &const_expkey[j*128]);
            #pragma unroll
            for(int k=0; k<128; k++){
                addRoundKey(r[k], const_expkey[j*128+k]);
            }
        } 

        //* The final round doesn’t include the MixColumns step
        //* subBytes   
        Sbox( &r[0  ] );
        Sbox( &r[8  ] );
        Sbox( &r[16 ] );
        Sbox( &r[24 ] );
        Sbox( &r[32 ] );
        Sbox( &r[40 ] );
        Sbox( &r[48 ] );
        Sbox( &r[56 ] );
        Sbox( &r[64 ] );
        Sbox( &r[72 ] );
        Sbox( &r[80 ] );
        Sbox( &r[88 ] );
        Sbox( &r[96 ] );
        Sbox( &r[104] );
        Sbox( &r[112] );
        Sbox( &r[120] );

        //* shiftRows
        shiftRows( r );

        //* Output
        #pragma unroll
        for(int k=0; k<128; k++){
            addRoundKey(r[k], const_expkey[1280+k]);
            dev_output[i*4096+k*32+laneID] = r[k];        
        }
    }
}
#endif // Opti

#endif //_ENCRYPT_ECB_H_
