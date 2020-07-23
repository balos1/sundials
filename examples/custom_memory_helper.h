/* -----------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
 * -----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------
 * Example of a custom SUNMemoryHelper that only supports CUDA
 * unmanaged memory only and synchronous copies.
 * -----------------------------------------------------------------*/

#include <cuda_runtime.h>

#include <sundials/sundials_memory.h>


SUNMemory MyMemoryHelper_Alloc(SUNMemoryHelper helper,
                               size_t memsize,
                               SUNMemoryType mem_type)
{
  SUNMemory mem = SUNMemoryNewEmpty();

  mem->ptr = NULL;
  mem->own = SUNTRUE;

  if (mem_type == SUNMEMTYPE_HOST ||
      mem_type == SUNMEMTYPE_PINNED)
  {
    mem->ptr  = malloc(memsize);
    mem->type = SUNMEMTYPE_HOST;
  }
  else if (mem_type == SUNMEMTYPE_UVM ||
           mem_type == SUNMEMTYPE_DEVICE)
  {
    cudaMalloc(&(mem->ptr), memsize);
    mem->type = SUNMEMTYPE_DEVICE;
  }
  else
  {
    free(mem);
    return(NULL);
  }

  return(mem);
}

void MyMemoryHelper_Dealloc(SUNMemoryHelper helper, SUNMemory mem)
{
  if (mem == NULL) return;

  if (mem->ptr != NULL && mem->own)
  {
    if (mem->type == SUNMEMTYPE_HOST)
    {
      free(mem->ptr);
      mem->ptr = NULL;
    }
    else if (mem->type == SUNMEMTYPE_DEVICE)
    {
      cudaFree(mem->ptr);
      mem->ptr = NULL;
    }
  }
}

int MyMemoryHelper_Copy(SUNMemoryHelper helper, SUNMemory dst,
                        SUNMemory src, size_t memory_size)
{
  int retval = 0;
  cudaError_t cuerr = cudaSuccess;

  switch(src->type)
  {
    case SUNMEMTYPE_HOST:
      if (dst->type == SUNMEMTYPE_HOST)
      {
        memcpy(dst->ptr, src->ptr, memory_size);
      }
      else if (dst->type == SUNMEMTYPE_DEVICE)
      {
        cuerr = cudaMemcpy(dst->ptr, src->ptr,
                           memory_size,
                           cudaMemcpyHostToDevice);
      }
      if (cuerr != cudaSuccess) retval = -1;
      break;
    case SUNMEMTYPE_DEVICE:
      if (dst->type == SUNMEMTYPE_HOST)
      {
        cuerr = cudaMemcpy(dst->ptr, src->ptr,
                           memory_size,
                           cudaMemcpyDeviceToHost);
      }
      else if (dst->type == SUNMEMTYPE_DEVICE)
      {
        cuerr = cudaMemcpy(dst->ptr, src->ptr,
                           memory_size,
                           cudaMemcpyDeviceToDevice);
      }
      if (cuerr != cudaSuccess) retval = -1;
      break;
    default:
      retval = -1;
  }

  return(retval);
}

SUNMemoryHelper MyMemoryHelper()
{
  SUNMemoryHelper helper;
  SUNMemoryHelper_Ops ops;

  /* Create the ops */
  ops = (SUNMemoryHelper_Ops) malloc(sizeof(struct _SUNMemoryHelper_Ops));
  memset(ops, 0, sizeof(struct _SUNMemoryHelper_Ops));

  /* Set the ops */
  ops->alloc     = MyMemoryHelper_Alloc;
  ops->dealloc   = MyMemoryHelper_Dealloc;
  ops->copy      = MyMemoryHelper_Copy;
  ops->copyasync = NULL;

  /* Allocate helper */
  helper = (SUNMemoryHelper) malloc(sizeof(struct _SUNMemoryHelper));
  memset(helper, 0, sizeof(struct _SUNMemoryHelper));

  /* Attach user data and ops */
  helper->content = NULL;
  helper->ops     = ops;

  return helper;
}
