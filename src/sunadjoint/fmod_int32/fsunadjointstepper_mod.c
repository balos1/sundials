/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.0
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */

/* ---------------------------------------------------------------
 * Programmer(s): Auto-generated by swig.
 * ---------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2024, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
 *  This section contains generic SWIG labels for method/variable
 *  declarations/attributes, and other compiler dependent labels.
 * ----------------------------------------------------------------------------- */

/* template workaround for compilers that cannot correctly implement the C++ standard */
#ifndef SWIGTEMPLATEDISAMBIGUATOR
# if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x560)
#  define SWIGTEMPLATEDISAMBIGUATOR template
# elif defined(__HP_aCC)
/* Needed even with `aCC -AA' when `aCC -V' reports HP ANSI C++ B3910B A.03.55 */
/* If we find a maximum version that requires this, the test would be __HP_aCC <= 35500 for A.03.55 */
#  define SWIGTEMPLATEDISAMBIGUATOR template
# else
#  define SWIGTEMPLATEDISAMBIGUATOR
# endif
#endif

/* inline attribute */
#ifndef SWIGINLINE
# if defined(__cplusplus) || (defined(__GNUC__) && !defined(__STRICT_ANSI__))
#   define SWIGINLINE inline
# else
#   define SWIGINLINE
# endif
#endif

/* attribute recognised by some compilers to avoid 'unused' warnings */
#ifndef SWIGUNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define SWIGUNUSED __attribute__ ((__unused__))
#   else
#     define SWIGUNUSED
#   endif
# elif defined(__ICC)
#   define SWIGUNUSED __attribute__ ((__unused__))
# else
#   define SWIGUNUSED
# endif
#endif

#ifndef SWIG_MSC_UNSUPPRESS_4505
# if defined(_MSC_VER)
#   pragma warning(disable : 4505) /* unreferenced local function has been removed */
# endif
#endif

#ifndef SWIGUNUSEDPARM
# ifdef __cplusplus
#   define SWIGUNUSEDPARM(p)
# else
#   define SWIGUNUSEDPARM(p) p SWIGUNUSED
# endif
#endif

/* internal SWIG method */
#ifndef SWIGINTERN
# define SWIGINTERN static SWIGUNUSED
#endif

/* internal inline SWIG method */
#ifndef SWIGINTERNINLINE
# define SWIGINTERNINLINE SWIGINTERN SWIGINLINE
#endif

/* qualifier for exported *const* global data variables*/
#ifndef SWIGEXTERN
# ifdef __cplusplus
#   define SWIGEXTERN extern
# else
#   define SWIGEXTERN
# endif
#endif

/* exporting methods */
#if defined(__GNUC__)
#  if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#    ifndef GCC_HASCLASSVISIBILITY
#      define GCC_HASCLASSVISIBILITY
#    endif
#  endif
#endif

#ifndef SWIGEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define SWIGEXPORT
#   else
#     define SWIGEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define SWIGEXPORT __attribute__ ((visibility("default")))
#   else
#     define SWIGEXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef SWIGSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define SWIGSTDCALL __stdcall
# else
#   define SWIGSTDCALL
# endif
#endif

/* Deal with Microsoft's attempt at deprecating C standard runtime functions */
#if !defined(SWIG_NO_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
# define _CRT_SECURE_NO_DEPRECATE
#endif

/* Deal with Microsoft's attempt at deprecating methods in the standard C++ library */
#if !defined(SWIG_NO_SCL_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_SCL_SECURE_NO_DEPRECATE)
# define _SCL_SECURE_NO_DEPRECATE
#endif

/* Deal with Apple's deprecated 'AssertMacros.h' from Carbon-framework */
#if defined(__APPLE__) && !defined(__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES)
# define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
#endif

/* Intel's compiler complains if a variable which was never initialised is
 * cast to void, which is a common idiom which we use to indicate that we
 * are aware a variable isn't used.  So we just silence that warning.
 * See: https://github.com/swig/swig/issues/192 for more discussion.
 */
#ifdef __INTEL_COMPILER
# pragma warning disable 592
#endif

