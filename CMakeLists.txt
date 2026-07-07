cmake_minimum_required(VERSION 3.20)

project(
  occ-shape-inspector
  VERSION 0.1.0
  DESCRIPTION "Lightweight CAD shape inspection tool"
  LANGUAGES CXX)

include(CMakeDependentOption)

option(OSI_BUILD_CLI "Build the osi-inspect command line tool" ON)
option(OSI_BUILD_GUI "Build the optional Qt GUI application" OFF)
option(OSI_BUILD_TESTS "Build smoke tests" ON)
option(OSI_WITH_OCCT "Enable Open CASCADE integration when available" ON)
set(OSI_OCCT_THIRDPARTY_DIR "" CACHE PATH
    "Optional third-party root directory for OCCT combined binary packages")
set(OSI_OCCT_SUPPRESS_DEFAULT_LIBRARIES "jemalloc.lib" CACHE STRING
    "MSVC default libraries suppressed when linking OCCT combined packages")
cmake_dependent_option(OSI_WITH_QT "Enable Qt integration when available" ON
                       "OSI_BUILD_GUI" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OSI_HAS_OCCT OFF)
if(OSI_WITH_OCCT)
  find_package(OpenCASCADE QUIET)
  if(OpenCASCADE_FOUND)
    set(OSI_HAS_OCCT ON)
    message(STATUS "Open CASCADE found; OCCT-backed loaders and statistics are enabled.")
  else()
    message(
      STATUS
        "Open CASCADE was requested but not found. Building with OCCT-backed loading disabled."
    )
  endif()
endif()

set(OSI_OCCT_COMMON_MODELING_TOOLKITS
  TKernel
  TKMath
  TKG2d
  TKG3d
  TKGeomBase
  TKGeomAlgo
  TKBRep
  TKTopAlgo)

set(OSI_OCCT_LOADER_BASE_TOOLKITS
  ${OSI_OCCT_COMMON_MODELING_TOOLKITS}
  TKXSBase)

set(OSI_OCCT_ANALYZER_TOOLKITS
  ${OSI_OCCT_COMMON_MODELING_TOOLKITS})

function(osi_list_existing_dirs out_var)
  set(existing_dirs)
  foreach(candidate IN LISTS ARGN)
    if(candidate AND IS_DIRECTORY "${candidate}")
      list(APPEND existing_dirs "${candidate}")
    endif()
  endforeach()
  list(REMOVE_DUPLICATES existing_dirs)
  set(${out_var} ${existing_dirs} PARENT_SCOPE)
endfunction()

function(osi_collect_occt_library_candidates out_var)
  set(candidates)

  if(OpenCASCADE_LIBRARY_DIR)
    list(APPEND candidates "${OpenCASCADE_LIBRARY_DIR}")
  endif()
  if(OpenCASCADE_LIBRARY_DIRS)
    list(APPEND candidates ${OpenCASCADE_LIBRARY_DIRS})
  endif()

  if(OpenCASCADE_DIR)
    get_filename_component(occt_cmake_dir "${OpenCASCADE_DIR}" ABSOLUTE)
    get_filename_component(occt_root_dir "${occt_cmake_dir}" DIRECTORY)
    list(APPEND candidates
      "${occt_root_dir}/lib"
      "${occt_root_dir}/win64/vc14/lib"
      "${occt_root_dir}/win64/vc141/lib"
      "${occt_root_dir}/win64/vc142/lib"
      "${occt_root_dir}/win64/vc143/lib"
      "${occt_root_dir}/win64/vc144/lib")
    file(GLOB occt_globbed_lib_dirs LIST_DIRECTORIES true
         "${occt_root_dir}/win64/*/lib")
    list(APPEND candidates ${occt_globbed_lib_dirs})
  endif()

  osi_list_existing_dirs(existing_candidates ${candidates})
  set(${out_var} ${existing_candidates} PARENT_SCOPE)
endfunction()

