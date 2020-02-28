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
 * CudaVectorContent class.
 *
 * Manages vector data layout for CUDA implementation of N_Vector.
 * -----------------------------------------------------------------
 */

#ifndef _SUNDIALS_CUDA_VECTOR_CONTENT_HPP_
#define _SUNDIALS_CUDA_VECTOR_CONTENT_HPP_

#include <cstdlib>
#include <iostream>

#include <cuda_runtime.h>

#include "ThreadPartitioning.hpp"
#include "sundials_device_memory.hpp"
#include <nvector/DeviceVectorContent.hpp>

namespace sundials
{
namespace device
{

template <typename TypeReal, typename TypeInt>
class CudaVectorContent : VectorContentInterface<TypeReal, TypeInt>
{
public:
  CudaVectorContent()
  : size_(0),
    mem_size_(0),
    mem_manager_(nullptr),
    own_data_(false),
    host_ptr_(nullptr),
    device_ptr_(nullptr),
    stream_(0),
    own_partitioning_(false),
    part_stream_(nullptr),
    part_reduce_(nullptr)
  {}

  CudaVectorContent(TypeInt N, MemoryClass* mem_manager, SUN_GPU_PREFIX(Stream_t) stream = 0)
  : size_(N),
    mem_size_(N*sizeof(TypeReal)),
    mem_manager_(mem_manager),
    own_data_(true),
    host_ptr_(nullptr),
    device_ptr_(nullptr),
    stream_(stream),
    own_partitioning_(true)
  {
    // Allocate the data
    mem_manager_->Allocate(mem_size_, reinterpret_cast<void**>(&host_ptr_), reinterpret_cast<void**>(&device_ptr_));

    // Allocate the partitioning
    part_stream_ = new StreamPartitioning<TypeReal, TypeInt>(size_, 256, stream_, *mem_manager_);
    part_reduce_ = new ReducePartitioning<TypeReal, TypeInt>(size_, 256, stream_, *mem_manager_);
  }

  CudaVectorContent(TypeInt N, MemoryClass* mem_manager,
                      TypeReal* host_ptr, TypeReal* device_ptr,
                      SUN_GPU_PREFIX(Stream_t) stream = 0)
  : size_(N),
    mem_size_(N*sizeof(TypeReal)),
    mem_manager_(mem_manager),
    own_data_(false),
    host_ptr_(host_ptr),
    device_ptr_(device_ptr),
    stream_(stream),
    own_partitioning_(true)
  {
    // Allocate the partitioning
    part_stream_ = new StreamPartitioning<TypeReal, TypeInt>(size_, 256, stream_, *mem_manager_);
    part_reduce_ = new ReducePartitioning<TypeReal, TypeInt>(size_, 256, stream_, *mem_manager_);
  }

  // Copy constructor does not copy data array values
  explicit CudaVectorContent(const CudaVectorContent& v)
  : size_(v.length()),
    mem_size_(v.length()*sizeof(TypeReal)),
    stream_(v.stream()),
    mem_manager_(new MemoryClass(*v.mem_manager_)),
    own_data_(true),
    host_ptr_(nullptr),
    device_ptr_(nullptr),
    own_partitioning_(true)
  {
    // Allocate the data
    mem_manager_->Allocate(mem_size_, reinterpret_cast<void**>(&host_ptr_), reinterpret_cast<void**>(&device_ptr_));

    // Allocate the partitioning
    part_stream_ = new StreamPartitioning<TypeReal, TypeInt>(v.length(), v.partStream().block(), stream_, *mem_manager_);
    part_reduce_ = new ReducePartitioning<TypeReal, TypeInt>(v.length(), v.partReduce().block(), stream_, *mem_manager_);
  }

  virtual ~CudaVectorContent()
  {
    if (own_data_) {
      mem_manager_->Deallocate(host_ptr_, device_ptr_);
    }
    if (own_partitioning_) {
      delete part_stream_;
      delete part_reduce_;
    }
    // We delete the memory manager last because it is used by the partitioning objects too.
    delete mem_manager_;
  }

  virtual TypeInt length() const
  {
    return size_;
  }

  virtual TypeReal* host()
  {
    return host_ptr_;
  }

  virtual const TypeReal* host() const
  {
    return host_ptr_;
  }

  virtual TypeReal* device()
  {
    return device_ptr_;
  }

  virtual const TypeReal* device() const
  {
    return device_ptr_;
  }

  virtual SUN_GPU_PREFIX(Stream_t) stream() const
  {
    return stream_;
  }

  virtual void stream(SUN_GPU_PREFIX(Stream_t) stream)
  {
    stream_ = stream;
    part_stream_->setStream(stream);
    part_reduce_->setStream(stream);
  }

  virtual SUNMemoryType getMemoryType() const
  {
    return mem_manager_->GetMemoryType();
  }

  virtual void copyToDevice()
  {
    mem_manager_->CopyToDevice(mem_size_, host_ptr_, device_ptr_, stream_);
  }

  virtual void copyFromDevice()
  {
    mem_manager_->CopyFromDevice(mem_size_, host_ptr_, device_ptr_, stream_);
  }

  virtual void setPartitioning(StreamPartitioning<TypeReal, TypeInt>* stream, ReducePartitioning<TypeReal, TypeInt>* reduce)
  {
     if (own_partitioning_) {
       delete part_stream_;
       delete part_reduce_;
     }
    part_stream_ = stream;
    part_reduce_ = reduce;
    own_partitioning_ = false;
  }

  virtual StreamPartitioning<TypeReal, TypeInt>& partStream() const
  {
    return *part_stream_;
  }

  virtual ReducePartitioning<TypeReal, TypeInt>& partReduce() const
  {
    return *part_reduce_;
  }

private:
  TypeInt size_;
  size_t mem_size_;
  MemoryClass* mem_manager_; // ideally this would be a unique_ptr
  bool own_data_;
  TypeReal* device_ptr_;
  TypeReal* host_ptr_;

  SUN_GPU_PREFIX(Stream_t) stream_;
  bool own_partitioning_;
  StreamPartitioning<TypeReal, TypeInt>* part_stream_; // ideally this would be a shared_ptr
  ReducePartitioning<TypeReal, TypeInt>* part_reduce_; // ideally this would be a shared_ptr

};

} // namespace device
} // namespace sundials


#endif // _SUNDIALS_CUDA_VECTOR_CONTENT_HPP_