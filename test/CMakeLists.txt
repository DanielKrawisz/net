cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

option (OPTIONAL_TESTS "Build optional Tests" OFF)
include (GoogleTest)

add_executable (
    unit_tests
    schema.cpp

    #async
    async.cpp

    # net
    IP.cpp
    circular_queue.cpp
    email.cpp

    # io
    log.cpp
    args_parser.cpp
    #run.cpp
)

target_link_libraries (unit_tests
  PRIVATE net io crypto numbers gtest_main gmock)
gtest_discover_tests (unit_tests)

if (OPTIONAL_TESTS)
    add_subdirectory (optional)
endif ()

target_compile_options (unit_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-ftemplate-depth=2048>
    $<$<CXX_COMPILER_ID:Clang>:-ftemplate-depth=2048>
    $<$<CXX_COMPILER_ID:AppleClang>:-ftemplate-depth=2048>
    $<$<CXX_COMPILER_ID:MSVC>:/constexpr:depth2048>
)
