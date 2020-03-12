
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
CUFILES		:= aesHost.cu 
# C/C++ source files (compiled with gcc / c++)
CCFILES		:= aescuda.cpp  aesCudaUtils.cpp benchmark.cpp

#C and CU FLAGS
CFLAGS 	:= -Xptxas -O3 
CUFLAGS	:= -O3

# Macros
CC = gcc
NVCC = /usr/local/cuda/bin/nvcc

# Path
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include
DATA_PATH = data
RES_PATH  = res


# Includes
INC  		= -I ./include
OLD_INC 	= -I /usr/local/cuda/samples/common/inc #To replace a deleted library
BOOST_ROOT 	= -I ~/boost_1_72_0

# Library
BOOST_LIB =  ~/boost_build/lib

#LINKFLAGS
LINKFLAGS	:= -L $(BOOST_LIB)  -l:libboost_system.a -l:libboost_filesystem.a



# Final product
PRODUCT = aes.exe

# -- src and obj List ----------
SRC  = $(addprefix ${SRC_PATH}/, $(CUFILES))
SRC += $(addprefix ${SRC_PATH}/, $(CCFILES))

OBJ  = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(CCFILES))))
OBJ += $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(CUFILES))))

###############################################################################
# Rules and targets

all : $(EXE_PATH)/$(PRODUCT)


#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ)
	$(NVCC) $(CFLAGS)  $^ -o $@ $(LINKFLAGS)

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(NVCC) $(CFLAGS) -x cu $(INC) $(OLD_INC) $(BOOST_ROOT) -dc $< -o $@

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cu
	$(NVCC) $(CUFLAGS) -x cu $(INC) $(OLD_INC) -dc $< -o $@
	
clean:
	rm -f $(OBJ)
	rm -f ${EXE_PATH}/${PRODUCT}

clean_res :
	rm ${RES_PATH}/*.dat
