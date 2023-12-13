cmake_minimum_required(VERSION 3.25.0)
project(CW VERSION 0.1.0 LANGUAGES C CXX)
set(CXXStandard 23)

include(CTest)
enable_testing()

file(GLOB INCLUDES "include/*.h")
file(GLOB SOURCES "src/*.cpp")

add_executable(CWServer "exec/main.cpp" ${SOURCES} ${INCLUDES})
set_property(TARGET CWServer PROPERTY CXX_STANDARD ${CXXStandard})

create_test_sourcelist (Tests CommonCxxTests.cpp
  tests/thread_pool_completion_test.cpp
  )

add_executable (CommonCxxTests ${Tests} ${SOURCES} ${INCLUDES})
set_property(TARGET CommonCxxTests PROPERTY CXX_STANDARD ${CXXStandard})

set (TestsToRun ${Tests})
remove (TestsToRun CommonCxxTests.cxx)
foreach (test ${TestsToRun})
  get_filename_component (TName ${test} NAME_WE)
  add_test (NAME ${TName} COMMAND CommonCxxTests ${TName})
endforeach ()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
