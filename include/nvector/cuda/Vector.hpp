/*
 * -----------------------------------------------------------------
 * Programmer(s): Slaven Peles, and Cody J. Balos @ LLNL
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
 */


/*
 * Vector class
 *
 * Manages vector data layout for CUDA implementation of N_Vector.
 *
 */

#ifndef _NVECTOR_CUDA_HPP_
#define _NVECTOR_CUDA_HPP_

#include <cstdlib>
#include <iostream>

#include <cuda_runtime.h>
#include "ThreadPartitioning.hpp"

#include <nvector/nvector_cuda.h>
#include <sundials/sundials_config.h>
#include "sundials_cuda.h"

namespace suncudavec
{

template <typename T, typename I>
class Vector : public _N_VectorContent_Cuda
{

public:
  Vector(I N,
         bool use_managed_memory = false, bool allocate_data = true,
         T* const h_vec = nullptr, T* const d_vec = nullptr)
  : mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    allocfn_(nullptr),
    freefn_(nullptr)
  {
    // Initialize base class members
    length = N;
    own_data = allocate_data;
    managed_mem = use_managed_memory;
    host_data = h_vec;
    device_data = d_vec;

    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256);
    partReduce_ = new ReducePartitioning<T, I>(N, 256, use_managed_memory);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  Vector(I N, cudaStream_t stream,
         bool use_managed_memory = false, bool allocate_data = true,
         T* const h_vec = nullptr, T* const d_vec = nullptr)
  : mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    allocfn_(nullptr),
    freefn_(nullptr)
  {
    // Initialize base class members
    length = N;
    own_data = allocate_data;
    managed_mem = use_managed_memory;
    host_data = h_vec;
    device_data = d_vec;

    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256, stream);
    partReduce_ = new ReducePartitioning<T, I>(N, 256, stream, use_managed_memory);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  Vector(I N,
         SUNAllocFn allocfn, SUNFreeFn freefn,
         bool allocate_data = true)
  : mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    allocfn_(allocfn),
    freefn_(freefn)
  {
    // Initialize base class members
    length = N;
    own_data = allocate_data;
    managed_mem = true;
    host_data = nullptr;
    device_data = nullptr;

    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256);
    partReduce_ = new ReducePartitioning<T, I>(N, 256, managed_mem, allocfn, freefn);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  Vector(I N, cudaStream_t stream,
         SUNAllocFn allocfn, SUNFreeFn freefn,
         bool allocate_data = true)
  : mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    allocfn_(allocfn),
    freefn_(freefn)
  {
    // Initialize base class members
    length = N;
    own_data = allocate_data;
    managed_mem = true;
    host_data = nullptr;
    device_data = nullptr;

    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256, stream);
    partReduce_ = new ReducePartitioning<T, I>(N, 256, stream, managed_mem, allocfn, freefn);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  // Copy constructor does not copy data array values
  explicit Vector(const Vector& v)
  : mem_size_(v.size()*sizeof(T)),
    partStream_(v.partStream_),
    partReduce_(v.partReduce_),
    ownPartitioning_(false),
    allocfn_(v.allocfn_),
    freefn_(v.freefn_)
  {
    // Initialize base class members
    length = v.size();
    own_data = true;
    managed_mem = v.managed_mem;
    host_data = nullptr;
    device_data = nullptr;

    allocate();
  }

  ~Vector()
  {
    cudaError_t err;

    if (ownPartitioning_) {
      delete partReduce_;
      delete partStream_;
    }

    if (own_data) {
      if (freefn_) {
        freefn_(device_data);
        device_data = nullptr;
        host_data = nullptr;
      } else {
        if (!managed_mem)
          free(host_data);
        err = cudaFree(device_data);
        SUNDIALS_CUDA_VERIFY(err);
        device_data = nullptr;
        host_data = nullptr;
      }
    }
  }

  void allocate()
  {
    if (allocfn_) {
      allocateCustom();
    } else if (managed_mem) {
      allocateManaged();
    } else {
      allocateUnmanaged();
    }
  }