/*  Errors in SWIG */
#define  SWIG_UnknownError    	   -1
#define  SWIG_IOError        	   -2
#define  SWIG_RuntimeError   	   -3
#define  SWIG_IndexError     	   -4
#define  SWIG_TypeError      	   -5
#define  SWIG_DivisionByZero 	   -6
#define  SWIG_OverflowError  	   -7
#define  SWIG_SyntaxError    	   -8
#define  SWIG_ValueError     	   -9
#define  SWIG_SystemError    	   -10
#define  SWIG_AttributeError 	   -11
#define  SWIG_MemoryError    	   -12
#define  SWIG_NullReferenceError   -13




#include <assert.h>
#define SWIG_exception_impl(DECL, CODE, MSG, RETURNNULL) \
 { printf("In " DECL ": " MSG); assert(0); RETURNNULL; }


enum {
    SWIG_MEM_OWN = 0x01,
    SWIG_MEM_RVALUE = 0x02,
    SWIG_MEM_CONST = 0x04
};


#define SWIG_check_mutable(SWIG_CLASS_WRAPPER, TYPENAME, FNAME, FUNCNAME, RETURNNULL) \
    if ((SWIG_CLASS_WRAPPER).cmemflags & SWIG_MEM_CONST) { \
        SWIG_exception_impl(FUNCNAME, SWIG_TypeError, \
            "Cannot pass const " TYPENAME " (class " FNAME ") " \
            "as a mutable reference", \
            RETURNNULL); \
    }


#define SWIG_check_nonnull(SWIG_CLASS_WRAPPER, TYPENAME, FNAME, FUNCNAME, RETURNNULL) \
  if (!(SWIG_CLASS_WRAPPER).cptr) { \
    SWIG_exception_impl(FUNCNAME, SWIG_TypeError, \
                        "Cannot pass null " TYPENAME " (class " FNAME ") " \
                        "as a reference", RETURNNULL); \
  }


#define SWIG_check_mutable_nonnull(SWIG_CLASS_WRAPPER, TYPENAME, FNAME, FUNCNAME, RETURNNULL) \
    SWIG_check_nonnull(SWIG_CLASS_WRAPPER, TYPENAME, FNAME, FUNCNAME, RETURNNULL); \
    SWIG_check_mutable(SWIG_CLASS_WRAPPER, TYPENAME, FNAME, FUNCNAME, RETURNNULL);


#include <stdio.h>
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(_WATCOM)
# ifndef snprintf
#  define snprintf _snprintf
# endif
#endif


/* Support for the `contract` feature.
 *
 * Note that RETURNNULL is first because it's inserted via a 'Replaceall' in
 * the fortran.cxx file.
 */
#define SWIG_contract_assert(RETURNNULL, EXPR, MSG) \
 if (!(EXPR)) { SWIG_exception_impl("$decl", SWIG_ValueError, MSG, RETURNNULL); } 


#define SWIGVERSION 0x040000 
#define SWIG_VERSION SWIGVERSION


#define SWIG_as_voidptr(a) (void *)((const void *)(a)) 
#define SWIG_as_voidptrptr(a) ((void)SWIG_as_voidptr(*a),(void**)(a)) 


#include <stdint.h>


#include "sunadjoint/sunadjoint_stepper.h"


typedef struct {
    void* cptr;
    int cmemflags;
} SwigClassWrapper;


SWIGINTERN SwigClassWrapper SwigClassWrapper_uninitialized() {
    SwigClassWrapper result;
    result.cptr = NULL;
    result.cmemflags = 0;
    return result;
}


#include <stdlib.h>
#ifdef _MSC_VER
# ifndef strtoull
#  define strtoull _strtoui64
# endif
# ifndef strtoll
#  define strtoll _strtoi64
# endif
#endif


#include <string.h>


SWIGINTERN void SWIG_assign(SwigClassWrapper* self, SwigClassWrapper other) {
  if (self->cptr == NULL) {
    /* LHS is unassigned */
    if (other.cmemflags & SWIG_MEM_RVALUE) {
      /* Capture pointer from RHS, clear 'moving' flag */
      self->cptr = other.cptr;
      self->cmemflags = other.cmemflags & (~SWIG_MEM_RVALUE);
    } else {
      /* Become a reference to the other object */
      self->cptr = other.cptr;
      self->cmemflags = other.cmemflags & (~SWIG_MEM_OWN);
    }
  } else if (other.cptr == NULL) {
    /* Replace LHS with a null pointer */
    free(self->cptr);
    *self = SwigClassWrapper_uninitialized();
  } else {
    if (self->cmemflags & SWIG_MEM_OWN) {
      free(self->cptr);
    }
    self->cptr = other.cptr;
    if (other.cmemflags & SWIG_MEM_RVALUE) {
      /* Capture RHS */
      self->cmemflags = other.cmemflags & ~SWIG_MEM_RVALUE;
    } else {
      /* Point to RHS */
      self->cmemflags = other.cmemflags & ~SWIG_MEM_OWN;
    }
  }
}

