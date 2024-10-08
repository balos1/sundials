# ---------------------------------------------------------------
# Programmer(s): Cody J. Balos @ LLNL
# ---------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2024, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# ---------------------------------------------------------------
# CMakeLists.txt file for the F2003 Newton SUNNonlinearSolver
# object library
# ---------------------------------------------------------------

sundials_add_f2003_library(
  sundials_fsunnonlinsolnewton_mod
  SOURCES fsunnonlinsol_newton_mod.f90 fsunnonlinsol_newton_mod.c
  LINK_LIBRARIES PUBLIC sundials_fcore_mod
  OUTPUT_NAME sundials_fsunnonlinsolnewton_mod
  VERSION ${sunnonlinsollib_VERSION}
  SOVERSION ${sunnonlinsollib_SOVERSION})

message(STATUS "Added SUNNONLINSOL_NEWTON F2003 interface")
