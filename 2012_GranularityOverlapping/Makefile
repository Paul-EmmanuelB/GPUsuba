
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
CUFLAGS	:= -Xptxas -O3

# Macros
CC = gcc
NVCC = /usr/local/cuda/bin/nvcc

# Path
SRC_PATH   = .
OBJ_PATH   = obj
EXE_PATH   = .
DATA_PATH  = data


# Final product
PRODUCT = aes.exe

# -- src and obj List ----------
SRC  = $(addprefix ${SRC_PATH}/, $(SRC_FILES))

OBJ  = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(SRC_FILES))))

###############################################################################
# Rules and targets

all : $(EXE_PATH)/$(PRODUCT)


#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT):
	$(NVCC) $(CUFLAGS)  $^ -o $@

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC)
	$(NVCC) $(CUFLAGS) -x cu  -dc $< -o $@
	
clean:
	rm -f $(OBJ)
	rm -f ${EXE_PATH}/${PRODUCT}