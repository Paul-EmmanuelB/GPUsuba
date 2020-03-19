
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
CUFILES		:= deviceInformations.cu 

# Macros
NVCC = /usr/local/cuda/bin/nvcc

# Final product
PRODUCT = devInfo.exe


###############################################################################
# Rules and targets

all : $(PRODUCT)


#-----Main rule ----------
$(PRODUCT): $(CUFILES)
	$(NVCC) $^ -o $@

clean:
	rm -f ${PRODUCT}
