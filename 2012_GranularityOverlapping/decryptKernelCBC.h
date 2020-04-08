/*
CBC decryption mode
*/

__global__ static void decrypt_Kernel( u32_t *dev_input,
u32_t* dev_output, size_t pitch_a, size_t pitch_b, u32_t* dev_sm_td1,
u32_t* dev_sm_td2, u32_t* dev_sm_td3, u32_t* dev_sm_td4 )
{
    // local thread index and global index
    int tid         = threadIdx.x;
    int blockId     = gridDim.x*blockIdx.y  + blockIdx.x;
    int global_tid  = blockDim.x*blockId    + tid;
    u32_t w1,w2,w3,w4,s1,s2,s3,s4;

    // store the T-boxes and sbox in share memory.
    __shared__ u32_t sm_td1[256], sm_td2[256], sm_td3[256], sm_td4[256];
    __shared__ u8_t sm_isbox[256];

    // store the ciphertext blocks for the later ex-or operations.
    __shared__ u32_t iv_1[global_tid], iv_2[global_tid], iv_3[global_tid], iv_4[global_tid] ;

    if (tid<256){
    //Load dev_sm_td1, dev_sm_td2, dev_sm_td3, dev_sm_td4 and
    // const_sm_isbox to share memory variables sm_td1, sm_td2,
    //sm_td3, sm_td4 and sm_isbox;
        sm_te1[global_tid]   = dev_sm_te1[global_tid];
        sm_te2[global_tid]   = dev_sm_te2[global_tid];
        sm_te3[global_tid]   = dev_sm_te3[global_tid];
        sm_te4[global_tid]   = dev_sm_te4[global_tid];
        sm_isbox[global_tid] = const_sm_sbox_inv[global_tid];
    }
    // first thread of each block store the cipher blocks that corresponding
    // to the last thread of the previous block to share memory.
    if(tid = = 0 & global_tid !=0 ){
    w1 = dev_input[global_tid-1];
    iv_1[0] = ( (w1>>24)&0xFF)|(( w1>>8)&0xFF00)|
    ( (w1<<8)&0xFF0000)|( (w1<<24)&0xFF000000);
    
    w2 = dev_input[global_tid-2];
    iv_2[0] = ( (w2>>24)&0xFF)|(( w2>>8)&0xFF00)|
    ( (w2<<8)&0xFF0000)|( (w2<<24)&0xFF000000);
    
    w3 = dev_input[global_tid-3];
    iv_3[0] = ( (w3>>24)&0xFF)|(( w3>>8)&0xFF00)|
    ( (w3<<8)&0xFF0000)|( (w3<<24)&0xFF000000);
    
    w4 = dev_input[global_tid-4];
    iv_4[0] = ( (w4>>24)&0xFF)|(( w4>>8)&0xFF00)|
    ( (w4<<8)&0xFF0000)|( (w4<<24)&0xFF000000);
    
    }
    //the first thread of the whole grid load the initialization vector (IV).
    if(global_tid==0){
    iv_1[0] = dev_iv[0];
    iv_2[0] = dev_iv[1];
    iv_3[0] = dev_iv[2];
    iv_4[0] = dev_iv[3];
    }
    //Load the blocks and reverses the byte order of the 32-bit words.
    w1 = dev_input[global_tid];
    w1= ((w1>>24)&0xFF)|(( w1>>8)&0xFF00)|( (w1<<8)&0xFF0000)|
    ( (w1<<24)&0xFF00 0000;
    ...
    if(tid<THREAD_NUM-1){
    //store the current cipher block in share memory for later usage.
    iv_1[tid+1] = w1;
    ...
    }
    if(global_tid==THREAD_NUM*BLOCK_NUM-1){
    //the final thread of a grid store the last cipher block to
    //global memory for next kernel.
    dev_iv[0] = w1;
    ...
    }
    // AddRoundKey: ex-or with round key
    w1 ^= const_m_ke[0];
    ...
    // round transformation: a set of table lookups operations.
    #pragma unroll
    for (int i = 1; i < 10; i++) {
    s1 = (sm_td1[w1 >> 24] ^ sm_td2[(w2 >> 16) & 0xFF] ^ sm_td3
    [(w3 >> 8) & 0xFF] ^ sm_td4[w4 & 0xFF]) ^
    const_m_ke[i*4];
    w1 = s1;
    ...
    }
    w1 = (u32_t)(sm_isbox[ s1 >> 24 ]) << 24; //SubBytes and ShiftRows
    w1 |= (u32_t)(sm_isbox[(s4 >> 16)& 0xFF]) << 16;
    w1 |= (u32_t)(sm_isbox[(s3 >> 8)& 0xFF]) << 8 ;
    w1 |= (u32_t)(sm_isbox[s2 & 0xFF]);
    //AddRoundKey and ex-or with the corresponding ciphertext to recover
    //the the exact plaintext.
    w1 ^= iv_1[tid]^const_m_kd[ROUNDS*4];
    w1 = ( (w1>>24)&0xFF)|(( w1>>8)&0xFF00)|( (w1<<8)&0xFF0000)
    |( (w1<<24)&0xFF000000;
    dev_output[global_tid] = w1 ; //store the results
    ...
}