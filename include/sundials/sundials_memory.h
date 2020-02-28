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
 * SUNDIALS memory helpers and types.
 * ----------------------------------------------------------------*/

#ifndef _SUNDIALS_MEMORY_H
#define _SUNDIALS_MEMORY_H

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

typedef enum
{
  SUNMEMTYPE_HOST,   // memory accessible from the host
  SUNMEMTYPE_DEVICE, // memory accessible only on the device
  SUNMEMTYPE_UVM     // memory accessible from the host or device
} SUNMemoryType;


#ifdef __cplusplus
}
#endif

#endif