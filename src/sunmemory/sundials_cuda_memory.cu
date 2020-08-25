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
 * SUNDIALS CUDA memory helper implementation.
 * ----------------------------------------------------------------*/

#include <cstdlib>

#include <sunmemory/sunmemory_cuda.h>
#include "sundials_debug.h"
#include "sundials_cuda.h"


SUNMemoryHelper SUNMemoryHelper_Cuda()
{
  SUNMemoryHelper helper;
  SUNMemoryHelper_Ops ops;

  /* Create the ops */
  ops = (SUNMemoryHelper_Ops) malloc(sizeof(struct _SUNMemoryHelper_Ops));
  memset(ops, 0, sizeof(struct _SUNMemoryHelper_Ops));

  /* Set the ops */
  ops->alloc     = SUNMemoryHelper_Alloc_Cuda;
  ops->dealloc   = SUNMemoryHelper_Dealloc_Cuda;
  ops->copy      = SUNMemoryHelper_Copy_Cuda;
  ops->copyasync = SUNMemoryHelper_CopyAsync_Cuda;

  /* Allocate helper */
  helper = (SUNMemoryHelper) malloc(sizeof(struct _SUNMemoryHelper));
  memset(helper, 0, sizeof(struct _SUNMemoryHelper));

  /* Attach content and ops */
  helper->content = NULL;
  helper->ops     = ops;

  return helper;
}

SUNMemory SUNMemoryHelper_Alloc_Cuda(SUNMemoryHelper helper,
                                     size_t memsize,
                                     SUNMemoryType mem_type)
{
  SUNMemory mem = SUNMemoryNewEmpty();

  mem->ptr  = NULL;
  mem->own  = SUNTRUE;
  mem->type = mem_type;

  if (mem_type == SUNMEMTYPE_HOST)
  {
    mem->ptr = malloc(memsize);
    if (mem->ptr == NULL)
    {
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Alloc_Cuda: malloc returned NULL\n");
      free(mem);
      return(NULL);
    }
  }
  else if (mem_type == SUNMEMTYPE_PINNED)
  {
    if (!SUNDIALS_CUDA_VERIFY(cudaMallocHost(&(mem->ptr), memsize)))
    {
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Alloc_Cuda: cudaMallocHost failed\n");
      free(mem);
      return(NULL);
    }
  }
  else if (mem_type == SUNMEMTYPE_DEVICE)
  {
    if (!SUNDIALS_CUDA_VERIFY(cudaMalloc(&(mem->ptr), memsize)))
    {
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Alloc_Cuda: cudaMalloc failed\n");
      free(mem);
      return(NULL);
    }
  }
  else if (mem_type == SUNMEMTYPE_UVM)
  {
    if (!SUNDIALS_CUDA_VERIFY(cudaMallocManaged(&(mem->ptr), memsize)))
    {
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Alloc_Cuda: cudaMallocManaged failed\n");
      free(mem);
      return(NULL);
    }
  }
  else
  {
    SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Alloc_Cuda: unknown memory type\n");
    free(mem);
    return(NULL);
  }

  return(mem);
}

void SUNMemoryHelper_Dealloc_Cuda(SUNMemoryHelper helper, SUNMemory mem)
{
  if (mem == NULL) return;

  if (mem->ptr != NULL && mem->own)
  {
    if (mem->type == SUNMEMTYPE_HOST)
    {
      free(mem->ptr);
      mem->ptr = NULL;
    }
    else if (mem->type == SUNMEMTYPE_PINNED)
    {
      if (!SUNDIALS_CUDA_VERIFY(cudaFreeHost(mem->ptr)))
      {
        SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Dealloc_Cuda: cudaFreeHost failed\n");
      }
      mem->ptr = NULL;
    }
    else if (mem->type == SUNMEMTYPE_DEVICE ||
             mem->type == SUNMEMTYPE_UVM)
    {
      if (!SUNDIALS_CUDA_VERIFY(cudaFree(mem->ptr)))
      {
        SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Dealloc_Cuda: cudaFree failed\n");
      }
      mem->ptr = NULL;
    }
    else
    {
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Dealloc_Cuda: unknown memory type\n");
    }
  }
}

