/*
 * -----------------------------------------------------------------
 * Programmer(s): Slaven Peles, Cody J. Balos @ LLNL
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


#ifndef _THREAD_PARTITIONING_HPP_
#define _THREAD_PARTITIONING_HPP_

#include <iostream>
#include <cuda_runtime.h>

#include "sundials_device_memory.hpp"

namespace sundials
{
namespace device
{

template<class T, class I>
class ThreadPartitioning
{

public:
  ThreadPartitioning()
  : block_(1),
    grid_(1),
    shMemSize_(0),
    stream_(0),
    bufferSize_(0),
    deviceBuffer_(nullptr),
    hostBuffer_(nullptr)
  {}

  ThreadPartitioning(unsigned block, MemoryClass& mc)
  : block_(block),
    grid_(1),
    shMemSize_(0),
    stream_(0),
    bufferSize_(0),
    deviceBuffer_(nullptr),
    hostBuffer_(nullptr),
    memManager_(mc)
  {}

  explicit ThreadPartitioning(ThreadPartitioning<T, I>& p)
  : block_(p.block_),
    grid_(p.grid_),
    shMemSize_(p.shMemSize_),
    stream_(p.stream_),
    bufferSize_(p.bufferSize_),
    deviceBuffer_(nullptr),
    hostBuffer_(nullptr),
    memManager_(p.memManager_)
  {}

  virtual ~ThreadPartitioning(){}

  unsigned grid() const
  {
    return grid_;
  }

  unsigned block() const
  {
    return block_;
  }

  unsigned shmem() const
  {
    return shMemSize_;
  }

  cudaStream_t stream() const
  {
    return stream_;
  }

  unsigned int bufferSize()
  {
    return bufferSize_;
  }

  T* devBuffer()
  {
    return deviceBuffer_;
  }

  const T* devBuffer() const
  {
    return deviceBuffer_;
  }

  T* hostBuffer()
  {
    return hostBuffer_;
  }

  const T* hostBuffer() const
  {
    return hostBuffer_;
  }

  void setStream(const cudaStream_t& stream)
  {
    stream_ = stream;
  }

  virtual void copyFromDevBuffer(unsigned int n) const
  {
    std::cerr << "Trying to copy buffer from base class in "
              << "suncudavec::ThreadPartitioning::copyFromDevBuffer\n";
  }

  /* pure virtual functions to get the relevant partitioning information */
  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize, cudaStream_t& stream) = 0;
  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize) = 0;

protected:
  unsigned block_;
  unsigned grid_;
  unsigned shMemSize_;
  size_t bufferSize_;
  cudaStream_t stream_;
  T* deviceBuffer_;
  T* hostBuffer_;

  MemoryClass& memManager_;

}; // class ThreadPartitioning



template<class T, class I>
class StreamPartitioning : public ThreadPartitioning<T, I>
{
  using ThreadPartitioning<T, I>::block_;
  using ThreadPartitioning<T, I>::grid_;
  using ThreadPartitioning<T, I>::stream_;

public:
  StreamPartitioning(I N, unsigned block, MemoryClass &mc)
  : ThreadPartitioning<T, I>(block, mc)
  {
    grid_ = (N + block_ - 1) / block_;
  }

  StreamPartitioning(I N, unsigned block, cudaStream_t stream, MemoryClass &mc)
  : ThreadPartitioning<T, I>(block, mc)
  {
    grid_ = (N + block_ - 1) / block_;
    stream_ = stream;
  }

  explicit StreamPartitioning(StreamPartitioning<T, I>& p)
  : ThreadPartitioning<T, I>(p)
  {}

  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize,
                               cudaStream_t& stream)
  {
    block = block_;
    grid  = (N + block_ - 1) / block_;
    shMemSize = 0;
    stream = stream_;

    return 0;
  }

  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize)
  {
    block = block_;
    grid  = (N + block_ - 1) / block_;
    shMemSize = 0;

    return 0;
  }

}; // class StreamPartitioning


template<class T, class I=int>
class ReducePartitioning : public ThreadPartitioning<T, I>
{
  using ThreadPartitioning<T, I>::block_;
  using ThreadPartitioning<T, I>::grid_;
  using ThreadPartitioning<T, I>::shMemSize_;
  using ThreadPartitioning<T, I>::stream_;
  using ThreadPartitioning<T, I>::bufferSize_;
  using ThreadPartitioning<T, I>::deviceBuffer_;
  using ThreadPartitioning<T, I>::hostBuffer_;
  using ThreadPartitioning<T, I>::memManager_;

public:
  ReducePartitioning(I N, unsigned block, MemoryClass& mc)
  : ThreadPartitioning<T, I>(block, mc)
  {
    grid_ = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize_ = block_*sizeof(T);
    bufferSize_ = grid_*sizeof(T);
    memManager_.Allocate(bufferSize_, reinterpret_cast<void**>(&hostBuffer_), reinterpret_cast<void**>(&deviceBuffer_));
  }

  ReducePartitioning(I N, unsigned block, cudaStream_t stream, MemoryClass& mc)
  : ThreadPartitioning<T, I>(block, mc)
  {
    stream_ = stream;
    grid_ = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize_ = block_*sizeof(T);
    bufferSize_ = grid_*sizeof(T);
    memManager_.Allocate(bufferSize_, reinterpret_cast<void**>(&hostBuffer_), reinterpret_cast<void**>(&deviceBuffer_));
  }

  explicit ReducePartitioning(ReducePartitioning<T, I>& p)
  : ThreadPartitioning<T, I>(p)
  {
    memManager_.Allocate(bufferSize_, reinterpret_cast<void**>(&hostBuffer_), reinterpret_cast<void**>(&deviceBuffer_));
  }

  ~ReducePartitioning()
  {
    memManager_.Deallocate(hostBuffer_, deviceBuffer_);
  }

  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize,
                               cudaStream_t& stream)
  {
    block = block_;
    grid  = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize = block_ * sizeof(T);
    stream = stream_;
    return 0;
  }

  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize)
  {
    block = block_;
    grid  = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize = block_ * sizeof(T);
    return 0;
  }

  virtual void copyFromDevBuffer(unsigned int n) const
  {
    memManager_.CopyFromDev(bufferSize_, hostBuffer_, deviceBuffer_);
  }

  static unsigned calcBufferSize(I N, unsigned block)
  {
    return (N + (block * 2 - 1)) / (block * 2) * sizeof(T);
  }

}; // class ReducePartitioning


} // namespace device
} // namespace sundials

#endif // _THREAD_PARTITIONING_HPP_
