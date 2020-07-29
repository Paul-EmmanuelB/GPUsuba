/*
Code from Nvidia programming guide
*/


#ifndef _OCCUPANCY_H_
#define _OCCUPANCY_H_

template<class T >
void kernelOccupancy(T func, int blockSize){
    int numBlocks;        // Occupancy in terms of active blocks

    // These variables are used to convert occupancy to warps
    int device;
    cudaDeviceProp prop;
    int activeWarps;
    int maxWarps;

    cudaGetDevice(&device);
    cudaGetDeviceProperties(&prop, device);
    
    cudaOccupancyMaxActiveBlocksPerMultiprocessor(
        &numBlocks,
        func,
        blockSize,
        0);

    activeWarps = numBlocks * blockSize / prop.warpSize;
    maxWarps = prop.maxThreadsPerMultiProcessor / prop.warpSize;

    std::cout << "Occupancy: " << (double)activeWarps / maxWarps * 100 << "%" << std::endl;
}


template<class T, typename... Args>
void launchMyKernel(T func, int maxBlockSize, Args... args)
{
    int blockSize;      // The launch configurator returned block size
    int minGridSize;    // The minimum grid size needed to achieve the
                        // maximum occupancy for a full device
                        // launch
    int gridSize;       // The actual grid size needed, based on input
                        // size

    cudaOccupancyMaxPotentialBlockSize(
        &minGridSize,
        &blockSize,
        (void*)func,
        0,
        maxBlockSize);

    std::cout << std::endl << "Max potential block size to achieve the best potential occupancy :   "<< std::endl;
    std::cout << "block size :  " << blockSize << std::endl;
    std::cout << "minGridSize :  " << minGridSize << std::endl;

    // Round up according to array size
    gridSize = (maxBlockSize + blockSize - 1) / blockSize;

    std::cout << "gridSize launched  :  " << gridSize << std::endl;

    func<<<gridSize, blockSize>>>(args...);
    cudaDeviceSynchronize();
}

#endif //_OCCUPANCY_H_