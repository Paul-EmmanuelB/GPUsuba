This folder displays several files from *aes* executable. Differences between them are based on the tool used and also on some options. 
More explanations and illustrations about those tools or options are available on [Nvidia's website](https://docs.nvidia.com/cuda/cuda-binary-utilities/index.html "Cuda binary utilities").

#### cuobjdump

Files beginning with *cuobjdump* use this precise tool. This tool can deal with both host and device code. But it will give less informations than its alternative called *nvdisasm*.

**-elf**    option - also included in concerned filename - gives from the executable all cuda elf sections in a human readable format.  
**-ptx**    gives ptx info. The *cuobjdump-elf.txt* is only here to highlight the ptx part. It's obviously more useful with other command line options.  
**-sass**   focuses only on cubin (cuda bin) files and give the assembly code of included kernels.  

#### nvdisasm   

This extraction tool works only on cubin files. If we have an executable with host code, we can use *cuobjdump* to extract cubin files :
```shell
 >> cuobjdump executable_filename -xelf all
```

We can also use *-lelf* option to get the list of cubin files embedded in the executable file. Then we can extract a precise cubin file with `-xelf cubin_filename` or even with words contained in cubin_filename. 

In our case, the file extracted is *aes.1.sm_30.cubin*. 

When extraction is done, we can apply the nvdisasm tool. It only deals with one input and with options as :  

**-cfg** to obtain the control flow graph on a kernel. 
**-plr** to print several details during the use of registers along the corresponding disassembly code.
**-g**   to annotate disassembly code. 

Png file is the corresponding representation of the control flow graph. 