function(osi_find_occt_toolkit out_var toolkit_name)
  find_library(detected_toolkit
               NAMES ${toolkit_name}
               PATHS "${OSI_OCCT_LIBRARY_DIR}"
               NO_DEFAULT_PATH)
  set(${out_var} "${detected_toolkit}" PARENT_SCOPE)
endfunction()

function(osi_append_existing_occt_toolkits out_var)
  set(existing_toolkits ${${out_var}})
  foreach(toolkit IN LISTS ARGN)
    osi_find_occt_toolkit(detected_toolkit ${toolkit})
    if(detected_toolkit)
      list(APPEND existing_toolkits ${toolkit})
      message(STATUS "Using optional OCCT toolkit: ${toolkit}")
    endif()
  endforeach()
  list(REMOVE_DUPLICATES existing_toolkits)
  set(${out_var} ${existing_toolkits} PARENT_SCOPE)
endfunction()

function(osi_detect_occt_thirdparty_dir out_var)
  set(detected_thirdparty_dir "")

  if(OSI_OCCT_THIRDPARTY_DIR)
    get_filename_component(detected_thirdparty_dir "${OSI_OCCT_THIRDPARTY_DIR}" ABSOLUTE)
  elseif(OpenCASCADE_DIR)
    get_filename_component(occt_cmake_dir "${OpenCASCADE_DIR}" ABSOLUTE)
    get_filename_component(occt_root_dir "${occt_cmake_dir}" DIRECTORY)
    get_filename_component(combined_root_dir "${occt_root_dir}" DIRECTORY)
    file(GLOB thirdparty_candidates LIST_DIRECTORIES true
         "${combined_root_dir}/3rdparty-*")
    if(thirdparty_candidates)
      list(SORT thirdparty_candidates)
      list(GET thirdparty_candidates 0 detected_thirdparty_dir)
      set(OSI_OCCT_THIRDPARTY_DIR "${detected_thirdparty_dir}" CACHE PATH
          "Optional third-party root directory for OCCT combined binary packages" FORCE)
    endif()
  endif()

  if(detected_thirdparty_dir AND IS_DIRECTORY "${detected_thirdparty_dir}")
    set(${out_var} "${detected_thirdparty_dir}" PARENT_SCOPE)
  else()
    set(${out_var} "" PARENT_SCOPE)
  endif()
endfunction()

function(osi_collect_occt_thirdparty_dirs thirdparty_root out_lib_dirs out_bin_dirs)
  set(library_dirs)
  set(runtime_dirs)

  if(thirdparty_root AND IS_DIRECTORY "${thirdparty_root}")
    file(GLOB_RECURSE thirdparty_libraries
         LIST_DIRECTORIES false
         "${thirdparty_root}/*.lib")
    foreach(library_file IN LISTS thirdparty_libraries)
      get_filename_component(library_dir "${library_file}" DIRECTORY)
      list(APPEND library_dirs "${library_dir}")
    endforeach()

    file(GLOB_RECURSE thirdparty_runtimes
         LIST_DIRECTORIES false
         "${thirdparty_root}/*.dll")
    foreach(runtime_file IN LISTS thirdparty_runtimes)
      get_filename_component(runtime_dir "${runtime_file}" DIRECTORY)
      list(APPEND runtime_dirs "${runtime_dir}")
    endforeach()
  endif()

  list(REMOVE_DUPLICATES library_dirs)
  list(REMOVE_DUPLICATES runtime_dirs)
  set(${out_lib_dirs} ${library_dirs} PARENT_SCOPE)
  set(${out_bin_dirs} ${runtime_dirs} PARENT_SCOPE)
endfunction()

