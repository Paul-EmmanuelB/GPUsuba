
#ifndef _AESCUDA_H_
#define _AESCUDA_H_
 

#include <iostream>
#include "aesCudaUtils.h"
#include <string.h>


//handler
extern "C"
int aesHost(unsigned char* result, const unsigned char* inData, int inputSize, const unsigned char* key, int keySize, bool toEncrypt);


int main_function(int argc, char *argv[]);



#endif // #ifndef _AESCUDA_H_
