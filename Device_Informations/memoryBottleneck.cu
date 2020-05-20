#include <cuda_runtime_api.h>
#include <iostream>

#include "utils.h"

typedef unsigned int uint32;

__global__ static void memoryTransfer( uint32* dev_input, uint32* dev_output, 
size_t inputSize)
{

    // Index calculations
    int x           = blockIdx.x * blockDim.x + threadIdx.x;    //global x id
    int y           = blockIdx.y * blockDim.y + threadIdx.y;    //global y id
    int xwidth      = blockDim.x * gridDim.x;                   //X width of the grid
    int global_tid  = y*xwidth + x;

    uint32 w1, w2, w3, w4;

    if(global_tid < inputSize/16) {
        w1 = dev_input[4*global_tid];
        w2 = dev_input[4*global_tid+1];
        w3 = dev_input[4*global_tid+2];
        w4 = dev_input[4*global_tid+3];

        dev_output[4*global_tid  ] = w1;
        dev_output[4*global_tid+1] = w2;
        dev_output[4*global_tid+2] = w3;
        dev_output[4*global_tid+3] = w4;
    }

}


__global__ static void encrypt_Kernel( size_t inputSize)
{
    // Index calculations
    int tid         = threadIdx.y*blockDim.x + threadIdx.x;     //local id
    int x           = blockIdx.x * blockDim.x + threadIdx.x;    //global x id
    int y           = blockIdx.y * blockDim.y + threadIdx.y;    //global y id
    int w           = blockDim.x * gridDim.x;                   //width of the grid
    int global_tid  = y*w + x;                                  //global id

    int blockSize = blockDim.x * blockDim.y; 
    
    uint32_t w1,w2,w3,w4,s1,s2,s3,s4;
    int ROUNDS = 11;

    // store the T-boxes and sbox in shared memory.
    __shared__ uint32_t sm_te1[256], sm_te2[256], sm_te3[256], sm_te4[256];
    __shared__ uint8_t sm_sbox[256];
    
    // Loading shared memory. 256 elements are needed
    int elemPerThread = 256/blockSize;
    
    if ( !elemPerThread && tid<256) {
        //load dev_sm_te1, dev_sm_te2, dev_sm_te3, dev_sm_te4 and
        // sm_sbox to share memory variables sm_te1, sm_te2,
        //sm_te3, sm_te4 and sm_sbox;
        sm_te1[tid]   = tid;
        sm_te2[tid]   = tid;
        sm_te3[tid]   = tid;
        sm_te4[tid]   = tid;
        sm_sbox[tid]  = tid;
    }
    else {
        for(int i=0; i<elemPerThread; i++) {
            sm_te1[tid*elemPerThread  + i]   = tid*(elemPerThread+1) + i;
            sm_te2[tid*elemPerThread  + i]   = tid*(elemPerThread+1) + i;
            sm_te3[tid*elemPerThread  + i]   = tid*(elemPerThread+1) + i;
            sm_te4[tid*elemPerThread  + i]   = tid*(elemPerThread+1) + i;
            sm_sbox[tid*elemPerThread + i]   = tid*(elemPerThread+1) + i;
        }
        int modEPT = 256%blockSize; //256 is not a multiple of blockSize
        if(!modEPT && (tid == blockSize-1)) {
            for(int i=0; i<modEPT; i++) {
                sm_te1[tid*(elemPerThread+1)  + i]   = tid*(elemPerThread+1) + i;
                sm_te2[tid*(elemPerThread+1)  + i]   = tid*(elemPerThread+1) + i;
                sm_te3[tid*(elemPerThread+1)  + i]   = tid*(elemPerThread+1) + i;
                sm_te4[tid*(elemPerThread+1)  + i]   = tid*(elemPerThread+1) + i;
                sm_sbox[tid*(elemPerThread+1) + i]   = tid*(elemPerThread+1) + i;
            }
        }
    }
    __syncthreads();

    // Each thread treat 16 bytes. 
    if(global_tid < inputSize/16) {
        
        //load the cipher blocks, all the global memory transactions are
        //coalesced. The original plain text load from files, due to the read
        //procedure reverse the byte order of the 32-bit words, So a reverse
        //process was necessary.


        w1 = 4*global_tid;
        w2 = 4*global_tid+1;
        w3 = 4*global_tid+2;
        w4 = 4*global_tid+3;

        // First round AddRoundKey: ex-or with round key
        //w1 ^= const_expkey[0];
        //w2 ^= const_expkey[1];
        //w3 ^= const_expkey[2];
        //w4 ^= const_expkey[3];

        // Round transformation: a set of table lookups operations.
        #pragma unroll
        for (int i = 1; i < 10; i++) {
            s1 = (sm_te4[(w4 >> 24)] ^ sm_te3[(w3 >> 16) & 0xFF] ^ sm_te2[(w2 >> 8) & 0xFF] ^ sm_te1[w1 & 0xFF]);
            s2 = (sm_te4[(w1 >> 24)] ^ sm_te3[(w4 >> 16) & 0xFF] ^ sm_te2[(w3 >> 8) & 0xFF] ^ sm_te1[w2 & 0xFF]);
            s3 = (sm_te4[(w2 >> 24)] ^ sm_te3[(w1 >> 16) & 0xFF] ^ sm_te2[(w4 >> 8) & 0xFF] ^ sm_te1[w3 & 0xFF]);
            s4 = (sm_te4[(w3 >> 24)] ^ sm_te3[(w2 >> 16) & 0xFF] ^ sm_te2[(w1 >> 8) & 0xFF] ^ sm_te1[w4 & 0xFF]);

            w1 = s1 ^ const_expkey[i * 4];
            w2 = s2 ^ const_expkey[i * 4 + 1];
            w3 = s3 ^ const_expkey[i * 4 + 2];
            w4 = s4 ^ const_expkey[i * 4 + 3];	
        } 

        // The final round doesn’t include the MixColumns
        s1  = (uint32_t)(sm_sbox[ w1        & 0xFF]);
        s1 |= (uint32_t)(sm_sbox[(w2 >>  8) & 0xFF]) << 8;
        s1 |= (uint32_t)(sm_sbox[(w3 >> 16) & 0xFF]) << 16;
        s1 |= (uint32_t)(sm_sbox[(w4 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s1 ^= const_expkey[(ROUNDS - 1) * 4]; //AddRoundKey

        
        s2  = (uint32_t)(sm_sbox[ w2        & 0xFF]);
        s2 |= (uint32_t)(sm_sbox[(w3 >>  8) & 0xFF]) << 8;
        s2 |= (uint32_t)(sm_sbox[(w4 >> 16) & 0xFF]) << 16;
        s2 |= (uint32_t)(sm_sbox[(w1 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s2 ^= const_expkey[(ROUNDS - 1) * 4 + 1]; //AddRoundKey

        s3  = (uint32_t)(sm_sbox[ w3        & 0xFF]);
        s3 |= (uint32_t)(sm_sbox[(w4 >>  8) & 0xFF]) << 8;
        s3 |= (uint32_t)(sm_sbox[(w1 >> 16) & 0xFF]) << 16;
        s3 |= (uint32_t)(sm_sbox[(w2 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s3 ^= const_expkey[(ROUNDS - 1) * 4 + 2]; //AddRoundKey

        s4  = (uint32_t)(sm_sbox[ w4        & 0xFF]);
        s4 |= (uint32_t)(sm_sbox[(w1 >>  8) & 0xFF]) << 8;
        s4 |= (uint32_t)(sm_sbox[(w2 >> 16) & 0xFF]) << 16;
        s4 |= (uint32_t)(sm_sbox[(w3 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s4 ^= const_expkey[(ROUNDS - 1) * 4 + 3]; //AddRoundKey
    }
}

int main(int argc, char * argv[]) {

    ///////////////////////////////////////////////////////////////
    // command line arguments
    ///////////////////////////////////////////////////////////////
    char *  filename;
    int     threadNum           = 512;  // Threads per block. This is a recommanded number.
    int     blockNum            = 0;    // Number of blocks in the grid

    std::cout << std::endl << "********************************************************************" ;
    std::cout << std::endl << "******                   CUDA Tests                           ******" ;
    std::cout << std::endl << "******                Memory Bottleneck                       ******" ;
    std::cout << std::endl << "********************************************************************" << std::endl << std::endl; 

    if (argc > 1){
        for( int n=1 ; n<argc ; n=n+2 ) {
            if((strcmp(argv[n],"-filename") == 0) && (n+1<argc)) {
                filename = argv[n+1];
            }
            else if((strcmp(argv[n],"-threadNum") == 0) && (n+1<argc)) {
                threadNum = atoi(argv[n+1]);
                if(threadNum ==0) {
                    printf("\n threadNum must be a non-null value.\n");
                    exit(1);
                }
            }
            else if((strcmp(argv[n],"-blockNum") == 0) && (n+1<argc)) {
                blockNum = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-help") == 0)) {
                std::cout << "   This is a CUDA test program." << std::endl;
				std::cout << "   \"-options value\" availables are:" << std::endl;
                std::cout << "   -filename, the file path to encrypt or decrypt." << std::endl;
                std::cout << "   -threadNum to set the number of threads per block. Default recommended value is 512." << std::endl;
                std::cout << "   -blockNum to set the number of blocks in the grid. Default value is 0 and will create enough blocks taking into account the input file size and the threadNum argument." << std::endl;
                std::cout << "   The order between options is not important." << std::endl << std::endl;
                exit(0);
            }
            else {
                std::cout << std::endl << "Argument " << argv[n] << " does not correspond to any valid arguments. Type -help for details about valid command line arguments." <<std::endl;
		        exit(1);
            }
        }


    }
	else {
		std::cout << std::endl << std::endl << "Not enough arguments. Type -help option in the command line for further explanations." << std::endl;
		exit(1);
	}

    std::cout << "    threadNum		= " << threadNum        << std::endl;
    std::cout << "    blockNum		= " << blockNum         << std::endl;
    std::cout << "    Filename		= " << filename  << std::endl << std::endl;

    // ***Inputdata file to encrypt/decrypt***
    //Checking for the size of the file
    int filesize;
    filesize = fsize(filename);
    uint8_t *inputData, *outputData;
    inputData = (uint8_t*)malloc((filesize)*sizeof(uint8_t));
    outputData = (uint8_t*)malloc((filesize)*sizeof(uint8_t));

    //Opening the file
    FILE * inputFile;
    int result;
    inputFile = fopen(filename,"rb");

    if (inputFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    result = fread (inputData, sizeof(uint8_t), filesize, inputFile);
    if(result != filesize) {
        perror("Reading error from the input file");
        exit(1);
    }
    fclose(inputFile);


    if(!blockNum) {
        blockNum = 1+filesize/(threadNum);
    }

    std::cout << "    Gridsize in term of block: " << blockNum  << std::endl;


    //Device vectors declarations and allocations
    uint32 * devInput, * devOutput;
    cudaMalloc( (void **) &devInput         , filesize*sizeof(uint8_t));
    cudaMalloc( (void **) &devOutput        , filesize*sizeof(uint8_t));


    //To record the device time execution
    cudaEvent_t startDevice, stopDevice;
	checkCudaErrors(cudaEventCreate(&startDevice));
	checkCudaErrors(cudaEventCreate(&stopDevice));

    //checkCudaErrors(cudaEventRecord(startHost, NULL));
    cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);

    //Warm Up
    for(int i=0; i < 1000 ; i++) { 
        encrypt_Kernel<<<blockNum,threadNum>>>(filesize);
    }
    
    checkCudaErrors(cudaEventRecord(startDevice, NULL));
    for(int j=0; j<1000; j++){ //for benchmarking
        encrypt_Kernel<<<blockNum,threadNum>>>(filesize);
    }			
    checkCudaErrors(cudaEventRecord(stopDevice, NULL));

    //Copy results from host memory to device memory
	cudaMemcpy(outputData, devOutput, filesize*sizeof(uint8_t), cudaMemcpyDeviceToHost);

    checkCudaErrors(cudaEventSynchronize(stopDevice));


    //Time calculation
    float Devmsec   = 0.0f;
    checkCudaErrors(cudaEventElapsedTime(&Devmsec, startDevice, stopDevice));
    Devmsec /= 1000;
    double throughput = 1.0e-9f*8*filesize/(Devmsec*1.0e-3f);
    printf("\n	GPU processing time: %f (ms)", Devmsec);
    printf("\n	GPU throughput: %f (Gbps)\n", throughput);


    //Free host memory
    free(inputData);
    free(outputData);
    // Free device memory
    cudaFree(devInput);
    cudaFree(devOutput);

}