int SUNMemoryHelper_Copy_Cuda(SUNMemoryHelper helper, SUNMemory dst,
                              SUNMemory src, size_t memory_size)
{
  int retval = 0;
  cudaError_t cuerr = cudaSuccess;

  switch(src->type)
  {
    case SUNMEMTYPE_HOST:
    case SUNMEMTYPE_PINNED:
      if (dst->type == SUNMEMTYPE_HOST ||
          dst->type == SUNMEMTYPE_PINNED)
      {
        memcpy(dst->ptr, src->ptr, memory_size);
      }
      else if (dst->type == SUNMEMTYPE_DEVICE ||
               dst->type == SUNMEMTYPE_UVM)
      {
        cuerr = cudaMemcpy(dst->ptr, src->ptr,
                           memory_size,
                           cudaMemcpyHostToDevice);
      }
      if (!SUNDIALS_CUDA_VERIFY(cuerr)) retval = -1;
      break;
    case SUNMEMTYPE_UVM:
    case SUNMEMTYPE_DEVICE:
      if (dst->type == SUNMEMTYPE_HOST ||
          dst->type == SUNMEMTYPE_PINNED)
      {
        cuerr = cudaMemcpy(dst->ptr, src->ptr,
                           memory_size,
                           cudaMemcpyDeviceToHost);
      }
      else if (dst->type == SUNMEMTYPE_DEVICE ||
              dst->type == SUNMEMTYPE_UVM)
      {
        cuerr = cudaMemcpy(dst->ptr, src->ptr,
                           memory_size,
                           cudaMemcpyDeviceToDevice);
      }
      if (!SUNDIALS_CUDA_VERIFY(cuerr)) retval = -1;
      break;
    default:
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_CopyAsync_Cuda: unknown memory type\n");
      retval = -1;
  }

  return(retval);
}

int SUNMemoryHelper_CopyAsync_Cuda(SUNMemoryHelper helper, SUNMemory dst,
                                   SUNMemory src, size_t memory_size,
                                   void* ctx)
{
  int retval = 0;
  cudaError_t cuerr = cudaSuccess;
  cudaStream_t stream = 0;

  if (ctx != NULL)
  {
    stream = *((cudaStream_t*) ctx);
  }

  switch(src->type)
  {
    case SUNMEMTYPE_HOST:
    case SUNMEMTYPE_PINNED:
      if (dst->type == SUNMEMTYPE_HOST ||
          dst->type == SUNMEMTYPE_PINNED)
      {
        memcpy(dst->ptr, src->ptr, memory_size);
      }
      else if (dst->type == SUNMEMTYPE_DEVICE ||
               dst->type == SUNMEMTYPE_UVM)
      {
        cuerr = cudaMemcpyAsync(dst->ptr, src->ptr,
                                memory_size,
                                cudaMemcpyHostToDevice,
                                stream);
      }
      if (!SUNDIALS_CUDA_VERIFY(cuerr)) retval = -1;
      break;
    case SUNMEMTYPE_UVM:
    case SUNMEMTYPE_DEVICE:
      if (dst->type == SUNMEMTYPE_HOST ||
          dst->type == SUNMEMTYPE_PINNED)
      {
        cuerr = cudaMemcpyAsync(dst->ptr, src->ptr,
                                memory_size,
                                cudaMemcpyDeviceToHost,
                                stream);
      }
      else if (dst->type == SUNMEMTYPE_DEVICE ||
              dst->type == SUNMEMTYPE_UVM)
      {
        cuerr = cudaMemcpyAsync(dst->ptr, src->ptr,
                                memory_size,
                                cudaMemcpyDeviceToDevice,
                                stream);
      }
      if (!SUNDIALS_CUDA_VERIFY(cuerr)) retval = -1;
      break;
    default:
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_CopyAsync_Cuda: unknown memory type\n");
      retval = -1;
  }

  return(retval);
}
