# GPUsuba
High-throughput cryptography on GPUs


This project aim to describe high-level cryptography in 
[Usuba](https://dadaiscrazy.github.io/usuba/ "Usuba project's Homepage") 
language for high-throughput cryptography on GPUs.

It splits into several parts and the first one is a search among literature 
for AES' codes and benchmarks implemented in CUDA to have a strong reference to compare to.


First_CUDA :
	It comes from the first known AES implementation on GPU through CUDA. Original is due to
	S. A. Manavski and corresponding paper : CUDA compatible GPU as an efﬁcient hardware
	accelerator for AES cryptography. In ICSPC 2007: Proc. of IEEE Int’l
	Conf. on Signal Processing and Communication, pages 65–68, 2007.


[engine_cuda](https://github.com/heipei/engine-cuda "Updated engine-cuda") :
	The updated engine-cuda project is based on the 
	[same named project](https://code.google.com/archive/p/engine-cuda/ "Original engine-cuda" ) 
	created in 2011. The updated is the implementation work sum up by "GPU-Acceleration of
	Block Ciphers in the OpenSSL Cryptographic Library" by Johannes Gilger and al.