function(osi_configure_occt_libraries)
  if(NOT OSI_HAS_OCCT)
    return()
  endif()

  osi_collect_occt_library_candidates(occt_library_candidates)
  find_library(OSI_OCCT_TKERNEL_LIBRARY
               NAMES TKernel
               PATHS ${occt_library_candidates}
               NO_DEFAULT_PATH)
  if(NOT OSI_OCCT_TKERNEL_LIBRARY)
    message(FATAL_ERROR
      "OCCT TKernel library was not found. Set OpenCASCADE_DIR to the OCCT cmake "
      "directory or provide a package with lib/win64/<compiler>/lib libraries.")
  endif()

  get_filename_component(detected_occt_library_dir "${OSI_OCCT_TKERNEL_LIBRARY}" DIRECTORY)
  set(OSI_OCCT_LIBRARY_DIR "${detected_occt_library_dir}" CACHE PATH
      "Detected OCCT library directory" FORCE)
  message(STATUS "Using OCCT library directory: ${OSI_OCCT_LIBRARY_DIR}")

  set(configured_loader_toolkits ${OSI_OCCT_LOADER_BASE_TOOLKITS})
  osi_append_existing_occt_toolkits(configured_loader_toolkits TKDE)

  if(EXISTS "${OSI_OCCT_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}TKDESTEP${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(OSI_OCCT_STEP_TOOLKITS TKDESTEP CACHE INTERNAL "OCCT STEP toolkits" FORCE)
    message(STATUS "Using OCCT STEP toolkit: TKDESTEP")
  else()
    set(legacy_step_toolkits TKSTEPBase TKSTEPAttr TKSTEP209 TKSTEP)
    set(missing_legacy_step_toolkits)
    foreach(toolkit IN LISTS legacy_step_toolkits)
      if(NOT EXISTS "${OSI_OCCT_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${toolkit}${CMAKE_STATIC_LIBRARY_SUFFIX}")
        list(APPEND missing_legacy_step_toolkits ${toolkit})
      endif()
    endforeach()

    if(missing_legacy_step_toolkits)
      message(FATAL_ERROR
        "OCCT STEP toolkit was not found. Expected TKDESTEP or "
        "TKSTEPBase/TKSTEPAttr/TKSTEP209/TKSTEP in ${OSI_OCCT_LIBRARY_DIR}. "
        "Missing legacy toolkits: ${missing_legacy_step_toolkits}")
    endif()

    set(OSI_OCCT_STEP_TOOLKITS ${legacy_step_toolkits} CACHE INTERNAL "OCCT STEP toolkits" FORCE)
    message(STATUS "Using legacy OCCT STEP toolkits: ${OSI_OCCT_STEP_TOOLKITS}")
  endif()

  list(APPEND configured_loader_toolkits ${OSI_OCCT_STEP_TOOLKITS})
  list(REMOVE_DUPLICATES configured_loader_toolkits)
  set(OSI_OCCT_LOADER_TOOLKITS ${configured_loader_toolkits}
      CACHE INTERNAL "Configured OCCT loader toolkits" FORCE)
  set(OSI_OCCT_ANALYZER_TOOLKITS_CONFIGURED ${OSI_OCCT_ANALYZER_TOOLKITS}
      CACHE INTERNAL "Configured OCCT analyzer toolkits" FORCE)

  osi_detect_occt_thirdparty_dir(detected_thirdparty_dir)
  set(OSI_OCCT_THIRDPARTY_LIBRARY_DIRS "" CACHE INTERNAL
      "Detected OCCT third-party library directories" FORCE)
  set(OSI_OCCT_THIRDPARTY_RUNTIME_DIRS "" CACHE INTERNAL
      "Detected OCCT third-party runtime directories" FORCE)
  unset(OSI_OCCT_JEMALLOC_LIBRARY CACHE)
  unset(OSI_OCCT_JEMALLOC_LIBRARY_DIR CACHE)

  if(detected_thirdparty_dir)
    message(STATUS "Using OCCT third-party directory: ${detected_thirdparty_dir}")
    osi_collect_occt_thirdparty_dirs("${detected_thirdparty_dir}"
                                     thirdparty_library_dirs
                                     thirdparty_runtime_dirs)
    set(OSI_OCCT_THIRDPARTY_LIBRARY_DIRS ${thirdparty_library_dirs} CACHE INTERNAL
        "Detected OCCT third-party library directories" FORCE)
    set(OSI_OCCT_THIRDPARTY_RUNTIME_DIRS ${thirdparty_runtime_dirs} CACHE INTERNAL
        "Detected OCCT third-party runtime directories" FORCE)
    list(LENGTH thirdparty_library_dirs thirdparty_library_dir_count)
    message(STATUS "Detected ${thirdparty_library_dir_count} OCCT third-party library directories.")

    find_library(OSI_OCCT_JEMALLOC_LIBRARY
                 NAMES jemalloc
                 PATHS ${thirdparty_library_dirs}
                 NO_DEFAULT_PATH)
    if(OSI_OCCT_JEMALLOC_LIBRARY)
      get_filename_component(jemalloc_library_dir "${OSI_OCCT_JEMALLOC_LIBRARY}" DIRECTORY)
      set(OSI_OCCT_JEMALLOC_LIBRARY_DIR "${jemalloc_library_dir}" CACHE PATH
          "Detected jemalloc library directory for OCCT" FORCE)
      message(STATUS "Using OCCT jemalloc library: ${OSI_OCCT_JEMALLOC_LIBRARY}")
    else()
      message(STATUS
        "OCCT third-party directory was found, but jemalloc.lib was not found below it. "
        "If your OCCT package requires jemalloc, pass "
        "-DOSI_OCCT_THIRDPARTY_DIR=<path-to-3rdparty-vc14-64>.")
    endif()
  else()
    message(STATUS
      "OCCT third-party directory was not found. If your OCCT package requires third-party "
      "libraries, pass -DOSI_OCCT_THIRDPARTY_DIR=<path-to-3rdparty-vc14-64>.")
  endif()
endfunction()

osi_configure_occt_libraries()

function(osi_target_include_occt target_name)
  if(NOT OSI_HAS_OCCT)
    return()
  endif()

  if(OpenCASCADE_INCLUDE_DIR)
    target_include_directories(${target_name} PRIVATE ${OpenCASCADE_INCLUDE_DIR})
  elseif(OpenCASCADE_INCLUDE_DIRS)
    target_include_directories(${target_name} PRIVATE ${OpenCASCADE_INCLUDE_DIRS})
  endif()
endfunction()

function(osi_target_link_occt_directories target_name)
  if(NOT OSI_HAS_OCCT)
    return()
  endif()

  if(OSI_OCCT_LIBRARY_DIR)
    target_link_directories(${target_name} INTERFACE "${OSI_OCCT_LIBRARY_DIR}")
  endif()

  if(OSI_OCCT_THIRDPARTY_LIBRARY_DIRS)
    target_link_directories(${target_name} INTERFACE ${OSI_OCCT_THIRDPARTY_LIBRARY_DIRS})
  endif()
endfunction()

function(osi_target_link_occt_thirdparty target_name)
  if(NOT OSI_HAS_OCCT)
    return()
  endif()

  if(OSI_OCCT_JEMALLOC_LIBRARY)
    target_link_libraries(${target_name} INTERFACE "${OSI_OCCT_JEMALLOC_LIBRARY}")
  endif()

  if(MSVC AND OSI_OCCT_SUPPRESS_DEFAULT_LIBRARIES)
    foreach(default_library IN LISTS OSI_OCCT_SUPPRESS_DEFAULT_LIBRARIES)
      target_link_options(${target_name} INTERFACE "/NODEFAULTLIB:${default_library}")
    endforeach()
  endif()
endfunction()

if(OSI_HAS_OCCT)
  add_library(osi_occt_loader_toolkits INTERFACE)
  add_library(osi::occt_loader_toolkits ALIAS osi_occt_loader_toolkits)
  osi_target_link_occt_directories(osi_occt_loader_toolkits)
  osi_target_link_occt_thirdparty(osi_occt_loader_toolkits)
  target_link_libraries(osi_occt_loader_toolkits INTERFACE ${OSI_OCCT_LOADER_TOOLKITS})

  add_library(osi_occt_analyzer_toolkits INTERFACE)
  add_library(osi::occt_analyzer_toolkits ALIAS osi_occt_analyzer_toolkits)
  osi_target_link_occt_directories(osi_occt_analyzer_toolkits)
  osi_target_link_occt_thirdparty(osi_occt_analyzer_toolkits)
  target_link_libraries(osi_occt_analyzer_toolkits INTERFACE ${OSI_OCCT_ANALYZER_TOOLKITS_CONFIGURED})
endif()

set(OSI_HAS_QT OFF)
if(OSI_WITH_QT)
  find_package(Qt6 COMPONENTS Widgets QUIET)
  if(Qt6_FOUND)
    set(OSI_HAS_QT ON)
    message(STATUS "Qt6 found; GUI targets are reserved for later phases.")
  else()
    message(
      STATUS
        "Qt was requested but not found. Core libraries and CLI are unaffected.")
  endif()
endif()

configure_file(cmake/osi_config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/generated/osi/osi_config.h @ONLY)

add_library(osi_core
  src/core/IssueLocation.cpp
  src/core/ShapeDocument.cpp
  src/core/ShapeIssue.cpp
  src/core/ShapeStatistics.cpp)
add_library(osi::core ALIAS osi_core)
target_include_directories(
  osi_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
    $<INSTALL_INTERFACE:include>)
target_compile_features(osi_core PUBLIC cxx_std_17)

add_library(osi_report src/report/ShapeReport.cpp)
add_library(osi::report ALIAS osi_report)
target_include_directories(
  osi_report
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(osi_report PUBLIC osi::core)
target_compile_features(osi_report PUBLIC cxx_std_17)

add_library(osi_io src/io/ShapeLoader.cpp)
add_library(osi::io ALIAS osi_io)
target_include_directories(
  osi_io
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(osi_io PUBLIC osi::core)
target_compile_features(osi_io PUBLIC cxx_std_17)
osi_target_include_occt(osi_io)
if(OSI_HAS_OCCT)
  target_link_libraries(osi_io PUBLIC osi::occt_loader_toolkits)
endif()

add_library(osi_analyzer
  src/analyzer/ShapeIssueDetector.cpp
  src/analyzer/ShapeAnalyzer.cpp
  src/analyzer/ShapeStatisticsCollector.cpp)
add_library(osi::analyzer ALIAS osi_analyzer)
target_include_directories(
  osi_analyzer
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(osi_analyzer PUBLIC osi::core osi::report)
target_compile_features(osi_analyzer PUBLIC cxx_std_17)
osi_target_include_occt(osi_analyzer)
if(OSI_HAS_OCCT)
  target_link_libraries(osi_analyzer PUBLIC osi::occt_analyzer_toolkits)
endif()

if(OSI_BUILD_CLI)
  add_executable(osi-inspect app/cli/main.cpp)
  target_link_libraries(osi-inspect PRIVATE osi::io osi::analyzer osi::report osi::core)
  target_compile_features(osi-inspect PRIVATE cxx_std_17)
endif()

if(OSI_BUILD_GUI)
  message(STATUS "GUI build is enabled, but the real GUI target is intentionally deferred.")
endif()

if(OSI_BUILD_TESTS)
  enable_testing()
  add_executable(osi_core_smoke_tests tests/core_smoke_test.cpp)
  target_link_libraries(osi_core_smoke_tests PRIVATE osi::core osi::report osi::analyzer)
  target_compile_features(osi_core_smoke_tests PRIVATE cxx_std_17)
  add_test(NAME osi_core_smoke_tests COMMAND osi_core_smoke_tests)

  add_executable(osi_io_loader_tests tests/io_loader_test.cpp)
  target_link_libraries(osi_io_loader_tests PRIVATE osi::io)
  target_compile_features(osi_io_loader_tests PRIVATE cxx_std_17)
  add_test(NAME osi_io_loader_tests COMMAND osi_io_loader_tests)
endif()
