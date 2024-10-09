! This file was automatically generated by SWIG (http://www.swig.org).
! Version 4.0.0
!
! Do not make changes to this file unless you know what you are doing--modify
! the SWIG interface file instead.

! ---------------------------------------------------------------
! Programmer(s): Auto-generated by swig.
! ---------------------------------------------------------------
! SUNDIALS Copyright Start
! Copyright (c) 2002-2024, Lawrence Livermore National Security
! and Southern Methodist University.
! All rights reserved.
!
! See the top-level LICENSE and NOTICE files for details.
!
! SPDX-License-Identifier: BSD-3-Clause
! SUNDIALS Copyright End
! ---------------------------------------------------------------

module fsunadjointcheckpointscheme_basic_mod
 use, intrinsic :: ISO_C_BINDING
 use fsundials_core_mod
 use fsunadjointcheckpointscheme_mod
 use fsundials_core_mod
 implicit none
 private

 ! DECLARATION CONSTRUCTS

 integer, parameter :: swig_cmem_own_bit = 0
 integer, parameter :: swig_cmem_rvalue_bit = 1
 integer, parameter :: swig_cmem_const_bit = 2
 type, bind(C) :: SwigClassWrapper
  type(C_PTR), public :: cptr = C_NULL_PTR
  integer(C_INT), public :: cmemflags = 0
 end type
 type, public :: SWIGTYPE_p_SUNMemoryHelper
  type(SwigClassWrapper), public :: swigdata
 end type
 public :: FSUNAdjointCheckpointScheme_Create_Basic
 public :: FSUNAdjointCheckpointScheme_ShouldWeSave_Basic
 public :: FSUNAdjointCheckpointScheme_InsertVector_Basic
 public :: FSUNAdjointCheckpointScheme_ShouldWeDelete_Basic
 public :: FSUNAdjointCheckpointScheme_RemoveVector_Basic
 public :: FSUNAdjointCheckpointScheme_LoadVector_Basic
 public :: FSUNAdjointCheckpointScheme_Destroy_Basic
 public :: FSUNAdjointCheckpointScheme_EnableDense_Basic

! WRAPPER DECLARATIONS
interface
function swigc_FSUNAdjointCheckpointScheme_Create_Basic(farg1, farg2, farg3, farg4, farg5, farg6, farg7, farg8) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_Create_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
import :: swigclasswrapper
integer(C_INT), intent(in) :: farg1
type(SwigClassWrapper) :: farg2
integer(C_INT64_T), intent(in) :: farg3
integer(C_INT64_T), intent(in) :: farg4
integer(C_INT), intent(in) :: farg5
integer(C_INT), intent(in) :: farg6
type(C_PTR), value :: farg7
type(C_PTR), value :: farg8
integer(C_INT) :: fresult
end function

function swigc_FSUNAdjointCheckpointScheme_ShouldWeSave_Basic(farg1, farg2, farg3, farg4, farg5) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_ShouldWeSave_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT64_T), intent(in) :: farg2
integer(C_INT64_T), intent(in) :: farg3
real(C_DOUBLE), intent(in) :: farg4
type(C_PTR), value :: farg5
integer(C_INT) :: fresult
end function

function swigc_FSUNAdjointCheckpointScheme_InsertVector_Basic(farg1, farg2, farg3, farg4, farg5) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_InsertVector_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT64_T), intent(in) :: farg2
integer(C_INT64_T), intent(in) :: farg3
real(C_DOUBLE), intent(in) :: farg4
type(C_PTR), value :: farg5
integer(C_INT) :: fresult
end function

function swigc_FSUNAdjointCheckpointScheme_ShouldWeDelete_Basic(farg1, farg2, farg3, farg4) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_ShouldWeDelete_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT64_T), intent(in) :: farg2
integer(C_INT64_T), intent(in) :: farg3
type(C_PTR), value :: farg4
integer(C_INT) :: fresult
end function

