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

#include <string>
#include "sundials_blprofile.h"

namespace amrex
{
  class BLProfiler
  {
  public:
    explicit BLProfiler(const std::string &funcname);
     ~BLProfiler();
  };
}

SUNProfiler newSUNProfiler(char* fname)
{
  return reinterpret_cast<void*>(new amrex::BLProfiler(fname));
}

void deleteSUNProfiler(SUNProfiler bl)
{
  delete reinterpret_cast<amrex::BLProfiler*>(bl);
}
