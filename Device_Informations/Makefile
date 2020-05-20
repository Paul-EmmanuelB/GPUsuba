
################################################################################
#
# Build script for project
#
###############################################################################

###############################################################################
##	@author Broux Paul-Emmanuel <paulemmanuelb@gmail.com>					 ##
###############################################################################

# Add source files here
EXECUTABLE	:= AES	
# Cuda source files (compiled with cudacc)
DEV_INFO_FILES		:= deviceInformations.cu 
MEM_FILES			:= memoryBottleneck.cu

#C and CU FLAGS
CUFLAGS	:= -Xptxas -O3 -arch=sm_62

# Macros
NVCC = /usr/local/cuda/bin/nvcc

#PATH
INCLUDE_PATH	= ../AES_Implementations/Implem_Methods/Include

# Final product
DEV_PRODUCT = devInfo.exe
MEM_PRODUCT = memBottl.exe

###############################################################################
# Rules and targets

all : $(DEV_PRODUCT)


#-----Main rule ----------
deviceInformations: $(DEV_INFO_FILES)
	$(NVCC)  $(CUFLAGS) $^ -o $(DEV_PRODUCT)

memoryBottleneck: $(MEM_FILES)
	$(NVCC)  $(CUFLAGS) $^ -o $(MEM_PRODUCT) -I $(INCLUDE_PATH)

clean:
	rm -f ${PRODUCT}