  void allocateManaged()
  {
    cudaError_t err;
    err = cudaMallocManaged((void**) &device_data, mem_size_);
    if (err != cudaSuccess)
      std::cerr << "Failed to allocate managed vector "
                << "in suncudavec::Vector::allocateManaged "
                << "(error code " << err << ")\n";
    host_data = device_data;
  }

  void allocateUnmanaged()
  {
    cudaError_t err;

    host_data = static_cast<T*>(malloc(mem_size_));
    if(host_data == nullptr)
      std::cerr << "Failed to allocate host vector "
                << "in suncudavec::Vector::allocateUnmanaged\n";

    err = cudaMalloc((void**) &device_data, mem_size_);
    SUNDIALS_CUDA_VERIFY(err);
  }

  void allocateCustom()
  {
    /* We assume managed memory when a custom allocator is provided */
    device_data = (realtype *) allocfn_(mem_size_);
    if (device_data == nullptr)
      std::cerr << "Failed to allocate vector with user-provied allocator "
                << "in suncudavec::Vector::allocateCustom()\n";
    host_data = device_data;
  }

  int size() const
  {
    return length;
  }

  T* host()
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem)
      cudaStreamSynchronize(partReduce_->stream());
    return host_data;
  }

  const T* host() const
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem)
      cudaStreamSynchronize(partReduce_->stream());
    return host_data;
  }

  T* device()
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem)
      cudaStreamSynchronize(partReduce_->stream());
    return device_data;
  }

  const T* device() const
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem)
      cudaStreamSynchronize(partReduce_->stream());
    return device_data;
  }

  bool isManaged() const
  {
    return managed_mem;
  }

  void copyToDev()
  {
    cudaError_t err;

    /* If the host and device pointers are the same, then we don't need
       to do a copy (this happens in the managed memory case), but we
       still need to synchronize the device to adhere to the unified
       memory access rules. */
    if (host_data == device_data) {
      err = cudaStreamSynchronize(partReduce_->stream());
      SUNDIALS_CUDA_VERIFY(err);
    } else {
      err = cudaMemcpyAsync(device_data, host_data, mem_size_, cudaMemcpyHostToDevice,
                            partReduce_->stream());
      SUNDIALS_CUDA_VERIFY(err);
    }
  }

  void copyFromDev()
  {
    cudaError_t err;

    /* If the host and device pointers are the same, then we don't need
       to do a copy (this happens in the managed memory case), but we
       still need to synchronize the device to adhere to the unified
       memory access rules. */
    if (host_data == device_data) {
      err = cudaStreamSynchronize(partReduce_->stream());
      SUNDIALS_CUDA_VERIFY(err);
    } else {
      err = cudaMemcpyAsync(host_data, device_data, mem_size_, cudaMemcpyDeviceToHost,
                            partReduce_->stream());
      SUNDIALS_CUDA_VERIFY(err);
    }
  }

  void setPartitioning(ThreadPartitioning<T, I>* stream, ThreadPartitioning<T, I>* reduce)
  {
     if (ownPartitioning_) {
       delete partStream_;
       delete partReduce_;
     }
    partStream_ = stream;
    partReduce_ = reduce;
    ownPartitioning_ = false;
  }

  ThreadPartitioning<T, I>& partStream() const
  {
    return *partStream_;
  }

  ThreadPartitioning<T, I>& partReduce() const
  {
    return *partReduce_;
  }


private:
  // I length;
  // T* host_data;
  // T* device_data;
  // bool own_data;
  // bool managed_mem;
  I mem_size_;
  ThreadPartitioning<T, I>* partStream_;
  ThreadPartitioning<T, I>* partReduce_;
  bool ownPartitioning_;
  SUNAllocFn allocfn_;
  SUNFreeFn freefn_;
};


} // namespace suncudavec




#endif // _NVECTOR_CUDA_HPP_
