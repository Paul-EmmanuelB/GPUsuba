This folder displays several files from *aes* executable. Differences between them are based on the tool used and also on some options. 
More explanations and illustrations about those tools or options are available on [Nvidia's website](https://docs.nvidia.com/cuda/cuda-binary-utilities/index.html "Cuda binary utilities").

#### cuobjdump

Files beginning with *cuobjdump* use this precise tool. This tool can deal with both host and device code. But it will give less informations than its alternative called *nvdisasm*.

**-elf**    option - also included in concerned filename - gives from the executable all cuda elf sections in a human readable format.  
**-ptx**    gives ptx assembly code. 
**-sass**   focuses only on cubin (cuda bin) files and give the SASS assembly code of included kernels.  
**-res-usage** gives resource usage for each ELF (registers, stack, local, constant, shared, texture, surface, sampler).

We obtain text files about the SASS and PTX assembly code in `cuobj` folder. 

#### nvdisasm   

This extraction tool works only on cubin files. If we have an executable with host code, we can use *cuobjdump* to extract cubin files :
```shell
 >> cuobjdump executable_filename -xelf all
```

We can also use *-lelf* option to get the list of cubin files embedded in the executable file. Then we can extract a precise cubin file with `-xelf cubin_filename` or even with words contained in cubin_filename. 

In our case, the files extracted are  *aes.1.sm_30.cubin* and *aes.2.sm_30.cubin*. The first one contains only few  informations such as :

```shell
.headerflags    @"EF_CUDA_TEXMODE_UNIFIED EF_CUDA_64BIT_ADDRESS EF_CUDA_SM62 EF_CUDA_VIRTUAL_SM(EF_CUDA_SM62)"
        .elftype        @"ET_EXEC" 
``` 
The other cubin file posess assembly code for both kernels and can be read in `nvdisasm` folder obtained with options explained below.  

When extraction is done, we can apply the nvdisasm tool. It only deals with one input and with options as :  

**-cfg** to obtain the control flow graph on a kernel. 
**-plr** print several details during the use of registers along the corresponding disassembly code in a specefic column.
**-g**   annotate disassembly code with source line information obtained from .debug_line section, if present.
**-sf**  separate the code corresponding with function symbols by some new lines to let them stand out in the printed disassembly.


