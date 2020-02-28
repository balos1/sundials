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

#include "sundials_device_memory.hpp"

namespace sundials
{
namespace device
{

void* AllocHost(size_t mem_size)
{
  void* host_data;
  host_data = malloc(mem_size);
  if (host_data == nullptr) {
    std::cerr << "Failed to allocate host data "
              << "in AllocHost\n";
  }
  return host_data;
}

void* AllocDevice(size_t mem_size)
{
  SUN_GPU_PREFIX(Error_t) err;
  void* device_ptr;
  err = SUN_GPU_PREFIX(Malloc)(&device_ptr, mem_size);
  if (err != SUN_GPU_PREFIX(Success)) {
    std::cerr << "Failed to allocate device data "
              << "in AllocDevice "
              << "(error code " << err << ")\n";
  }
  return device_ptr;
}

void* AllocUVM(size_t mem_size)
{
  SUN_GPU_PREFIX(Error_t) err;
  void* uvm_data;
  err = SUN_GPU_PREFIX(MallocManaged)(&uvm_data, mem_size);
  if (err != SUN_GPU_PREFIX(Success)) {
    std::cerr << "Failed to allocate managed data "
              << "in AllocUVM"
              << "(error code " << err << ")\n";
  }
  return uvm_data;
}

void FreeHost(void* ptr)
{
  free(ptr);
}

void FreeDevice(void* ptr)
{
  SUN_GPU_PREFIX(Error_t) err;
  err = SUN_GPU_PREFIX(Free)(ptr);
  if (err != SUN_GPU_PREFIX(Success))
    std::cerr << "Failed to deallocate device data "
              << "in FreeDevice "
              << "(error code " << err << ")\n";
}

void FreeUVM(void* ptr)
{
  SUN_GPU_PREFIX(Error_t) err;
  err = SUN_GPU_PREFIX(Free)(ptr);
  if (err != SUN_GPU_PREFIX(Success))
    std::cerr << "Failed to deallocate managed data "
              << "in SUNMemoryAllocator::FreeUVM "
              << "(error code " << err << ")\n";
}

int MemoryClass::Allocate(size_t mem_size, void** host_ptr, void** device_ptr)
{
  if (GetMemoryType() == SUNMEMTYPE_HOST || GetMemoryType() == SUNMEMTYPE_DEVICE) {
    *host_ptr = AllocHost_(mem_size);
    if (*host_ptr == nullptr) return -1;
  }

  if (GetMemoryType() == SUNMEMTYPE_DEVICE) {
    *device_ptr = AllocDevice_(mem_size);
    if (*device_ptr == nullptr) return -1;
  }

  if (GetMemoryType() == SUNMEMTYPE_UVM) {
    *device_ptr = AllocUVM_(mem_size);
    *host_ptr = *device_ptr;
    if (*host_ptr == nullptr) return -1;
  }

  return 0;
}

int MemoryClass::Deallocate(void* host_ptr, void* device_ptr)
{
  if (GetMemoryType() == SUNMEMTYPE_HOST || GetMemoryType() == SUNMEMTYPE_DEVICE) {
    FreeHost_(host_ptr);
    host_ptr = nullptr;
  }

  if (GetMemoryType() == SUNMEMTYPE_DEVICE) {
    FreeDevice_(device_ptr);
    device_ptr = nullptr;
  }

  if (GetMemoryType() == SUNMEMTYPE_UVM) {
    FreeUVM_(device_ptr);
    device_ptr = nullptr;
    host_ptr = device_ptr;
  }

  return 0;
}

void MemoryClass::CopyToDevice(size_t mem_size, void* host_ptr, void* device_ptr, SUN_GPU_PREFIX(Stream_t) stream)
{
  SUN_GPU_PREFIX(Error_t) err;

  /* If the host and device pointers are the same, then we don't need
      to do a copy (this happens in the managed memory case), but we
      still need to synchronize the device to adhere to the unified
      memory access rules. */
  if (GetMemoryType() == SUNMEMTYPE_UVM) {
    err = SUN_GPU_PREFIX(StreamSynchronize)(stream);
    if(err != SUN_GPU_PREFIX(Success))
      std::cerr << "Failed to synchronize stream in "
                << "MemoryClass::CopyToDevice "
                << "(error code " << err << ")\n";
  } else {
    err = SUN_GPU_PREFIX(MemcpyAsync)(device_ptr, host_ptr, mem_size,
                                      SUN_GPU_PREFIX(MemcpyHostToDevice),
                                      stream);
    if(err != SUN_GPU_PREFIX(Success))
      std::cerr << "Failed to copy vector from host to device in "
                << "MemoryClass::CopyToDevice "
                << "(error code " << err << ")\n";
  }
}

void MemoryClass::CopyFromDevice(size_t mem_size, void* host_ptr, void* device_ptr, SUN_GPU_PREFIX(Stream_t) stream)
{
  SUN_GPU_PREFIX(Error_t) err;

  /* If the host and device pointers are the same, then we don't need
      to do a copy (this happens in the managed memory case), but we
      still need to synchronize the device to adhere to the unified
      memory access rules. */
  if (GetMemoryType() == SUNMEMTYPE_UVM) {
    err = SUN_GPU_PREFIX(StreamSynchronize)(stream);
    if(err != SUN_GPU_PREFIX(Success))
      std::cerr << "Failed to synchronize stream in "
                << "MemoryClass::CopyFromDevice "
                << "(error code " << err << ")\n";
  } else {
    err = SUN_GPU_PREFIX(MemcpyAsync)(host_ptr, device_ptr, mem_size,
                                      SUN_GPU_PREFIX(MemcpyDeviceToHost),
                                      stream);
    if(err != SUN_GPU_PREFIX(Success))
      std::cerr << "Failed to copy vector from device to host in "
                << "MemoryClass::CopyFromDevice "
                << "(error code " << err << ")\n";
  }
}

} // namespace device
} // namespace sundials