function swigc_FSUNAdjointCheckpointScheme_RemoveVector_Basic(farg1, farg2, farg3, farg4) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_RemoveVector_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT64_T), intent(in) :: farg2
integer(C_INT64_T), intent(in) :: farg3
type(C_PTR), value :: farg4
integer(C_INT) :: fresult
end function

function swigc_FSUNAdjointCheckpointScheme_LoadVector_Basic(farg1, farg2, farg3, farg4, farg5, farg6) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_LoadVector_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT64_T), intent(in) :: farg2
integer(C_INT64_T), intent(in) :: farg3
integer(C_INT), intent(in) :: farg4
type(C_PTR), value :: farg5
type(C_PTR), value :: farg6
integer(C_INT) :: fresult
end function

function swigc_FSUNAdjointCheckpointScheme_Destroy_Basic(farg1) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_Destroy_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT) :: fresult
end function

function swigc_FSUNAdjointCheckpointScheme_EnableDense_Basic(farg1, farg2) &
bind(C, name="_wrap_FSUNAdjointCheckpointScheme_EnableDense_Basic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT), intent(in) :: farg2
integer(C_INT) :: fresult
end function

end interface


contains
 ! MODULE SUBPROGRAMS
function FSUNAdjointCheckpointScheme_Create_Basic(io_mode, mem_helper, interval, estimate, save_stages, keep, sunctx, &
  check_scheme_ptr) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
integer(SUNDataIOMode), intent(in) :: io_mode
type(SWIGTYPE_p_SUNMemoryHelper), intent(in) :: mem_helper
integer(C_INT64_T), intent(in) :: interval
integer(C_INT64_T), intent(in) :: estimate
integer(C_INT), intent(in) :: save_stages
integer(C_INT), intent(in) :: keep
type(C_PTR) :: sunctx
type(C_PTR), target, intent(inout) :: check_scheme_ptr
integer(C_INT) :: fresult 
integer(C_INT) :: farg1 
type(SwigClassWrapper) :: farg2 
integer(C_INT64_T) :: farg3 
integer(C_INT64_T) :: farg4 
integer(C_INT) :: farg5 
integer(C_INT) :: farg6 
type(C_PTR) :: farg7 
type(C_PTR) :: farg8 

farg1 = io_mode
farg2 = mem_helper%swigdata
farg3 = interval
farg4 = estimate
farg5 = save_stages
farg6 = keep
farg7 = sunctx
farg8 = c_loc(check_scheme_ptr)
fresult = swigc_FSUNAdjointCheckpointScheme_Create_Basic(farg1, farg2, farg3, farg4, farg5, farg6, farg7, farg8)
swig_result = fresult
end function

function FSUNAdjointCheckpointScheme_ShouldWeSave_Basic(check_scheme, step_num, stage_num, t, yes_or_no) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNAdjointCheckpointScheme), target, intent(inout) :: check_scheme
integer(C_INT64_T), intent(in) :: step_num
integer(C_INT64_T), intent(in) :: stage_num
real(C_DOUBLE), intent(in) :: t
integer(C_INT), dimension(*), target, intent(inout) :: yes_or_no
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
integer(C_INT64_T) :: farg2 
integer(C_INT64_T) :: farg3 
real(C_DOUBLE) :: farg4 
type(C_PTR) :: farg5 

farg1 = c_loc(check_scheme)
farg2 = step_num
farg3 = stage_num
farg4 = t
farg5 = c_loc(yes_or_no(1))
fresult = swigc_FSUNAdjointCheckpointScheme_ShouldWeSave_Basic(farg1, farg2, farg3, farg4, farg5)
swig_result = fresult
end function

function FSUNAdjointCheckpointScheme_InsertVector_Basic(check_scheme, step_num, stage_num, t, state) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNAdjointCheckpointScheme), target, intent(inout) :: check_scheme
integer(C_INT64_T), intent(in) :: step_num
integer(C_INT64_T), intent(in) :: stage_num
real(C_DOUBLE), intent(in) :: t
type(N_Vector), target, intent(inout) :: state
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
integer(C_INT64_T) :: farg2 
integer(C_INT64_T) :: farg3 
real(C_DOUBLE) :: farg4 
type(C_PTR) :: farg5 

