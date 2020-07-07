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

#ifndef _SUNDIALS_BLPROFILE_H
#define _SUNDIALS_BLPROFILE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* SUNProfiler;
SUNProfiler newSUNProfiler(char* fname);
void deleteSUNProfiler(SUNProfiler bl);

#ifdef USE_BL_PROFILE
#define SUN_BL_PROFILE_BEGIN(fname) SUNProfiler __scope_profiler = newSUNProfiler((fname))
#define SUN_BL_PROFILE_END(fname) deleteSUNProfiler(__scope_profiler)
#else
#define SUN_BL_PROFILE_BEGIN(fname)
#define SUN_BL_PROFILE_END(fname)
#endif

#ifdef __cplusplus
}
#endif
#endif // _SUNDIALS_BLPROFILE_H