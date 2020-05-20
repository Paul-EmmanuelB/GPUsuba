/*

From Jia, Z., Maggioni, M., Smith, J., & Scarpazza, D. P. (2019). Dissecting the NVidia Turing T4 GPU via Microbenchmarking

*/



__global__ void KernelFunction() {
    //declare shared memory space
    __shared__ unsigned int s_tvalue[];
    __shared__ undigned int s_index[];
    //preheat the data
    for(it=0; it<iterations; it++) {
        start_time=clock();
        j=my_array[j];
        //store the array index
        s_index[it]=j;
        end_time=clock();
        //store the access latency
        s_tvalue[it]=end_time-start_time;
    }
}