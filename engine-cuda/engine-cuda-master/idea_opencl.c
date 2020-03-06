// vim:ft=opencl:
/**
 * @version 0.1.3 (2011)
 * @author Johannes Gilger <heipei@hackvalue.de>
 * 
 * Copyright 2011 Johannes Gilger
 *
 * This file is part of engine-cuda
 *
 * engine-cuda is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License or
 * any later version.
 * 
 * engine-cuda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with engine-cuda. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <openssl/idea.h>
#include <CL/opencl.h>
#include "common.h"
#include "opencl_common.h"

static cl_mem idea_iv = NULL;

void IDEA_opencl_transfer_key_schedule(IDEA_KEY_SCHEDULE *ks, cl_mem *device_schedule, cl_command_queue queue) {
	assert(ks);
	clEnqueueWriteBuffer(queue,*device_schedule,CL_TRUE,0,sizeof(IDEA_KEY_SCHEDULE),ks,0,NULL,NULL);
}

void IDEA_opencl_transfer_iv(cl_context context, const unsigned char *iv,cl_command_queue queue) {
	cl_int error;
	if(!idea_iv)
		CL_ASSIGN(idea_iv = clCreateBuffer(context,CL_MEM_READ_ONLY,IDEA_BLOCK_SIZE,NULL,&error));
	CL_WRAPPER(clEnqueueWriteBuffer(queue,idea_iv,CL_TRUE,0,IDEA_BLOCK_SIZE,iv,0,NULL,NULL));
}

void IDEA_opencl_crypt(opencl_crypt_parameters *c) {

	size_t blockSize[3] = {MAX_THREAD, 0, 0};
	size_t gridSize[3] = {c->nbytes/IDEA_BLOCK_SIZE, 0, 0};

	if (!(c->nbytes%(IDEA_BLOCK_SIZE*MAX_THREAD))==0) {
		gridSize[0] += (MAX_THREAD - (gridSize[0]%MAX_THREAD));
	}

	clSetKernelArg(*c->d_kernel, 0, sizeof(cl_mem), c->d_in);
	clSetKernelArg(*c->d_kernel, 1, sizeof(cl_mem), c->d_schedule);

	cl_uint args;
	clGetKernelInfo(*c->d_kernel,CL_KERNEL_NUM_ARGS,4,&args,NULL);

	if(args > 2 && idea_iv) {
		clSetKernelArg(*c->d_kernel, 2, sizeof(cl_mem), &idea_iv);
		clSetKernelArg(*c->d_kernel, 3, sizeof(cl_mem), c->d_out);
	}

	clEnqueueWriteBuffer(*c->queue,*c->d_in,CL_FALSE,0,c->nbytes,c->in,0,NULL,NULL);

	OPENCL_TIME_KERNEL("IDEA    ",1)

	if(args > 2) {
		clEnqueueReadBuffer(*c->queue,*c->d_out,CL_FALSE,0,c->nbytes,c->out,0,NULL,NULL);
		IDEA_opencl_transfer_iv(*c->context,c->in+c->nbytes-IDEA_BLOCK_SIZE,*c->queue);
	} else {
		clEnqueueReadBuffer(*c->queue,*c->d_in,CL_TRUE,0,c->nbytes,c->out,0,NULL,NULL);
	}
}