farg1 = c_loc(check_scheme)
farg2 = step_num
farg3 = stage_num
farg4 = t
farg5 = c_loc(state)
fresult = swigc_FSUNAdjointCheckpointScheme_InsertVector_Basic(farg1, farg2, farg3, farg4, farg5)
swig_result = fresult
end function

function FSUNAdjointCheckpointScheme_ShouldWeDelete_Basic(check_scheme, step_num, stage_num, yes_or_no) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNAdjointCheckpointScheme), target, intent(inout) :: check_scheme
integer(C_INT64_T), intent(in) :: step_num
integer(C_INT64_T), intent(in) :: stage_num
integer(C_INT), dimension(*), target, intent(inout) :: yes_or_no
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
integer(C_INT64_T) :: farg2 
integer(C_INT64_T) :: farg3 
type(C_PTR) :: farg4 

farg1 = c_loc(check_scheme)
farg2 = step_num
farg3 = stage_num
farg4 = c_loc(yes_or_no(1))
fresult = swigc_FSUNAdjointCheckpointScheme_ShouldWeDelete_Basic(farg1, farg2, farg3, farg4)
swig_result = fresult
end function

function FSUNAdjointCheckpointScheme_RemoveVector_Basic(check_scheme, step_num, stage_num, out) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNAdjointCheckpointScheme), target, intent(inout) :: check_scheme
integer(C_INT64_T), intent(in) :: step_num
integer(C_INT64_T), intent(in) :: stage_num
type(C_PTR) :: out
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
integer(C_INT64_T) :: farg2 
integer(C_INT64_T) :: farg3 
type(C_PTR) :: farg4 

farg1 = c_loc(check_scheme)
farg2 = step_num
farg3 = stage_num
farg4 = out
fresult = swigc_FSUNAdjointCheckpointScheme_RemoveVector_Basic(farg1, farg2, farg3, farg4)
swig_result = fresult
end function

function FSUNAdjointCheckpointScheme_LoadVector_Basic(check_scheme, step_num, stage_num, peek, out, tout) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNAdjointCheckpointScheme), target, intent(inout) :: check_scheme
integer(C_INT64_T), intent(in) :: step_num
integer(C_INT64_T), intent(in) :: stage_num
integer(C_INT), intent(in) :: peek
type(C_PTR) :: out
real(C_DOUBLE), dimension(*), target, intent(inout) :: tout
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
integer(C_INT64_T) :: farg2 
integer(C_INT64_T) :: farg3 
integer(C_INT) :: farg4 
type(C_PTR) :: farg5 
type(C_PTR) :: farg6 

farg1 = c_loc(check_scheme)
farg2 = step_num
farg3 = stage_num
farg4 = peek
farg5 = out
farg6 = c_loc(tout(1))
fresult = swigc_FSUNAdjointCheckpointScheme_LoadVector_Basic(farg1, farg2, farg3, farg4, farg5, farg6)
swig_result = fresult
end function

function FSUNAdjointCheckpointScheme_Destroy_Basic(check_scheme_ptr) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(C_PTR), target, intent(inout) :: check_scheme_ptr
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(check_scheme_ptr)
fresult = swigc_FSUNAdjointCheckpointScheme_Destroy_Basic(farg1)
swig_result = fresult
end function

function FSUNAdjointCheckpointScheme_EnableDense_Basic(check_scheme, on_or_off) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNAdjointCheckpointScheme), target, intent(inout) :: check_scheme
integer(C_INT), intent(in) :: on_or_off
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
integer(C_INT) :: farg2 

farg1 = c_loc(check_scheme)
farg2 = on_or_off
fresult = swigc_FSUNAdjointCheckpointScheme_EnableDense_Basic(farg1, farg2)
swig_result = fresult
end function


end module
