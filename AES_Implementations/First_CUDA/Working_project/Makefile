
#/***************************************************************************
# *   Copyright (C) 2020                                                    *
# *                                                                         *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# ***************************************************************************/


#	@author Broux Paul-Emmanuel <paulemmanuelb@gmail.com>



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
CCFILES		:= aescuda.cpp  aesCudaUtils.cpp main.cpp

#C and CU FLAGS
CFLAGS 	:= -O3 
CUFLAGS	:= -Xptxas -O3
PROFILING := -pg

# Macros
CC = gcc
NVCC = /usr/local/cuda/bin/nvcc

# Path
SRC_PATH   = src
OBJ_PATH   = obj
EXE_PATH   = exe
INC_PATH   = include
DATA_PATH  = data
RES_PATH   = res
BENCH_PATH = bench


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
	$(NVCC) $(CUFLAGS)  $^ -o $@ $(LINKFLAGS)

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(NVCC) $(CFLAGS) $(INC) $(OLD_INC) $(BOOST_ROOT) -dc $< -o $@

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cu
	$(NVCC) $(CUFLAGS) $(INC) $(OLD_INC) -dc $< -o $@

# -- Profiling ----------
profil : $(OBJ)
	$(NVCC) $(CUFLAGS) $(PROFILING)  $^ -o $@ $(LINKFLAGS)

# -- Cleaning ----------
clean:
	rm -f $(OBJ)
	rm -f ${EXE_PATH}/${PRODUCT}

cleanRes :
	rm ${RES_PATH}/*.dat

cleanBench :
	rm ${BENCH_PATH}/benchResult
