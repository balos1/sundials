# ---------------------------------------------------------------
# Programmer(s): Cody J. Balos @ LLNL
# ---------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2022, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# ---------------------------------------------------------------
# CMake macro for adding libraries.
# Wraps the add_library command for sundials specific needs.
# ---------------------------------------------------------------


# The macro:
#
#   SUNDIALS_ADD_LIBRARY(<target>
#                        SOURCES source1 source2 ...
#                        [HEADERS header1 header2 ...]
#                        [OBJECT_LIBRARIES objlib1 objlib2 ...]
#                        [LINK_LIBRARIES <PRIVATE|PUBLIC|INTERFACE> <item>...
#                                       [<PRIVATE|PUBLIC|INTERFACE> <item>...] ]
#                        [INCLUDE_DIRECTORIES <PRIVATE|PUBLIC|INTERFACE> <item>...
#                                            [<PRIVATE|PUBLIC|INTERFACE> <item>...] ]
#                        [COMPILE_DEFINITIONS <PRIVATE|PUBLIC|INTERFACE> <item>...
#                                            [<PRIVATE|PUBLIC|INTERFACE> <item>...] ]
#                        [COMPILE_OPTIONS <PRIVATE|PUBLIC|INTERFACE> <item>...
#                                        [<PRIVATE|PUBLIC|INTERFACE> <item>...] ]
#                        [COMPILE_FEATURES <PRIVATE|PUBLIC|INTERFACE> <item>...
#                                         [<PRIVATE|PUBLIC|INTERFACE> <item>...] ]
#                        [PROPERTIES <PROPERTY> <value> ... [<PROPERTY> <value> ...] ]
#                        [INCLUDE_SUBDIR]
#                        [OUTPUT_NAME name]
#                        [VERSION version]
#                        [SOVERSION version]
#                        [STATIC_ONLY | SHARED_ONLY]
#                        [OBJECT_LIB_ONLY])
#
# adds libraries to be built from the source files listed in the command
# invocation. It is a convenient wrapper of the CMake add_library command that
# is specific to our usage of add_library in SUNDIALS.
#
# By default, the macro uses the CMake add_library command to create the
# targets:
#   - <target>${_SHARED_LIB_SUFFIX} (will be a shared library)
#   - <target>${_STATIC_LIB_SUFFIX} (will be a static library)
#   - <target>_obj${_SHARED_LIB_SUFFIX} (an object library that is used to
#     create <target>${_SHARED_LIB_SUFFIX})
#   - <target>_obj${_STATIC_LIB_SUFFIX} (an object library that is used to
#     create <target>${_STATIC_LIB_SUFFIX})
#   - <target> (an alias to the shared library, if enabled, otherwise an
#     alias to the static library)
#
# The SOURCES input is a list of source files used to create the library.
#
# The option HEADERS is a list of header files to install with the library. The
# option INCLUDE_SUBDIR can be used to specify a subdirectory in which the
# header files should be installed.
#
# The option OBJECT_LIBRARIES is a list of object library to include when
# building the target. This is similar to adding more source files, but its done
# using the already compiled objects.
#
# The option LINK_LIBRARIES is a list that is passed to target_link_libraries on
# all the targets generated by the macro. See the CMake target_link_libraries
# documentation for more info.
#
# The option INCLUDE_DIRECTORIES is a list that is passed to
# target_include_directories on all the targets generated by the macro. The
# {{libtype}} keyword will be replaced with the appropriate library type string
# (currently this only used by the sundials_add_f2003_library macro). See the
# CMake target_include_directories documentation for more info.
#
# The option COMPILE_DEFINITIONS is a list that is passed to
# target_compile_definitions on all the targets generated by the macro. See the
# CMake target_compile_definitions documentation for more info.
#
# The option COMPILE_OPTIONS is a list that is passed to target_compile_options
# on all the targets generated by the macro. See the CMake
# target_compile_options documentation for more info.
#
# The option COMPILE_FEATURES is a list that is passed to
# target_compile_features on all the targets generated by the macro. See the
# CMake target_compile_features documentation for more info.
#
# The option PROPERTIES is a list that is passed to set_target_properties on all
# the targets generated by the macro. See the CMake set_target_properties
# documentation for more info.
#
# The option INCLUDE_SUBDIR sets a subdirectory in which the header files listed
# in HEADERS should be installed.
#
# The option OUTPUT_NAME sets the actual name of the produced library.
#
# The option VERSION sets the library version number on the shared target.
#
# The option SOVERSION sets the library soversion number on the shared target.
#
# The option STATIC_ONLY will cause the macro to only create the static library
# targets.
#
# The option SHARED_ONLY will cause the macro to only create the shared library
# targets.
#
# The option OBJECT_LIB_ONLY will cause the macro to only create the object
# library targets.
macro(sundials_add_library target)

  set(options STATIC_ONLY SHARED_ONLY OBJECT_LIB_ONLY)
  set(oneValueArgs INCLUDE_SUBDIR OUTPUT_NAME VERSION SOVERSION)
  set(multiValueArgs SOURCES HEADERS OBJECT_LIBRARIES LINK_LIBRARIES
                     INCLUDE_DIRECTORIES COMPILE_DEFINITIONS COMPILE_OPTIONS
                     COMPILE_FEATURES PROPERTIES)

  # parse keyword arguments/options
  cmake_parse_arguments(sundials_add_library
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # library types to create
  set(_libtypes "")
  if(BUILD_STATIC_LIBS AND (NOT sundials_add_library_SHARED_ONLY))
    set(_libtypes "STATIC")
  endif()

  if(BUILD_SHARED_LIBS AND (NOT sundials_add_library_STATIC_ONLY))
    set(_libtypes "${_libtypes};SHARED")
  endif()

  foreach(_libtype ${_libtypes})

    # library suffix
    if(${_libtype} MATCHES "STATIC")
      set(_lib_suffix "${_STATIC_LIB_SUFFIX}")
    else()
      set(_lib_suffix "${_SHARED_LIB_SUFFIX}")
    endif()

    # source files
    set(sources ${sundials_add_library_SOURCES})

    # give the object library its own target name
    set(obj_target ${target}_obj${_lib_suffix})

    # --------------------------------------------------------------------------
    # Create Object library
    # --------------------------------------------------------------------------

    # create the target for the object library
    add_library(${obj_target} OBJECT ${sources})

    # add all object libraries to object library
    if(sundials_add_library_OBJECT_LIBRARIES)
      target_link_libraries(${obj_target}
        PRIVATE ${sundials_add_library_OBJECT_LIBRARIES})
    endif()

    # add all link libraries to object library
    if(USE_GENERIC_MATH)
      target_link_libraries(${obj_target} PRIVATE m)
    endif()
    if(sundials_add_library_LINK_LIBRARIES)
      if(${_libtype} MATCHES "STATIC")
        append_static_suffix(sundials_add_library_LINK_LIBRARIES _all_libs)
      else()
        set(_all_libs ${sundials_add_library_LINK_LIBRARIES})
      endif()
      target_link_libraries(${obj_target} ${_all_libs})
    endif()

    # add includes to object library
    target_include_directories(${obj_target}
      PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${SUNDIALS_SOURCE_DIR}/src/sundials>
    )
    if(sundials_add_library_INCLUDE_DIRECTORIES)
      string(REPLACE "{{libtype}}" "${_libtype}" _includes "${sundials_add_library_INCLUDE_DIRECTORIES}")
      target_include_directories(${obj_target} ${_includes})
    endif()

    # add compile definitions to object library for SUNDIALS_EXPORT
    if(${_libtype} MATCHES "STATIC")
      target_compile_definitions(${obj_target} PRIVATE SUNDIALS_STATIC_DEFINE)
    else()
      target_compile_definitions(${obj_target} PRIVATE sundials_generic_EXPORTS)
    endif()

    # add all other compile definitions to object library
    if(sundials_add_library_COMPILE_DEFINITIONS)
      target_compile_definitions(${obj_target} ${sundials_add_library_COMPILE_DEFINITIONS})
    endif()

    # add compile options to object library
    if(sundials_add_library_COMPILE_OPTIONS)
      target_compile_options(${obj_target} ${sundials_add_library_COMPILE_OPTIONS})
    endif()

    # add compile features
    if(sundials_add_library_COMPILE_FEATURES)
      target_compile_features(${obj_target} ${sundials_add_library_COMPILE_FEATURES})
    endif()

    # object files going into shared libs need PIC code
    set_target_properties(${obj_target} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

    # set any other properties
    if(sundials_add_library_PROPERTIES)
      string(REPLACE "{{libtype}}" "${_libtype}" _properties "${sundials_add_library_PROPERTIES}")
      set_target_properties(${obj_target} PROPERTIES ${_properties})
    endif()

    # --------------------------------------------------------------------------
    # Create Library
    # --------------------------------------------------------------------------

    # now create the real library
    if(NOT sundials_add_library_OBJECT_LIB_ONLY)

      # set target name
      set(_actual_target_name ${target}${_lib_suffix})

      add_library(${_actual_target_name} ${_libtype} $<TARGET_OBJECTS:${obj_target}>)

      # add any object library dependencies
      if(sundials_add_library_OBJECT_LIBRARIES)
        if(${_libtype} MATCHES "STATIC")
          append_static_suffix(sundials_add_library_OBJECT_LIBRARIES _all_objs)
        else()
          set(_all_objs ${sundials_add_library_OBJECT_LIBRARIES})
        endif()
        foreach(_tmp ${_all_objs})
          # We use target_sources since target_link_libraries does not work
          # as expected with CMake 3.12 (see CMake issues 18090 and 18692).
          # TODO(DJG): Update whenever we require CMake 3.14 or 3.21
          target_sources(${_actual_target_name} PRIVATE $<TARGET_OBJECTS:${_tmp}>)
        endforeach()
      endif()

      # add all link libraries
      if(USE_GENERIC_MATH)
        target_link_libraries(${_actual_target_name} PRIVATE m)
      endif()
      if(sundials_add_library_LINK_LIBRARIES)
        target_link_libraries(${_actual_target_name} ${sundials_add_library_LINK_LIBRARIES})
      endif()

      if(SUNDIALS_BUILD_WITH_PROFILING OR SUNDIALS_LOGGING_ENABLE_MPI)
        if(ENABLE_MPI AND MPI_C_FOUND)
          # Workaround issues with sundials_generic object library dependency on
          # MPI not getting propagated when building examples.
          # Workaround bug in CMake < 3.17.3 when using MPI::MPI_C and CUDA
          target_include_directories(${_actual_target_name} PUBLIC ${MPI_C_INCLUDE_DIRS})
          target_link_libraries(${_actual_target_name} PUBLIC ${MPI_C_LIBRARIES})
        endif()
      endif()

      # add common includes
      # Building: public, config/export generated, and shared private headers
      # Installing: installed include directory
      target_include_directories(${_actual_target_name} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${SUNDIALS_SOURCE_DIR}/src/sundials>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

      # add all other includes
      if(sundials_add_library_INCLUDE_DIRECTORIES)
        string(REPLACE "{{libtype}}" "${_libtype}" _includes "${sundials_add_library_INCLUDE_DIRECTORIES}")
        target_include_directories(${_actual_target_name} ${_includes})
      endif()

      # add compile definitions for SUNDIALS_EXPORT
      if(${_libtype} MATCHES "STATIC")
        target_compile_definitions(${_actual_target_name} PRIVATE SUNDIALS_STATIC_DEFINE)
      else()
        target_compile_definitions(${obj_target} PRIVATE sundials_generic_EXPORTS)
      endif()

      # add all other compile definitions
      if(sundials_add_library_COMPILE_DEFINITIONS)
        target_compile_definitions(${_actual_target_name} ${sundials_add_library_COMPILE_DEFINITIONS})
      endif()

      # add all compile options
      if(sundials_add_library_COMPILE_OPTIONS)
        target_compile_options(${_actual_target_name} ${sundials_add_library_COMPILE_OPTIONS})
      endif()

      # add compile features
      if(sundials_add_library_COMPILE_FEATURES)
        target_compile_features(${_actual_target_name} ${sundials_add_library_COMPILE_FEATURES})
      endif()

      # exported targets are in the SUNDIALS:: namespace, so we remove the sundials_ prefix from the exported name
      string(REPLACE "sundials_" "" _export_name "${_actual_target_name}")
      set_target_properties(${_actual_target_name} PROPERTIES EXPORT_NAME ${_export_name})

      # create an alias to match the exported target name, this way another projects can use it with either find_package() or add_subdirectory()
      add_library(SUNDIALS::${_export_name} ALIAS ${_actual_target_name})

      # set the correct output name
      if(sundials_add_library_OUTPUT_NAME)
        set_target_properties(${_actual_target_name} PROPERTIES
          OUTPUT_NAME ${sundials_add_library_OUTPUT_NAME}
          CLEAN_DIRECT_OUTPUT 1
        )
      else()
        set_target_properties(${_actual_target_name} PROPERTIES
          OUTPUT_NAME ${target}
          CLEAN_DIRECT_OUTPUT 1
        )
      endif()

      # set the library versions
      if(sundials_add_library_VERSION)
        set_target_properties(${_actual_target_name} PROPERTIES
          VERSION ${sundials_add_library_VERSION}
        )
      endif()
      if(sundials_add_library_SOVERSION)
        set_target_properties(${_actual_target_name} PROPERTIES
          SOVERSION ${sundials_add_library_SOVERSION}
        )
      endif()

      # set any other properties
      if(sundials_add_library_PROPERTIES)
        string(REPLACE "{{libtype}}" "${_libtype}" _properties "${sundials_add_library_PROPERTIES}")
        set_target_properties(${_actual_target_name} PROPERTIES ${_properties})
      endif()

      # install phase
      install(TARGETS ${_actual_target_name} DESTINATION ${CMAKE_INSTALL_LIBDIR} EXPORT sundials-targets)

    endif()

  endforeach()

  # --------------------------------------------------------------------------
  # Install header files
  # --------------------------------------------------------------------------

  if(sundials_add_library_HEADERS)
    install(FILES ${sundials_add_library_HEADERS}
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${sundials_add_library_INCLUDE_SUBDIR}")
  endif()

  # --------------------------------------------------------------------------
  # Aliases to real library names without the suffixes.
  # --------------------------------------------------------------------------

  if(TARGET ${target}_obj${_SHARED_LIB_SUFFIX})
    add_library(${target}_obj ALIAS ${target}_obj${_SHARED_LIB_SUFFIX})
    if(NOT sundials_add_library_OBJECT_LIB_ONLY)
      add_library(${target} ALIAS ${target}${_SHARED_LIB_SUFFIX})
      set(_SUNDIALS_ALIAS_TARGETS "${target}->${target}${_SHARED_LIB_SUFFIX};${_SUNDIALS_ALIAS_TARGETS}" CACHE INTERNAL "" FORCE)

      # Namespaced alias for using build directory directly
      string(REPLACE "sundials_" "" _export_name "${target}")
      add_library(SUNDIALS::${_export_name} ALIAS ${target}${_SHARED_LIB_SUFFIX})
    endif()
  else()
    add_library(${target}_obj ALIAS ${target}_obj${_STATIC_LIB_SUFFIX})
    if(NOT sundials_add_library_OBJECT_LIB_ONLY)
      add_library(${target} ALIAS ${target}${_STATIC_LIB_SUFFIX})
      set(_SUNDIALS_ALIAS_TARGETS "${target}->${target}${_STATIC_LIB_SUFFIX};${_SUNDIALS_ALIAS_TARGETS}" CACHE INTERNAL "" FORCE)

      # Namespaced alias for using build directory directly
      string(REPLACE "sundials_" "" _export_name "${target}")
      add_library(SUNDIALS::${_export_name} ALIAS ${target}${_STATIC_LIB_SUFFIX})
    endif()
  endif()

  # --------------------------------------------------------------------------
  # List of installed SUNDIALS components
  # --------------------------------------------------------------------------

  if(NOT sundials_add_library_OBJECT_LIB_ONLY)
    string(REPLACE "sundials_" "" _comp_name "${target}")
    set(_SUNDIALS_INSTALLED_COMPONENTS "${_comp_name};${_SUNDIALS_INSTALLED_COMPONENTS}" CACHE INTERNAL "" FORCE)
  endif()

endmacro(sundials_add_library)


macro(sundials_add_f2003_library target)

  set(options )
  set(oneValueArgs OUTPUT_NAME VERSION SOVERSION)
  set(multiValueArgs SOURCES OBJECT_LIBRARIES LINK_LIBRARIES INCLUDE_DIRECTORIES
                     COMPILE_DEFINITIONS COMPILE_OPTIONS PROPERTIES)

  # parse keyword arguments/options
  cmake_parse_arguments(sundials_add_f2003_library
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # set target properties and target dependencies so that includes
  # and links get passed on when this target is used
  if(CMAKE_Fortran_MODULE_DIRECTORY)
    set(_includes
      PUBLIC
        $<BUILD_INTERFACE:${CMAKE_Fortran_MODULE_DIRECTORY}_{{libtype}}>
        $<INSTALL_INTERFACE:${Fortran_INSTALL_MODDIR}>
    )
    set(_properties PROPERTIES Fortran_MODULE_DIRECTORY "${CMAKE_Fortran_MODULE_DIRECTORY}_{{libtype}}")
  else()
    set(_includes
      PUBLIC
        $<BUILD_INTERFACE:${CMAKE_Fortran_MODULE_DIRECTORY}_{{libtype}}>
        $<INSTALL_INTERFACE:${Fortran_INSTALL_MODDIR}>
    )
    set(_properties PROPERTIES Fortran_MODULE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir")
  endif()

  # get the name of the C library which the fortran library interfaces to
  string(REPLACE "sundials_f" "sundials_" _clib_name "${target}")
  string(REPLACE "_mod" "" _clib_name "${_clib_name}")

  # If SundialsSetupFortran.cmake did not set CMAKE_Fortran_PREPROCESS to ON,
  # then add a compiler flag to preprocess Fortran code.
  if(CMAKE_VERSION VERSION_LESS "3.18")
    set(_preprocess PRIVATE -cpp)
  endif()

  sundials_add_library(${target}
    SOURCES ${sundials_add_f2003_library_SOURCES}
    OBJECT_LIBRARIES ${sundials_add_f2003_library_OBJECT_LIBRARIES}
    LINK_LIBRARIES
      ${sundials_add_f2003_library_LINK_LIBRARIES}
      PUBLIC ${_clib_name} # depend on the c library
    INCLUDE_DIRECTORIES
      ${sundials_add_f2003_library_INCLUDE_DIRECTORIES}
      ${_includes}
    COMPILE_DEFINITIONS ${sundials_add_f2003_library_COMPILE_DEFINITIONS}
    COMPILE_OPTIONS ${sundials_add_f2003_library_COMPILE_OPTIONS} ${_preprocess}
    PROPERTIES ${sundials_add_f2003_library_PROPERTIES} ${_properties}
    OUTPUT_NAME ${sundials_add_f2003_library_OUTPUT_NAME}
    VERSION ${sundials_add_f2003_library_VERSION}
    SOVERSION ${sundials_add_f2003_library_SOVERSION}
    ${sundials_add_f2003_library_UNPARSED_ARGUMENTS}
  )

endmacro()


macro(append_static_suffix libs_in libs_out)
  set(${libs_out} "")
  foreach(_lib ${${libs_in}})
    if(TARGET ${_lib}${_STATIC_LIB_SUFFIX})
      list(APPEND ${libs_out} ${_lib}${_STATIC_LIB_SUFFIX})
    else()
      list(APPEND ${libs_out} ${_lib})
    endif()
  endforeach()
endmacro()
