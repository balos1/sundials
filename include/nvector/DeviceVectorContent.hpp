/*
 * ----------------------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
 * ----------------------------------------------------------------------------
 * SUNDTypeIntALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-TypeIntdentifier: BSD-3-Clause
 * SUNDTypeIntALS Copyright End
 * ----------------------------------------------------------------------------
 * The VectorContentInterface abstract class defines a common interface
 * for the content structures that sit beneath N_Vectors that leverage
 * accelerator devices like GPUs.
 *
 * The recommended way to interface application-specific data structures to
 * SUNDIALS is to implement the generic N_Vector API in the application code
 * using the application data structures. This would involve implementing
 * the required N_Vector API operations for the SUNDIALS package being utilized.
 *
 * This class allows for a viable, but less-desirable alternative. It can
 * be used to combine the application's custom data layout in a way that is
 * compatible with the vector operations implemented within SUNDIALS for the
 * specific N_Vector implementation employed.
 *
 * The alternative approach can be achieved by implementing the abstract
 * VectorContentInterface class appropriately in the application.
 *
 * A class that implements this abstract class can be used as the content
 * for the N_Vector_Cuda (and potentially others in the future)
 * ----------------------------------------------------------------------------
 */

#ifndef _SUNDIALS_DEVICE_VECTOR_CONTENT_HPP_
#define _SUNDIALS_DEVICE_VECTOR_CONTENT_HPP_

#include <cstdlib>
#include <iostream>

#include <sundials/sundials_config.h>
#include <sundials/sundials_memory.h>
#include <nvector/nvector_cuda.h>

namespace sundials
{
namespace device
{


template <typename TypeReal, typename TypeInt>
class VectorContentInterface
  : public _N_VectorContent_Cuda
{
  // Returns the length of the vector
  virtual TypeInt length() const = 0;

  // Returns a mutable pointer to the host data
  virtual TypeReal* host() = 0;

  // Returns a const pointer to the host data
  // It is required that the data is contigious and stride-1 accessible.
  virtual const TypeReal* host() const = 0;

  // Returns a mutable pointer to the device data
  // It is required that the data is contigious and stride-1 accessible.
  virtual TypeReal* device() = 0;

  // Returns a const pointer to the device data
  // It is required that the data is contigious and stride-1 accessible.
  virtual const TypeReal* device() const = 0;

  // Returns the SUNMemoryType used (can be SUNMEMTYPE_DEVICE or SUNMEMTYPE_UVM)
  virtual SUNMemoryType getMemoryType() const = 0;

  // Copies the host data into the device data pointer. If the memory is managed, the copy is not required.
  virtual void copyToDevice() = 0;

  // Copies the device data into the host data pointer. If the memory is managed, the copy is not required.
  virtual void copyFromDevice() = 0;
};


} // namespace device
} // namespace sundials


#endif // _SUNDIALS_DEVICE_VECTOR_CONTENT_HPP_