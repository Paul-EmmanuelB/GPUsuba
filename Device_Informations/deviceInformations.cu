/*

Print all basic device informations you could need. 

*/

#include <cstdio>
#include <cuda_runtime_api.h>



// To prevent calls from errors
#define CUDA_SAFE_CALL_NO_SYNC(x)                                			\
  do {                                                            			\
    cudaError_t err = x;			                              			\
    if (err != cudaSuccess) {                                 	  			\
		fprintf(stderr, "Cuda error in file '%s' in line %i : %s.\n",       \
		            __FILE__, __LINE__, cudaGetErrorString( err) );         \
		exit(EXIT_FAILURE);                                                 \
    }                                                             			\
  } while(0)




void deviceInfo() {

	int deviceCount;
	cudaDeviceProp deviceProp;
                                                     
    CUDA_SAFE_CALL_NO_SYNC(cudaGetDeviceCount(&deviceCount));          
    if (deviceCount == 0) {                                                  
        fprintf(stderr, "There is no device.\n");                            
        exit(EXIT_FAILURE);
	}
	
	for(int dev=0 ; dev < deviceCount ; dev++){
		CUDA_SAFE_CALL_NO_SYNC( cudaGetDeviceProperties(&deviceProp, dev) );
		printf("	Device Number:						%d\n"					, dev);
		printf("	Name: 							%s\n"						, deviceProp.name);
		//printf("	Unique identifier 							%lu\n"		, deviceProp.uuid);
		printf("	Total Global memory (bytes)				%-lu\n"				, deviceProp.totalGlobalMem);
		printf("	Shared memory per block (bytes)				%-10lu\n"		, deviceProp.sharedMemPerBlock);		
		printf("	Maximum 32-bits registers per block			%d\n"			, deviceProp.regsPerBlock);
		printf("	Warp size (threads)					%d\n"					, deviceProp.warpSize);
		printf("	Max threads per block					%d\n"				, deviceProp.maxThreadsPerBlock);
		printf("	Max threads in x dimension				%d\n"				, deviceProp.maxThreadsDim[0]);
		printf("	Max threads in y dimension				%d\n"				, deviceProp.maxThreadsDim[1]);
		printf("	Max threads in z dimension				%d\n"				, deviceProp.maxThreadsDim[2]);
		printf("	Where x*y*z					<=	1024\n"		);
		printf("	Max grid size (block)					%d\n"				, deviceProp.maxGridSize[0]);
		printf("	Max grid size (block)					%d\n"				, deviceProp.maxGridSize[1]);
		printf("	Max grid size (block)					%d\n"				, deviceProp.maxGridSize[2]);
		printf("	Clock rate (kHZ)					%d\n"					, deviceProp.clockRate);
		printf("	Total constant memory (bytes)				%lu\n"			, deviceProp.totalConstMem);
		printf("	Major							%d\n"						, deviceProp.major);
		printf("	Minor							%d\n"						, deviceProp.minor);
		printf("	Multiprocessor count					%d\n"				, deviceProp.multiProcessorCount);
		printf("	Run time limit for kernel execution			%d\n"			, deviceProp.kernelExecTimeoutEnabled );
		printf("	Integrated GPU (motherboard)				%d\n"			, deviceProp.integrated);
		printf("	Mapping host mem into CUDA adress space			%d\n"		, deviceProp.canMapHostMemory);
		printf("	Compute mode						%d\n"					, deviceProp.computeMode);
		printf("	Memory Clock Rate (KHz): 				%d\n"				, deviceProp.memoryClockRate);
		printf("	Memory Bus Width (bits): 				%d\n"				, deviceProp.memoryBusWidth);
		printf("	Peak Memory Bandwidth (GB/s): 				%f\n"			, 2.0*deviceProp.memoryClockRate*(deviceProp.memoryBusWidth/8)/1.0e6); //Double rate memory explain the x2
		printf("	L2 cache size (bytes)					%d\n"				, deviceProp.l2CacheSize);
		printf("	Max resident threads per multiprocessor			%d\n"		, deviceProp.maxThreadsPerMultiProcessor);
		printf("	Supports caching globals in L1				%d\n"			, deviceProp.globalL1CacheSupported);
		printf("	Supports caching locals in L1				%d\n"			, deviceProp.localL1CacheSupported);
		printf("	Shared memory per MP (bytes)				%lu\n"			, deviceProp.sharedMemPerMultiprocessor); //Shared by all threads blocks simultaneously resident on a multiprocessor
		printf("	32-bits registers per MP				%d\n"				, deviceProp.regsPerMultiprocessor); //Shared by all threads blocks simultaneously resident on a multiprocessor


	}	

}



int main(int argc, char *argv[]){
	
	deviceInfo();	

	return 0;
}

