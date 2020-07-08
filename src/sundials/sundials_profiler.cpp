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
 * -----------------------------------------------------------------*/

#include <iostream>
#include "sundials_profiler.h"
#include "nvToolsExt.h"

class NvtxProfiler
{
public:
  explicit NvtxProfiler(const char* funcname)
  {
    /* r = nvtxRangeStartA(funcname); */
    nvtxRangePushA(funcname);
  }

  ~NvtxProfiler()
  {
    /* nvtxRangeEnd(r); */
    nvtxRangePop();
  }

private:
  nvtxRangeId_t r;
};

SUNProfiler newSUNProfiler(const char* fname)
{
  return reinterpret_cast<void*>(new NvtxProfiler(fname));
}

void deleteSUNProfiler(SUNProfiler bl)
{
  delete reinterpret_cast<NvtxProfiler*>(bl);
}
