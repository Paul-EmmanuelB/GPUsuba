
__global__ static void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, 
size_t inputSize, uint32_t* dev_sm_te1,
uint32_t* dev_sm_te2, uint32_t* dev_sm_te3, uint32_t* dev_sm_te4 )
{

    // local thread index and global index
    int tid         = threadIdx.x;
    int blockId     = gridDim.x*blockIdx.y  + blockIdx.x;
    int global_tid  = blockDim.x*blockId    + tid;
    uint32_t w1,w2,w3,w4,s1,s2,s3,s4;

    if(global_tid < inputSize/16) {
        
        // store the T-boxes and sbox in shared memory.
        __shared__ uint32_t sm_te1[256], sm_te2[256], sm_te3[256], sm_te4[256];
        __shared__ uint8_t sm_sbox[256];
        
        if (tid<256){
            //load dev_sm_te1, dev_sm_te2, dev_sm_te3, dev_sm_te4 and
            // const_sm_sbox to share memory variables sm_te1, sm_te2,
            //sm_te3, sm_te4 and sm_sbox;
            sm_te1[global_tid]   = dev_sm_te1[global_tid];
            sm_te2[global_tid]   = dev_sm_te2[global_tid];
            sm_te3[global_tid]   = dev_sm_te3[global_tid];
            sm_te4[global_tid]   = dev_sm_te4[global_tid];
            sm_sbox[global_tid]  = const_sm_sbox[global_tid];
        }
        
        //load the cipher blocks, all the global memory transactions are
        //coalesced. The original plain text load from files, due to the read
        //procedure reverse the byte order of the 32-bit words, So a reverse
        //process was necessary.
        w1 = dev_input[4*global_tid];
        w1= ((w1>>24)&0xFF)|(( w1>>8)&0xFF00)|( (w1<<8)&0xFF0000)|
        ( (w1<<24)&0xFF00 0000;
        
        w2 = dev_input[4*global_tid+1];
        w2= ((w2>>24)&0xFF)|(( w2>>8)&0xFF00)|( (w2<<8)&0xFF0000)|
        ( (w2<<24)&0xFF00 0000;

        w3 = dev_input[4*global_tid+2];
        w3= ((w3>>24)&0xFF)|(( w3>>8)&0xFF00)|( (w3<<8)&0xFF0000)|
        ( (w3<<24)&0xFF00 0000;

        w4 = dev_input[4*global_tid+3];
        w4= ((w4>>24)&0xFF)|(( w4>>8)&0xFF00)|( (w4<<8)&0xFF0000)|
        ( (w4<<24)&0xFF00 0000;

        // first round AddRoundKey: ex-or with round key
        w1 ^= const_m_ke[0];
        w2 ^= const_m_ke[1];
        w3 ^= const_m_ke[2];
        w4 ^= const_m_ke[3];

        // round transformation: a set of table lookups operations.
        #pragma unroll
        for (int i = 1; i < 10; i++) {
            s1 = (sm_te1[w1 >> 24] ^ sm_te2[(w2 >> 16) & 0xFF] ^
            sm_te3[(w3 >> 8) & 0xFF] ^ sm_te4[w4 & 0xFF]) ^
            const_m_ke[i*4];
            w1 = s1;

            s2 = (sm_te1[w1 >> 24] ^ sm_te2[(w2 >> 16) & 0xFF] ^
            sm_te3[(w3 >> 8) & 0xFF] ^ sm_te4[w4 & 0xFF]) ^
            const_m_ke[i*4+1];
            w2 = s2;
            
            s3 = (sm_te1[w1 >> 24] ^ sm_te2[(w2 >> 16) & 0xFF] ^
            sm_te3[(w3 >> 8) & 0xFF] ^ sm_te4[w4 & 0xFF]) ^
            const_m_ke[i*4+2];
            w3 = s3;
            
            s4 = (sm_te1[w1 >> 24] ^ sm_te2[(w2 >> 16) & 0xFF] ^
            sm_te3[(w3 >> 8) & 0xFF] ^ sm_te4[w4 & 0xFF]) ^
            const_m_ke[i*4+3];
            w4 = s4;
            
        } 
        
        //The final round doesn’t include the MixColumns
        w1 = (uint32_t)(sm_sbox[ s1 >> 24 ]) << 24; //SubBytes and ShiftRows
        w1 |= (uint32_t)(sm_sbox[(s2 >> 16)& 0xFF]) << 16;
        w1 |= (uint32_t)(sm_sbox[(s3 >> 8)& 0xFF]) << 8 ;
        w1 |= (uint32_t)(sm_sbox[s4 & 0xFF]);
        w1 ^= const_m_ke[ROUNDS*4]; //AddRoundKey
        w1 =( (w1>>24)&0xFF)|(( w1>>8)&0xFF00)|( (w1<<8)&0xFF0000)
        |( (w1<<24)&0xFF000000;
        dev_output[4*global_tid] = w1 ; //store the cipher text
        
        w2 = (uint32_t)(sm_sbox[ s1 >> 24 ]) << 24; //SubBytes and ShiftRows
        w2 |= (uint32_t)(sm_sbox[(s2 >> 16)& 0xFF]) << 16;
        w2 |= (uint32_t)(sm_sbox[(s3 >> 8)& 0xFF]) << 8 ;
        w2 |= (uint32_t)(sm_sbox[s4 & 0xFF]);
        w2 ^= const_m_ke[ROUNDS*4]; //AddRoundKey
        w2 =( (w2>>24)&0xFF)|(( w2>>8)&0xFF00)|( (w2<<8)&0xFF0000)
        |( (w2<<24)&0xFF000000;
        dev_output[4*global_tid+1] = w2 ; //store the cipher text

        w3 = (uint32_t)(sm_sbox[ s1 >> 24 ]) << 24; //SubBytes and ShiftRows
        w3 |= (uint32_t)(sm_sbox[(s2 >> 16)& 0xFF]) << 16;
        w3 |= (uint32_t)(sm_sbox[(s3 >> 8)& 0xFF]) << 8 ;
        w3 |= (uint32_t)(sm_sbox[s4 & 0xFF]);
        w3 ^= const_m_ke[ROUNDS*4]; //AddRoundKey
        w3 =( (w3>>24)&0xFF)|(( w3>>8)&0xFF00)|( (w3<<8)&0xFF0000)
        |( (w3<<24)&0xFF000000;
        dev_output[4*global_tid+2] = w3 ; //store the cipher text

        w4 = (uint32_t)(sm_sbox[ s1 >> 24 ]) << 24; //SubBytes and ShiftRows
        w4 |= (uint32_t)(sm_sbox[(s2 >> 16)& 0xFF]) << 16;
        w4 |= (uint32_t)(sm_sbox[(s3 >> 8)& 0xFF]) << 8 ;
        w4 |= (uint32_t)(sm_sbox[s4 & 0xFF]);
        w4 ^= const_m_ke[ROUNDS*4]; //AddRoundKey
        w4 =( (w4>>24)&0xFF)|(( w4>>8)&0xFF00)|( (w4<<8)&0xFF0000)
        |( (w4<<24)&0xFF000000;
        dev_output[4*global_tid+3] = w4 ; //store the cipher text
    }
}