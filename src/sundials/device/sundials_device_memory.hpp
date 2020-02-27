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
 * An extremely simple memory allocator to be used for data that
 * may reside in different memory spaces.
 * -----------------------------------------------------------------
 */

#ifndef _SUN_MEMORYCLASS_HPP_
#define _SUN_MEMORYCLASS_HPP_

#include <cstdlib>
#include <iostream>
#include <cuda_runtime.h>

#define SUN_GPU_PREFIX(STR) cuda ## STR

namespace sundials
{
namespace device
{

void* AllocHost(size_t mem_size);
void* AllocDevice(size_t mem_size);
void* AllocUVM(size_t mem_size);
void FreeHost(void* ptr);
void FreeDevice(void* ptr);
void FreeUVM(void* ptr);

// SUNDIALS supported memory types.
typedef enum
{
  SUNMEMTYPE_HOST,   // memory accessible from the host
  SUNMEMTYPE_DEVICE, // memory accessible only on the device
  SUNMEMTYPE_UVM     // memory accessible from the host or device
} SUNMemoryType;

// Memory management class.
class MemoryClass
{
public:

  // Using our built in allocation routines.
  MemoryClass(SUNMemoryType memory_type)
   : memory_type_(memory_type),
     AllocHost_(AllocHost),
     AllocDevice_(AllocDevice),
     AllocUVM_(AllocUVM),
     FreeHost_(FreeHost),
     FreeDevice_(FreeDevice),
     FreeUVM_(FreeUVM)
  {}

  // Using user-provided UVM allocate routines.
  MemoryClass(void* (*AllocUVMFn)(size_t mem_size),
              void  (*FreeUVMFn)(void* ptr))
   : memory_type_(SUNMEMTYPE_UVM),
     AllocHost_(AllocHost),
     AllocDevice_(AllocDevice),
     FreeHost_(FreeHost),
     FreeDevice_(FreeDevice),
     AllocUVM_(AllocUVMFn),
     FreeUVM_(FreeUVMFn)
  {}

  // Using user-provided host/device allocate routines.
  MemoryClass(void* (*AllocDeviceFn)(size_t mem_size),
              void  (*FreeDeviceFn)(void* ptr),
              void* (*AllocHostFn)(size_t mem_size),
              void  (*FreeHostFn)(void* ptr))
   : memory_type_(SUNMEMTYPE_DEVICE),
     AllocHost_(AllocHostFn),
     AllocDevice_(AllocDeviceFn),
     FreeHost_(FreeHostFn),
     FreeDevice_(FreeDeviceFn),
     AllocUVM_(AllocUVM),
     FreeUVM_(FreeUVM)
  {}

  explicit MemoryClass(const MemoryClass& mc)
   : memory_type_(mc.memory_type_),
     AllocHost_(mc.AllocHost_),
     AllocDevice_(mc.AllocDevice_),
     AllocUVM_(mc.AllocUVM_),
     FreeHost_(mc.FreeHost_),
     FreeDevice_(mc.FreeDevice_),
     FreeUVM_(mc.FreeUVM_)
  {}

  virtual ~MemoryClass()
  {}

  virtual SUNMemoryType GetMemoryType()
  {
    return memory_type_;
  }

  virtual int Allocate(size_t mem_size, void** host_ptr, void** device_ptr);
  virtual int Deallocate(void* host_ptr, void* device_ptr);
  virtual void CopyToDev(size_t mem_size, void* host_ptr, void* device_ptr, SUN_GPU_PREFIX(Stream_t) stream = 0);
  virtual void CopyFromDev(size_t mem_size, void* host_ptr, void* device_ptr, SUN_GPU_PREFIX(Stream_t) stream = 0);

private:
  void*(*AllocHost_)(size_t mem_size);
  void*(*AllocDevice_)(size_t mem_size);
  void*(*AllocUVM_)(size_t mem_size);
  void (*FreeHost_)(void* ptr);
  void (*FreeDevice_)(void* ptr);
  void (*FreeUVM_)(void* ptr);

  SUNMemoryType memory_type_;
};

} // namespace device
} // namespace sundials

#endif // _SUN_MEMORYCLASS_HPP_