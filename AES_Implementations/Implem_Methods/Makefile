
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

# Source files
SRC_FILES := main.cu


#C and CU FLAGS
CUFLAGS	:= -Xptxas -v -O3 -arch=sm_62

# Macros
CC = gcc
NVCC = /usr/local/cuda/bin/nvcc

# Path
SRC_PATH_REF   	= Reference
SRC_PATH_BS    	= BlockSwitching
SRC_PATH_WBA    = WarpBasedAccess
SRC_PATH_OV    	= Overlapping
SRC_PATH_AoS    = AoS
SRC_PATH_TM    	= TextureMemory
SRC_PATH_Shufl  = Shuffle
SRC_PATH_Bts	= Bitslicing
EXE_PATH   		= .
DATA_PATH  		= Data
INCLUDE_PATH	= Include

# Final product
NAME_PRODUCT = aes.exe
PRODUCT = $(addprefix ${EXE_PATH}/, $(NAME_PRODUCT))

# -- Src lists ----------
SRC_REF  	= $(addprefix ${SRC_PATH_REF}/, $(SRC_FILES))
SRC_BS		= $(addprefix ${SRC_PATH_BS}/, $(SRC_FILES))
SRC_WBA		= $(addprefix ${SRC_PATH_WBA}/, $(SRC_FILES))
SRC_OV		= $(addprefix ${SRC_PATH_OV}/, $(SRC_FILES))
SRC_AoS		= $(addprefix ${SRC_PATH_AoS}/, $(SRC_FILES))
SRC_TM		= $(addprefix ${SRC_PATH_TM}/, $(SRC_FILES))
SRC_Shufl	= $(addprefix ${SRC_PATH_Shufl}/, $(SRC_FILES))
SRC_Bts	    = $(addprefix ${SRC_PATH_Bts}/, $(SRC_FILES))

###############################################################################
# Rules and targets

all: Ref


#-----Main rule ----------

Ref:  $(SRC_REF)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

BS:  $(SRC_BS)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

WBA:  $(SRC_WBA)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

OV:  $(SRC_OV)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

AoS:  $(SRC_AoS)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

TM:  $(SRC_TM)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

Shufl:  $(SRC_Shufl)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

Bts:  $(SRC_Bts)
	$(NVCC) $(CUFLAGS)  $^ -o $(PRODUCT) -I $(INCLUDE_PATH)

dataset:
	head -c 8MB /dev/urandom > $(DATA_PATH)/dataset.txt

clean:
	rm -f ${PRODUCT}

cleanRes:
	rm -f Result/result.*

cleanDataset:
	rm -f $(DATA_PATH)/dataset.txt
