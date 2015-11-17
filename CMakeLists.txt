# SExp - A S-Expression Parser for C++
# Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)
project(sexp VERSION 0.1.0)

option(WARNINGS "Switch on extra warnings" OFF)
option(WERROR "Turn warnings into errors" OFF)
option(USE_LOCALE "Use C++ locale support" OFF)

if(WARNINGS)
  set(WARNINGS_CXX_FLAGS ${WARNINGS_CXX_FLAGS}
    -pedantic -Wall -Wextra -Wno-c++0x-compat -Wnon-virtual-dtor
    -Wshadow -Wcast-qual -Winit-self -Wno-unused-parameter
    -Weffc++ -Wconversion -Wold-style-cast)
  # -Winline -Wfloat-equal -Wunreachable-code
endif()

if(WERROR)
  set(WARNINGS_CXX_FLAGS ${WARNINGS_CXX_FLAGS}
    -Werror)
endif()

find_package(Threads REQUIRED)

add_compile_options(-std=c++11)

if(USE_LOCALE)
  add_definitions(-DSEXP_USE_LOCALE)
endif()

# file(GLOB SUPERTUX_LISP_SOURCES supertux/lisp/*.cpp)
# add_library(supertux-lisp ${SUPERTUX_LISP_SOURCES})
# target_include_directories(supertux-lisp SYSTEM PUBLIC supertux)

# file(GLOB PINGUS_LISP_SOURCES pingus/lisp/*.cpp)
# add_library(pingus-lisp ${PINGUS_LISP_SOURCES})
# target_include_directories(pingus-lisp SYSTEM PUBLIC pingus)

file(GLOB SEXP_SOURCES src/*.cpp)
add_library(sexp ${SEXP_SOURCES})
target_compile_options(sexp PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(sexp SYSTEM PUBLIC include)

if(BUILD_TESTS)
  # build gtest
  # ${CMAKE_CURRENT_SOURCE_DIR} in include_directories is needed to generate -isystem instead of -I flags
  add_library(gtest_main STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/src/gtest_main.cc)
  target_include_directories(gtest_main SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include/)
  add_library(gtest STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/src/gtest-all.cc)
  target_include_directories(gtest SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/)
  target_include_directories(gtest SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include/)

  # build sexp tests
  file(GLOB TEST_SEXP_SOURCES tests/*.cpp)
  add_executable(test_sexp ${TEST_SEXP_SOURCES})
  target_compile_options(test_sexp PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(test_sexp
    gtest gtest_main
    sexp
    ${CMAKE_THREAD_LIBS_INIT})

  # add 'make test' target, use 'make test ARGS="-V"' or 'ctest -V' for verbose
  enable_testing()
  add_test(NAME test_sexp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_sexp)
endif()

if(BUILD_BENCHMARKS)
  # build google-benchmark
  include_directories(${PROJECT_SOURCE_DIR}/src)
  set(BENCHMARK_SOURCES
    "external/benchmark/src/benchmark.cc"
    "external/benchmark/src/colorprint.cc"
    "external/benchmark/src/commandlineflags.cc"
    "external/benchmark/src/console_reporter.cc"
    "external/benchmark/src/csv_reporter.cc"
    "external/benchmark/src/json_reporter.cc"
    "external/benchmark/src/log.cc"
    "external/benchmark/src/reporter.cc"
    "external/benchmark/src/sleep.cc"
    "external/benchmark/src/string_util.cc"
    "external/benchmark/src/sysinfo.cc"
    "external/benchmark/src/walltime.cc"
    "external/benchmark/src/re_std.cc")
  add_library(benchmark ${BENCHMARK_SOURCES})
  target_compile_definitions(benchmark PRIVATE -DHAVE_STD_REGEX)
  target_compile_options(benchmark PRIVATE -std=c++1y)
  target_include_directories(benchmark SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/benchmark/include)

  # build benchmarks
  file(GLOB BENCHMARKSOURCES benchmarks/*.cpp)
  foreach(SOURCE ${BENCHMARKSOURCES})
    get_filename_component(SOURCE_BASENAME ${SOURCE} NAME_WE)
    add_executable(${SOURCE_BASENAME} ${SOURCE})
    target_link_libraries(${SOURCE_BASENAME} benchmark sexp ${CMAKE_THREAD_LIBS_INIT})
    set_target_properties(${SOURCE_BASENAME} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/benchmarks/")
    target_compile_options(${SOURCE_BASENAME} PRIVATE -std=c++1y ${WARNINGS_CXX_FLAGS})
  endforeach(SOURCE)
endif()

# EOF #
