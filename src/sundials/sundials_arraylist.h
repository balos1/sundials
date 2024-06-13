/* -----------------------------------------------------------------
 * Programmer: Cody J. Balos @ LLNL
 * -----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2024, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------
 * Implementation of a resizable container similar to a std::vector.
 * The values can be anything but data must be contiguous.
 #
 # To use the ArrayList, first define TTYPE with your data type
 # before including this header. If you need ArrayLists that hold
 # different types in the same file, then define TTYPE for the first,
 # include this header, then repeat.
 * -----------------------------------------------------------------*/

#include <stdlib.h>
#include <sundials/sundials_core.h>

#ifndef TTYPE
#error "Must define template type for SUNArrayList"
#endif

#define CONCAT(a, b)            a##b
#define PASTE(a, b)             CONCAT(a, b)
#define MAKE_NAME(prefix, name) PASTE(prefix, PASTE(_, name))

#define SUNArrayListTtype_s MAKE_NAME(SUNArrayList, PASTE(TTYPE, _s))
#define SUNArrayListTtype   MAKE_NAME(SUNArrayList, TTYPE)

typedef struct SUNArrayListTtype_s* SUNArrayListTtype;

struct SUNArrayListTtype_s
{
  size_t size;
  size_t capacity;
  TTYPE* values;
};

// This constant controls how much space will be allocated when a resize is needed.
// The new capacity is GROWTH_FACTOR*current_capacity.
// Some std::vector implementations use 2, but 1.5 will be more conservative in terms
// of the memory usage but yields a larger constant factor in terms of the
// amorirtized constant time complexity.
#define GROWTH_FACTOR 1.5

static inline SUNArrayListTtype MAKE_NAME(SUNArrayListTtype,
                                          New)(size_t init_capacity)
{
  SUNArrayListTtype self =
    (SUNArrayListTtype)malloc(sizeof(struct SUNArrayListTtype_s));
  self->size     = 0;
  self->capacity = init_capacity > 0 ? init_capacity : 1;
  self->values   = (TTYPE*)malloc(sizeof(TTYPE) * self->capacity);
  return self;
}

static inline void MAKE_NAME(SUNArrayListTtype, Destroy)(SUNArrayListTtype* self)
{
  if (!(*self)) return;
  free((*self)->values);
  free(*self);
  *self = NULL;
}

static inline sunbooleantype MAKE_NAME(SUNArrayListTtype,
                                       IsEmpty)(SUNArrayListTtype self)
{
  return self->size == 0;
}

static inline void MAKE_NAME(SUNArrayListTtype, Resize)(SUNArrayListTtype self,
                                                        size_t new_capacity)
{
  if (new_capacity <= self->capacity) return;
  TTYPE* new_values = (TTYPE*)realloc(self->values, sizeof(TTYPE) * new_capacity);
  self->values   = new_values;
  self->capacity = new_capacity;
}

static inline void MAKE_NAME(SUNArrayListTtype, Grow)(SUNArrayListTtype self)
{
  if (self->size == self->capacity)
  {
    size_t new_capacity = (size_t)(ceil(self->capacity * GROWTH_FACTOR));
    MAKE_NAME(SUNArrayListTtype, Resize)(self, new_capacity);
  }
}

static inline void MAKE_NAME(SUNArrayListTtype,
                             PushBack)(SUNArrayListTtype self, TTYPE element)
{
  if (self->size == self->capacity)
  {
    MAKE_NAME(SUNArrayListTtype, Grow)(self);
  }
  self->values[self->size++] = element;
}

static inline TTYPE* MAKE_NAME(SUNArrayListTtype, At)(SUNArrayListTtype self,
                                                      size_t index)
{
  if (index < 0 || index >= self->size)
  {
    // Handle index out of bounds
    return NULL;
  }
  return &(self->values[index]);
}

static inline void MAKE_NAME(SUNArrayListTtype, Set)(SUNArrayListTtype self,
                                                     size_t index, TTYPE element)
{
  if (index < 0 || index >= self->size)
  {
    // Handle index out of bounds
    return;
  }
  self->values[index] = element;
}

static inline void MAKE_NAME(SUNArrayListTtype, PopBack)(SUNArrayListTtype self)
{
  if (self->size == 0) return;
  self->size--;
}

static inline void MAKE_NAME(SUNArrayListTtype, Erase)(SUNArrayListTtype self,
                                                       size_t index)
{
  static TTYPE nullish;
  if (self->size == 0) return;
  MAKE_NAME(SUNArrayListTtype, Set)(self, index, nullish);
  for (size_t i = index; i < self->size - 1; i++)
  {
    self->values[i]     = self->values[i + 1];
    self->values[i + 1] = nullish;
  }
  self->size -= 1;
}

static inline size_t MAKE_NAME(SUNArrayListTtype, Size)(SUNArrayListTtype self)
{
  return self->size;
}

static inline size_t MAKE_NAME(SUNArrayListTtype, Capacity)(SUNArrayListTtype self)
{
  return self->capacity;
}

#undef TTYPE
