# -----------------------------------------------------------------------------
# Programmer(s): Slaven Peles, and Cody J. Balos @ LLNL
# -----------------------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2020, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# -----------------------------------------------------------------------------
# Find module for Trilinos that uses the TrilinosConfig.cmake that is installed
# with Trilinos. The module will also create a Trilinos::Trilinos target.
# -----------------------------------------------------------------------------

# First try and find Trilinos using Trilinos_DIR only.
find_package(Trilinos
  NAMES Trilinos TRILINOS
  PATHS
    ${Trilinos_DIR}/lib/cmake/Trilinos
    ${Trilinos_DIR}
  NO_DEFAULT_PATH
  QUIET)

# If Trilinos_DIR was not provided, try and find Trilinos
# somewhere else unless using in the xSDK mode.
if (NOT (Trilinos_FOUND OR USE_XSDK_DEFAULTS))
  find_package(Trilinos
    NAMES Trilinos TRILINOS
    PATHS
      ${Trilinos_DIR}/lib/cmake/Trilinos
      ${Trilinos_DIR}
    QUIET)
endif()

# set package variables including Trilinos_FOUND
find_package_handle_standard_args(Trilinos
  REQUIRED_VARS
    Trilinos_LIBRARIES      # defined in TrilinosConfig.cmake
    Trilinos_INCLUDE_DIRS   # defined in TrilinosConfig.cmake
  )

# Create Trilinos target
if(Trilinos_FOUND AND NOT TARGET Trilinos::Trilinos)
  add_library(Trilinos::Trilinos IMPORTED INTERFACE)
  set_target_properties(Trilinos::Trilinos PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Trilinos_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${Trilinos_LIBRARIES}")
endif()