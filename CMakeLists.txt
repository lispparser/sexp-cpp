cmake_minimum_required(VERSION 3.0)
project(lispparser VERSION 0.0.0)

option(WARNINGS "Switch on extra warnings" OFF)
option(WERROR "Turn warnings into errors" OFF)

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

add_compile_options(-std=c++1y)

# file(GLOB SUPERTUX_LISP_SOURCES supertux/lisp/*.cpp)
# add_library(supertux-lisp ${SUPERTUX_LISP_SOURCES})
# target_include_directories(supertux-lisp SYSTEM PUBLIC supertux)

# file(GLOB PINGUS_LISP_SOURCES pingus/lisp/*.cpp)
# add_library(pingus-lisp ${PINGUS_LISP_SOURCES})
# target_include_directories(pingus-lisp SYSTEM PUBLIC pingus)

file(GLOB LISPPARSER_SOURCES src/lisp/*.cpp)
add_library(lispparser ${LISPPARSER_SOURCES})
target_include_directories(lispparser SYSTEM PUBLIC src)

if(BUILD_TESTS)
  # build gtest
  # ${CMAKE_CURRENT_SOURCE_DIR} in include_directories is needed to generate -isystem instead of -I flags
  add_library(gtest_main STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/src/gtest_main.cc)
  target_include_directories(gtest_main SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include/)
  add_library(gtest STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/src/gtest-all.cc)
  target_include_directories(gtest SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/)
  target_include_directories(gtest SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include/)

  # build lispparser tests
  file(GLOB TEST_LISPPARSER_SOURCES tests/*.cpp)
  add_executable(test_lispparser ${TEST_LISPPARSER_SOURCES})
  target_compile_options(test_lispparser PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(test_lispparser
    gtest gtest_main
    lispparser
    ${CMAKE_THREAD_LIBS_INIT})

  # add 'make test' target, use 'make test ARGS="-V"' or 'ctest -V' for verbose
  enable_testing()
  add_test(NAME test_lispparser
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_lispparser)
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
    target_link_libraries(${SOURCE_BASENAME} benchmark lispparser ${CMAKE_THREAD_LIBS_INIT})
    set_target_properties(${SOURCE_BASENAME} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/benchmarks/")
    target_compile_options(${SOURCE_BASENAME} PRIVATE -std=c++1y ${WARNINGS_CXX_FLAGS})
  endforeach(SOURCE)
endif()

# EOF #