SWIGEXPORT void _wrap_SUNAdjointStepper__adj_sunstepper_set(SwigClassWrapper const *farg1, void *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNStepper arg2 = (SUNStepper) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::adj_sunstepper", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNStepper)(farg2);
  if (arg1) (arg1)->adj_sunstepper = arg2;
}


SWIGEXPORT void * _wrap_SUNAdjointStepper__adj_sunstepper_get(SwigClassWrapper const *farg1) {
  void * fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNStepper result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::adj_sunstepper", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNStepper) ((arg1)->adj_sunstepper);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__fwd_sunstepper_set(SwigClassWrapper const *farg1, void *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNStepper arg2 = (SUNStepper) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::fwd_sunstepper", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNStepper)(farg2);
  if (arg1) (arg1)->fwd_sunstepper = arg2;
}


SWIGEXPORT void * _wrap_SUNAdjointStepper__fwd_sunstepper_get(SwigClassWrapper const *farg1) {
  void * fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNStepper result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::fwd_sunstepper", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNStepper) ((arg1)->fwd_sunstepper);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__checkpoint_scheme_set(SwigClassWrapper const *farg1, SUNAdjointCheckpointScheme farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNAdjointCheckpointScheme arg2 = (SUNAdjointCheckpointScheme) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::checkpoint_scheme", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNAdjointCheckpointScheme)(farg2);
  if (arg1) (arg1)->checkpoint_scheme = arg2;
}


SWIGEXPORT SUNAdjointCheckpointScheme _wrap_SUNAdjointStepper__checkpoint_scheme_get(SwigClassWrapper const *farg1) {
  SUNAdjointCheckpointScheme fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNAdjointCheckpointScheme result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::checkpoint_scheme", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNAdjointCheckpointScheme) ((arg1)->checkpoint_scheme);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__tf_set(SwigClassWrapper const *farg1, double const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  sunrealtype arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::tf", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (sunrealtype)(*farg2);
  if (arg1) (arg1)->tf = arg2;
}


SWIGEXPORT double _wrap_SUNAdjointStepper__tf_get(SwigClassWrapper const *farg1) {
  double fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  sunrealtype result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::tf", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (sunrealtype) ((arg1)->tf);
  fresult = (sunrealtype)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__step_idx_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::step_idx", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->step_idx = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__step_idx_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::step_idx", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->step_idx);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__final_step_idx_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::final_step_idx", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->final_step_idx = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__final_step_idx_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::final_step_idx", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->final_step_idx);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__Jac_set(SwigClassWrapper const *farg1, SUNMatrix farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNMatrix arg2 = (SUNMatrix) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::Jac", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNMatrix)(farg2);
  if (arg1) (arg1)->Jac = arg2;
}


SWIGEXPORT SUNMatrix _wrap_SUNAdjointStepper__Jac_get(SwigClassWrapper const *farg1) {
  SUNMatrix fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNMatrix result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::Jac", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNMatrix) ((arg1)->Jac);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__JacP_set(SwigClassWrapper const *farg1, SUNMatrix farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNMatrix arg2 = (SUNMatrix) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JacP", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNMatrix)(farg2);
  if (arg1) (arg1)->JacP = arg2;
}


SWIGEXPORT SUNMatrix _wrap_SUNAdjointStepper__JacP_get(SwigClassWrapper const *farg1) {
  SUNMatrix fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNMatrix result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JacP", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNMatrix) ((arg1)->JacP);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__JacFn_set(SwigClassWrapper const *farg1, SUNJacFn farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacFn arg2 = (SUNJacFn) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JacFn", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNJacFn)(farg2);
  if (arg1) (arg1)->JacFn = arg2;
}


SWIGEXPORT SUNJacFn _wrap_SUNAdjointStepper__JacFn_get(SwigClassWrapper const *farg1) {
  SUNJacFn fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacFn result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JacFn", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNJacFn) ((arg1)->JacFn);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__JacPFn_set(SwigClassWrapper const *farg1, SUNJacFn farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacFn arg2 = (SUNJacFn) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JacPFn", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNJacFn)(farg2);
  if (arg1) (arg1)->JacPFn = arg2;
}


