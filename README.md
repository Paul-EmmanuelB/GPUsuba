GPUsuba
=======
High-throughput cryptography on GPUs


This project aim to describe high-level cryptography in 
[Usuba](https://dadaiscrazy.github.io/usuba/ "Usuba project's Homepage") 
language for high-throughput cryptography on GPUs.

It splits into several parts:
-   Searching among literature from 2007 - the beginning of CUDA - to this date for implementations and benchmarks of AES in CUDA. 
-	Using the literature as a reference and extract innovative techniques that improve the throughput. 
-	Writing a reference implementation and adding one by one the found methods to benchmark the real impact of each one of them. 



Available folders 
=================

Device_Informations
-------------------
Allow to have certain informations about the GPUs available to the host device who run the application. Those informations are given by the CUDA Runtime API. Check the [Device Management](https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__DEVICE.html "CUDA Toolkit Documentation") section from the CUDA toolkit documentation for further details.

AES_Implementations
-------------------
Regroup all the AES implementations done. 
#### First_CUDA  
It comes from the first known AES implementation on GPU through CUDA. It is the first updated implementation of this project for understanding AES basic methods and having a reference to begin with. 
Original is due to S. A. Manavski and corresponding paper [1].

#### Implem_Methods
Contain a reference implementation inspired by both Iwai and al. [2] along with Li and al. [3]. This reference provides granularity improvment (16 o/thread) and specific memory allocation for each data needed. It's a starting point to compare with other improvments methods.




---
---
## References
[1] CUDA compatible GPU as an efﬁcient hardware accelerator for AES cryptography. In ICSPC 2007: Proc. of IEEE Int’l Conf. on Signal Processing and Communication, pages 65–68, 2007.

[2] IWAI, Keisuke, NISHIKAWA, Naoki, et KUROKAWA, Takakazu. Acceleration of AES encryption on CUDA GPU. International Journal of Networking and Computing, 2012, vol. 2, no 1, p. 131-145.

[3] LI, Qinjian, ZHONG, Chengwen, ZHAO, Kaiyong, et al. Implementation and analysis of AES encryption on GPU. In : 2012 IEEE 14th International Conference on High Performance Computing and Communication & 2012 IEEE 9th International Conference on Embedded Software and Systems. IEEE, 2012. p. 843-848.