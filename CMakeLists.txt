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

function(osi_target_link_occt target_name)
  if(NOT OSI_HAS_OCCT)
    return()
  endif()

  if(OpenCASCADE_INCLUDE_DIR)
    target_include_directories(${target_name} PRIVATE ${OpenCASCADE_INCLUDE_DIR})
  elseif(OpenCASCADE_INCLUDE_DIRS)
    target_include_directories(${target_name} PRIVATE ${OpenCASCADE_INCLUDE_DIRS})
  endif()

  if(OpenCASCADE_LIBRARIES)
    target_link_libraries(${target_name} PRIVATE ${OpenCASCADE_LIBRARIES})
  else()
    target_link_libraries(
      ${target_name}
      PRIVATE
        TKernel
        TKMath
        TKBRep
        TKTopAlgo
        TKGeomBase
        TKGeomAlgo
        TKXSBase
        TKSTEPBase
        TKSTEPAttr
        TKSTEP209
        TKSTEP)
  endif()
endfunction()

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
osi_target_link_occt(osi_io)

add_library(osi_analyzer
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
osi_target_link_occt(osi_analyzer)

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
  target_link_libraries(osi_core_smoke_tests PRIVATE osi::core osi::report)
  target_compile_features(osi_core_smoke_tests PRIVATE cxx_std_17)
  add_test(NAME osi_core_smoke_tests COMMAND osi_core_smoke_tests)

  add_executable(osi_io_loader_tests tests/io_loader_test.cpp)
  target_link_libraries(osi_io_loader_tests PRIVATE osi::io)
  target_compile_features(osi_io_loader_tests PRIVATE cxx_std_17)
  add_test(NAME osi_io_loader_tests COMMAND osi_io_loader_tests)
endif()