SWIGEXPORT SUNJacFn _wrap_SUNAdjointStepper__JacPFn_get(SwigClassWrapper const *farg1) {
  SUNJacFn fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacFn result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JacPFn", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNJacFn) ((arg1)->JacPFn);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__JvpFn_set(SwigClassWrapper const *farg1, SUNJacTimesFn farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn arg2 = (SUNJacTimesFn) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JvpFn", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNJacTimesFn)(farg2);
  if (arg1) (arg1)->JvpFn = arg2;
}


SWIGEXPORT SUNJacTimesFn _wrap_SUNAdjointStepper__JvpFn_get(SwigClassWrapper const *farg1) {
  SUNJacTimesFn fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JvpFn", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNJacTimesFn) ((arg1)->JvpFn);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__JPvpFn_set(SwigClassWrapper const *farg1, SUNJacTimesFn farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn arg2 = (SUNJacTimesFn) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JPvpFn", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNJacTimesFn)(farg2);
  if (arg1) (arg1)->JPvpFn = arg2;
}


SWIGEXPORT SUNJacTimesFn _wrap_SUNAdjointStepper__JPvpFn_get(SwigClassWrapper const *farg1) {
  SUNJacTimesFn fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::JPvpFn", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNJacTimesFn) ((arg1)->JPvpFn);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__vJpFn_set(SwigClassWrapper const *farg1, SUNJacTimesFn farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn arg2 = (SUNJacTimesFn) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::vJpFn", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNJacTimesFn)(farg2);
  if (arg1) (arg1)->vJpFn = arg2;
}


SWIGEXPORT SUNJacTimesFn _wrap_SUNAdjointStepper__vJpFn_get(SwigClassWrapper const *farg1) {
  SUNJacTimesFn fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::vJpFn", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNJacTimesFn) ((arg1)->vJpFn);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__vJPpFn_set(SwigClassWrapper const *farg1, SUNJacTimesFn farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn arg2 = (SUNJacTimesFn) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::vJPpFn", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNJacTimesFn)(farg2);
  if (arg1) (arg1)->vJPpFn = arg2;
}


SWIGEXPORT SUNJacTimesFn _wrap_SUNAdjointStepper__vJPpFn_get(SwigClassWrapper const *farg1) {
  SUNJacTimesFn fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNJacTimesFn result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::vJPpFn", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNJacTimesFn) ((arg1)->vJPpFn);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__nst_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nst", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->nst = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__nst_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nst", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->nst);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__njeval_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njeval", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->njeval = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__njeval_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njeval", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->njeval);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__njpeval_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njpeval", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->njpeval = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__njpeval_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njpeval", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->njpeval);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__njtimesv_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njtimesv", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->njtimesv = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__njtimesv_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njtimesv", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->njtimesv);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__njptimesv_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njptimesv", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->njptimesv = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__njptimesv_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::njptimesv", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->njptimesv);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__nvtimesj_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nvtimesj", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->nvtimesj = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__nvtimesj_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nvtimesj", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->nvtimesj);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__nvtimesjp_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nvtimesjp", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->nvtimesjp = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__nvtimesjp_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nvtimesjp", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->nvtimesjp);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__nrecompute_set(SwigClassWrapper const *farg1, int64_t const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t arg2 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nrecompute", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (int64_t)(*farg2);
  if (arg1) (arg1)->nrecompute = arg2;
}


