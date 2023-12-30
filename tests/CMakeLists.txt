create_test_sourcelist (Tests CommonCxxTests.cpp

  invertedindex_write_singlethread_test.cpp
  invertedindex_write_multithread_test.cpp

  threadpool_subscriber_test.cpp
  threadpool_thread_count_test.cpp
  threadpool_completion_test.cpp
  
)

add_executable (CommonCxxTests ${Tests} ${SOURCES} ${INCLUDES})

set (TestsToRun ${Tests})
remove (TestsToRun CommonCxxTests.cpp)
foreach (test ${TestsToRun})
  get_filename_component (TName ${test} NAME_WE)
  add_test (NAME ${TName} COMMAND CommonCxxTests ${TName})
endforeach ()