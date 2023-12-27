create_test_sourcelist (Tests CommonCxxTests.cpp
  thread_pool_completion_test.cpp
  inverted_index_write_test_single.cpp
)

add_executable (CommonCxxTests ${Tests} ${SOURCES} ${INCLUDES})

set (TestsToRun ${Tests})
remove (TestsToRun CommonCxxTests.cpp)
foreach (test ${TestsToRun})
  get_filename_component (TName ${test} NAME_WE)
  add_test (NAME ${TName} COMMAND CommonCxxTests ${TName})
endforeach ()