SWIGEXPORT int64_t _wrap_SUNAdjointStepper__nrecompute_get(SwigClassWrapper const *farg1) {
  int64_t fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  int64_t result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::nrecompute", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result =  ((arg1)->nrecompute);
  fresult = (int64_t)(result);
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__user_data_set(SwigClassWrapper const *farg1, void *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  void *arg2 = (void *) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::user_data", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (void *)(farg2);
  if (arg1) (arg1)->user_data = arg2;
}


SWIGEXPORT void * _wrap_SUNAdjointStepper__user_data_get(SwigClassWrapper const *farg1) {
  void * fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  void *result = 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::user_data", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (void *) ((arg1)->user_data);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__content_set(SwigClassWrapper const *farg1, void *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  void *arg2 = (void *) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::content", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (void *)(farg2);
  if (arg1) (arg1)->content = arg2;
}


SWIGEXPORT void * _wrap_SUNAdjointStepper__content_get(SwigClassWrapper const *farg1) {
  void * fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  void *result = 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::content", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (void *) ((arg1)->content);
  fresult = result;
  return fresult;
}


SWIGEXPORT void _wrap_SUNAdjointStepper__sunctx_set(SwigClassWrapper const *farg1, void *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNContext arg2 = (SUNContext) 0 ;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::sunctx", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  arg2 = (SUNContext)(farg2);
  if (arg1) (arg1)->sunctx = arg2;
}


SWIGEXPORT void * _wrap_SUNAdjointStepper__sunctx_get(SwigClassWrapper const *farg1) {
  void * fresult ;
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  SUNContext result;
  
  SWIG_check_mutable_nonnull(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::sunctx", return 0);
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  result = (SUNContext) ((arg1)->sunctx);
  fresult = result;
  return fresult;
}


SWIGEXPORT SwigClassWrapper _wrap_new_SUNAdjointStepper_() {
  SwigClassWrapper fresult ;
  struct SUNAdjointStepper_ *result = 0 ;
  
  result = (struct SUNAdjointStepper_ *)calloc(1, sizeof(struct SUNAdjointStepper_));
  fresult.cptr = result;
  fresult.cmemflags = SWIG_MEM_RVALUE | (1 ? SWIG_MEM_OWN : 0);
  return fresult;
}


SWIGEXPORT void _wrap_delete_SUNAdjointStepper_(SwigClassWrapper *farg1) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  
  SWIG_check_mutable(*farg1, "struct SUNAdjointStepper_ *", "SUNAdjointStepper_", "SUNAdjointStepper_::~SUNAdjointStepper_()", return );
  arg1 = (struct SUNAdjointStepper_ *)(farg1->cptr);
  free((char *) arg1);
}


SWIGEXPORT void _wrap_SUNAdjointStepper__op_assign__(SwigClassWrapper *farg1, SwigClassWrapper const *farg2) {
  struct SUNAdjointStepper_ *arg1 = (struct SUNAdjointStepper_ *) 0 ;
  struct SUNAdjointStepper_ *arg2 = 0 ;
  
  (void)sizeof(arg1);
  (void)sizeof(arg2);
  SWIG_assign(farg1, *farg2);
  
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_Create(void *farg1, void *farg2, int64_t const *farg3, N_Vector farg4, double const *farg5, SUNAdjointCheckpointScheme farg6, void *farg7, void *farg8) {
  int fresult ;
  SUNStepper arg1 = (SUNStepper) 0 ;
  SUNStepper arg2 = (SUNStepper) 0 ;
  int64_t arg3 ;
  N_Vector arg4 = (N_Vector) 0 ;
  sunrealtype arg5 ;
  SUNAdjointCheckpointScheme arg6 = (SUNAdjointCheckpointScheme) 0 ;
  SUNContext arg7 = (SUNContext) 0 ;
  SUNAdjointStepper *arg8 = (SUNAdjointStepper *) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNStepper)(farg1);
  arg2 = (SUNStepper)(farg2);
  arg3 = (int64_t)(*farg3);
  arg4 = (N_Vector)(farg4);
  arg5 = (sunrealtype)(*farg5);
  arg6 = (SUNAdjointCheckpointScheme)(farg6);
  arg7 = (SUNContext)(farg7);
  arg8 = (SUNAdjointStepper *)(farg8);
  result = (SUNErrCode)SUNAdjointStepper_Create(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_ReInit(void *farg1, N_Vector farg2, double const *farg3, N_Vector farg4, double const *farg5) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  N_Vector arg2 = (N_Vector) 0 ;
  sunrealtype arg3 ;
  N_Vector arg4 = (N_Vector) 0 ;
  sunrealtype arg5 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (N_Vector)(farg2);
  arg3 = (sunrealtype)(*farg3);
  arg4 = (N_Vector)(farg4);
  arg5 = (sunrealtype)(*farg5);
  result = (SUNErrCode)SUNAdjointStepper_ReInit(arg1,arg2,arg3,arg4,arg5);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_Evolve(void *farg1, double const *farg2, N_Vector farg3, double *farg4, int *farg5) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  sunrealtype arg2 ;
  N_Vector arg3 = (N_Vector) 0 ;
  sunrealtype *arg4 = (sunrealtype *) 0 ;
  int *arg5 = (int *) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (sunrealtype)(*farg2);
  arg3 = (N_Vector)(farg3);
  arg4 = (sunrealtype *)(farg4);
  arg5 = (int *)(farg5);
  result = (SUNErrCode)SUNAdjointStepper_Evolve(arg1,arg2,arg3,arg4,arg5);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_OneStep(void *farg1, double const *farg2, N_Vector farg3, double *farg4, int *farg5) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  sunrealtype arg2 ;
  N_Vector arg3 = (N_Vector) 0 ;
  sunrealtype *arg4 = (sunrealtype *) 0 ;
  int *arg5 = (int *) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (sunrealtype)(*farg2);
  arg3 = (N_Vector)(farg3);
  arg4 = (sunrealtype *)(farg4);
  arg5 = (int *)(farg5);
  result = (SUNErrCode)SUNAdjointStepper_OneStep(arg1,arg2,arg3,arg4,arg5);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_RecomputeFwd(void *farg1, int64_t const *farg2, double const *farg3, double const *farg4, N_Vector farg5) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  int64_t arg2 ;
  sunrealtype arg3 ;
  sunrealtype arg4 ;
  N_Vector arg5 = (N_Vector) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (int64_t)(*farg2);
  arg3 = (sunrealtype)(*farg3);
  arg4 = (sunrealtype)(*farg4);
  arg5 = (N_Vector)(farg5);
  result = (SUNErrCode)SUNAdjointStepper_RecomputeFwd(arg1,arg2,arg3,arg4,arg5);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_SetJacFn(void *farg1, SUNJacFn farg2, SUNMatrix farg3, SUNJacFn farg4, SUNMatrix farg5) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  SUNJacFn arg2 = (SUNJacFn) 0 ;
  SUNMatrix arg3 = (SUNMatrix) 0 ;
  SUNJacFn arg4 = (SUNJacFn) 0 ;
  SUNMatrix arg5 = (SUNMatrix) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (SUNJacFn)(farg2);
  arg3 = (SUNMatrix)(farg3);
  arg4 = (SUNJacFn)(farg4);
  arg5 = (SUNMatrix)(farg5);
  result = (SUNErrCode)SUNAdjointStepper_SetJacFn(arg1,arg2,arg3,arg4,arg5);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_SetJacTimesVecFn(void *farg1, SUNJacTimesFn farg2, SUNJacTimesFn farg3) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  SUNJacTimesFn arg2 = (SUNJacTimesFn) 0 ;
  SUNJacTimesFn arg3 = (SUNJacTimesFn) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (SUNJacTimesFn)(farg2);
  arg3 = (SUNJacTimesFn)(farg3);
  result = (SUNErrCode)SUNAdjointStepper_SetJacTimesVecFn(arg1,arg2,arg3);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_SetVecTimesJacFn(void *farg1, SUNJacTimesFn farg2, SUNJacTimesFn farg3) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  SUNJacTimesFn arg2 = (SUNJacTimesFn) 0 ;
  SUNJacTimesFn arg3 = (SUNJacTimesFn) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (SUNJacTimesFn)(farg2);
  arg3 = (SUNJacTimesFn)(farg3);
  result = (SUNErrCode)SUNAdjointStepper_SetVecTimesJacFn(arg1,arg2,arg3);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_SetUserData(void *farg1, void *farg2) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  void *arg2 = (void *) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (void *)(farg2);
  result = (SUNErrCode)SUNAdjointStepper_SetUserData(arg1,arg2);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_PrintAllStats(void *farg1, void *farg2, int const *farg3) {
  int fresult ;
  SUNAdjointStepper arg1 = (SUNAdjointStepper) 0 ;
  FILE *arg2 = (FILE *) 0 ;
  SUNOutputFormat arg3 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper)(farg1);
  arg2 = (FILE *)(farg2);
  arg3 = (SUNOutputFormat)(*farg3);
  result = (SUNErrCode)SUNAdjointStepper_PrintAllStats(arg1,arg2,arg3);
  fresult = (SUNErrCode)(result);
  return fresult;
}


SWIGEXPORT int _wrap_FSUNAdjointStepper_Destroy(void *farg1) {
  int fresult ;
  SUNAdjointStepper *arg1 = (SUNAdjointStepper *) 0 ;
  SUNErrCode result;
  
  arg1 = (SUNAdjointStepper *)(farg1);
  result = (SUNErrCode)SUNAdjointStepper_Destroy(arg1);
  fresult = (SUNErrCode)(result);
  return fresult;
}



