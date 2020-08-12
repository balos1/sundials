# ---------------------------------------------------------------------------
# Programmer(s): Cody J. Balos @ LLNL
# ---------------------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2020, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# ---------------------------------------------------------------------------
# Provides the macro:
#
#   SUNDIALS_OPTION(<variable> <type> <docstring> <default value>
#                   [SHOW_IF dependencies]
#                   [DEPENDS_ON dependencies]
#                   [XSDK_NAME name]
#                   [XSDK_HIDE | ADVANCED])
#
# Within CMake creates a cache variable <variable> and sets it to the value
# <default value> if <variable> is not yet defined. <type> may be any of the
# types valid for CMake's set command (FILEPATH, PATH, STRING, BOOL, INTERNAL).
# <docstring> is a description of the <variable>.
#
# The SHOW_IF option can be used to provide variables which must evaluate
# to true in order to make <variable> a CACHE variable. If not all dependencies
# are met <variable> is made an INTERNAL variable.
#
# The DEPENDS_ON option can be used to provide variable which must evaluate
# to true in order to set <variable>. If the DEPENDS_ON dependencies do not
# all evaluate to true, then <variable> is set to "".
#
# The XSDK_NAME option can be used to set the xSDK compliant name of the
# variable. If the XSDK_NAME variable is provided, or USE_XSDK_DEFAULTS=ON,
# then SUNDIALS_OPTION creates a second variable <name> that is shadowed
# by <variable>.
#
# The XSDK_HIDE option can be used to hide the variable when USE_XSDK_DEFAULTS=ON
# i.e., it makes <variable> an INTERNAL variable in xsdk mode.
#
# The ADVANCED option can be used to make <variable> an advanced CMake option.
# ---------------------------------------------------------------------------


macro(sundials_option NAME TYPE DOCSTR DEFAULT_VALUE)
  set(options ADVANCED XSDK_HIDE)           # macro options
  set(oneValueArgs XSDK_NAME)               # macro keyword inputs followed by a single value
  set(multiValueArgs SHOW_IF DEPENDS_ON)    # macro keyword inputs followed by multiple values

  # parse inputs and create variables sundials_option_<keyword>
  cmake_parse_arguments(sundials_option "${options}" "${oneValueArgs}"
    "${multiValueArgs}" ${ARGN} )

  # ensure conflicting options were not used
  if(sundials_option_XSDK_HIDE AND sundials_option_ADVANCED)
    print_error("Cannot use XSDK_HIDE and ADVANCED with sundials_option macro")
  endif()

  set(create_option TRUE)

  # check if dependencies for this option have been met
  set(all_show_if_dependencies_met TRUE)
  if(sundials_option_SHOW_IF)
      foreach(_dependency ${sundials_option_SHOW_IF})
        if(NOT ${_dependency})
          set(all_show_if_dependencies_met FALSE)
        endif()
    endforeach()
  endif()

  # check if dependencies for this option have been met
  set(all_depends_on_dependencies_met TRUE)
  if(sundials_option_DEPENDS_ON)
      foreach(_dependency ${sundials_option_DEPENDS_ON})
        if(NOT ${_dependency})
          set(all_depends_on_dependencies_met FALSE)
          list(APPEND depends_on_dependencies_not_met "${_dependency},")
        endif()
    endforeach()
  endif()

  if(create_option AND all_show_if_dependencies_met AND all_depends_on_dependencies_met)

    if(DEFINED ${NAME})
      # if the variable is already defined, keep its value
      set(${NAME} ${${NAME}} CACHE ${TYPE} ${DOCSTR} FORCE)
    else()
      # if the variable is not already defined, use the default value
      set(${NAME} ${DEFAULT_VALUE} CACHE ${TYPE} ${DOCSTR})
    endif()

    # handle xSDK variables
    if(sundials_option_XSDK_NAME)

      if(USE_XSDK_DEFAULTS)

        # xSDK mode is ON, use xSDK compliant variables
        if(DEFINED ${sundials_option_XSDK_NAME})
          # if the xSDK variable is already defined, keep its value
          set(${sundials_option_XSDK_NAME} ${${sundials_option_XSDK_NAME}}
            CACHE ${TYPE} ${DOCSTR})
        else()
          # if the xSDK variable is not already defined, use the default value
          set(${sundials_option_XSDK_NAME} ${DEFAULT_VALUE} CACHE ${TYPE} ${DOCSTR})
        endif()

        # replace the normal variable with the xSDK version
        set(${NAME} ${${sundials_option_XSDK_NAME}} CACHE ${TYPE} "${DOCSTR}" FORCE)
        message(STATUS "Replacing ${NAME} with ${sundials_option_XSDK_NAME}")
        mark_as_advanced(FORCE ${NAME})

      elseif(DEFINED ${sundials_option_XSDK_NAME})

        # xSDK mode in OFF, remove the xSDK variable and make sure the normal
        # variable is visible.
        unset(${sundials_option_XSDK_NAME} CACHE)
        mark_as_advanced(CLEAR ${NAME})

      endif()

    endif()

    # if in xSDK mode, hide the normal variable if necessary
    if(sundials_option_XSDK_HIDE AND USE_XSDK_DEFAULTS)
      set(${NAME} ${${NAME}} CACHE INTERNAL ${DOCSTR})
    endif()

    # make the option advanced if necessary
    if(sundials_option_ADVANCED)
      mark_as_advanced(FORCE ${NAME})
    endif()

  elseif(create_option AND NOT all_depends_on_dependencies_met)

    # don't set the normal variable unless all dependencies were met
    if(DEFINED ${NAME})
      if(${NAME})
        print_warning("The variable ${NAME} was set to ${${NAME}} but not all of its dependencies (${depends_on_dependencies_not_met}) evaluate to TRUE." "Setting ${NAME} to \"\".")
      endif()
      set(${NAME} "" CACHE INTERNAL ${DOCSTR} FORCE)
    endif()

    # don't set the xSDK variable unless all dependencies were met
    if(sundials_option_XSDK_NAME)
      if(DEFINED ${sundials_option_XSDK_NAME})
        if(${sundials_option_XSDK_NAME})
          print_warning("The variable ${sundials_option_XSDK_NAME} was set to ${${sundials_option_XSDK_NAME}} but not all of its dependencies (${depends_on_dependencies_not_met}) evaluate to TRUE." "Setting ${NAME} to \"\".")
        endif()
        set(${sundials_option_XSDK_NAME} "" CACHE INTERNAL ${DOCSTR} FORCE)
      endif()
    endif()

  elseif(create_option AND NOT all_show_if_dependencies_met)

    # hide the normal variable if not all dependencies were met
    if(DEFINED ${NAME})
      set(${NAME} ${${NAME}} CACHE INTERNAL ${DOCSTR} FORCE)
    endif()

    # hide the xSDK variable also if it exists
    if(sundials_option_XSDK_NAME)
      if(DEFINED ${sundials_option_XSDK_NAME})
        set(${sundials_option_XSDK_NAME} ${${sundials_option_XSDK_NAME}} CACHE INTERNAL ${DOCSTR} FORCE)
      endif()
    endif()

  endif()


  unset(create_option)
  unset(all_show_if_dependencies_met)
  unset(all_depends_on_dependencies_met)
  unset(depends_on_dependencies_not_met)

endmacro()
