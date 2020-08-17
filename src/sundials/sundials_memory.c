
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
 * SUNDIALS memory helper.
 * ----------------------------------------------------------------*/

#include <string.h>

#include "sundials_debug.h"
#include <sundials/sundials_memory.h>


SUNMemory SUNMemoryNewEmpty()
{
  SUNMemory mem = NULL;

  mem = (SUNMemory) malloc(sizeof(struct _SUNMemory));
  if (mem == NULL)
  {
    SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryNewEmpty: malloc failed\n");
    return(NULL);
  }

  return(mem);
}


SUNMemory SUNMemoryHelper_Alias(SUNMemory mem)
{
  SUNMemory alias = SUNMemoryNewEmpty();

  alias->ptr  = mem->ptr;
  alias->type = mem->type;
  alias->own  = SUNFALSE;

  return(alias);
}


SUNMemory SUNMemoryHelper_Wrap(void* ptr, SUNMemoryType mem_type)
{
  SUNMemory mem = SUNMemoryNewEmpty();

  mem->ptr = ptr;
  mem->own = SUNFALSE;

  switch(mem_type)
  {
    case SUNMEMTYPE_HOST:
      mem->type = SUNMEMTYPE_HOST;
      break;
    case SUNMEMTYPE_PINNED:
      mem->type = SUNMEMTYPE_PINNED;
      break;
    case SUNMEMTYPE_DEVICE:
      mem->type = SUNMEMTYPE_DEVICE;
      break;
    case SUNMEMTYPE_UVM:
      mem->type = SUNMEMTYPE_UVM;
      break;
    default:
      free(mem);
      SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Wrap: unknown memory type\n");
      return(NULL);
  }

  return(mem);
}


SUNMemory SUNMemoryHelper_Alloc(SUNMemoryHelper helper, size_t memsize,
                                SUNMemoryType mem_type)
{
  if (helper->ops->alloc == NULL)
  {
    SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Alloc: function pointer is NULL\n");
    return NULL;
  }
  return helper->ops->alloc(helper, memsize, mem_type);
}


void SUNMemoryHelper_Dealloc(SUNMemoryHelper helper, SUNMemory mem)
{
  if (helper->ops->dealloc == NULL)
  {
    SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Dealloc: function pointer is NULL\n");
  }
  helper->ops->dealloc(helper, mem);
  if (mem != NULL) free(mem);
}


int SUNMemoryHelper_Copy(SUNMemoryHelper helper, SUNMemory dst,
                         SUNMemory src, size_t memory_size)
{
  if (helper->ops->copy == NULL)
  {
    SUNDIALS_DEBUG_PRINT("ERROR in SUNMemoryHelper_Copy: function pointer is NULL\n");
    return(-1);
  }
  return(helper->ops->copy(helper, dst, src, memory_size));
}


int SUNMemoryHelper_CopyAsync(SUNMemoryHelper helper, SUNMemory dst,
                              SUNMemory src, size_t memory_size,
                              void* ctx)
{
  if (helper->ops->copyasync == NULL)
  {
    return(SUNMemoryHelper_Copy(helper, dst, src, memory_size));
  }
  return(helper->ops->copyasync(helper, dst, src, memory_size, ctx));
}

void SUNMemoryHelper_Destroy(SUNMemoryHelper helper)
{
  if (helper->ops->destroy == NULL)
  {
    free(helper->ops);
    free(helper);
  }
  else
  {
    helper->ops->destroy(helper);
  }
}


SUNMemoryHelper SUNMemoryHelper_Clone(SUNMemoryHelper helper)
{
  if (helper->ops->clone == NULL)
  {
    SUNMemoryHelper hclone = (SUNMemoryHelper) malloc(sizeof(struct _SUNMemoryHelper));
    memcpy(hclone, helper, sizeof(struct _SUNMemoryHelper));
    hclone->ops = (SUNMemoryHelper_Ops) malloc(sizeof(struct _SUNMemoryHelper_Ops));
    memcpy(hclone->ops, helper->ops, sizeof(struct _SUNMemoryHelper_Ops));
    hclone->content = NULL;
    return(hclone);
  }
  else
  {
    return(helper->ops->clone(helper));
  }
}