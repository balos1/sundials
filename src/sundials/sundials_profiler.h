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

#ifndef _SUNDIALS_PROFILER_H
#define _SUNDIALS_PROFILER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* SUNProfiler;
SUNProfiler newSUNProfiler(const char* fname);
void deleteSUNProfiler(SUNProfiler bl);

#ifdef USE_PROFILER
#define SUN_PROFILER_BEGIN(fname) SUNProfiler __scope_profiler = newSUNProfiler((fname))
#define SUN_PROFILER_END(fname) deleteSUNProfiler(__scope_profiler)
#else
#define SUN_PROFILER_BEGIN(fname)
#define SUN_PROFILER_END(fname)
#endif

#ifdef __cplusplus
}
#endif
#endif // _SUNDIALS_PROFILER_H