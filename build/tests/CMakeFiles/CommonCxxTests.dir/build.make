# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/teh-phoenix/Documents/7/KPI7_PC_CW

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/teh-phoenix/Documents/7/KPI7_PC_CW/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/CommonCxxTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/CommonCxxTests.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/CommonCxxTests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/CommonCxxTests.dir/flags.make

tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/flags.make
tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o: tests/CommonCxxTests.cpp
tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/teh-phoenix/Documents/7/KPI7_PC_CW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o -MF CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o.d -o CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o -c /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests/CommonCxxTests.cpp

tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.i"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests/CommonCxxTests.cpp > CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.i

tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.s"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests/CommonCxxTests.cpp -o CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.s

tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/flags.make
tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o: /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/thread_pool_completion_test.cpp
tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/teh-phoenix/Documents/7/KPI7_PC_CW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o -MF CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o.d -o CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o -c /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/thread_pool_completion_test.cpp

tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.i"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/thread_pool_completion_test.cpp > CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.i

tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.s"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/thread_pool_completion_test.cpp -o CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.s

tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/flags.make
tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o: /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/inverted_index_write_test_single.cpp
tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/teh-phoenix/Documents/7/KPI7_PC_CW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o -MF CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o.d -o CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o -c /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/inverted_index_write_test_single.cpp

tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.i"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/inverted_index_write_test_single.cpp > CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.i

tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.s"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests/inverted_index_write_test_single.cpp -o CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.s

tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/flags.make
tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o: /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/inverted_index.cpp
tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/teh-phoenix/Documents/7/KPI7_PC_CW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o -MF CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o.d -o CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o -c /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/inverted_index.cpp

tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.i"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/inverted_index.cpp > CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.i

tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.s"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/inverted_index.cpp -o CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.s

tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/flags.make
tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o: /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/thread_pool.cpp
tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o: tests/CMakeFiles/CommonCxxTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/teh-phoenix/Documents/7/KPI7_PC_CW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o -MF CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o.d -o CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o -c /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/thread_pool.cpp

tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.i"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/thread_pool.cpp > CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.i

tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.s"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/teh-phoenix/Documents/7/KPI7_PC_CW/src/thread_pool.cpp -o CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.s

# Object files for target CommonCxxTests
CommonCxxTests_OBJECTS = \
"CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o" \
"CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o" \
"CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o" \
"CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o" \
"CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o"

# External object files for target CommonCxxTests
CommonCxxTests_EXTERNAL_OBJECTS =

tests/CommonCxxTests: tests/CMakeFiles/CommonCxxTests.dir/CommonCxxTests.cpp.o
tests/CommonCxxTests: tests/CMakeFiles/CommonCxxTests.dir/thread_pool_completion_test.cpp.o
tests/CommonCxxTests: tests/CMakeFiles/CommonCxxTests.dir/inverted_index_write_test_single.cpp.o
tests/CommonCxxTests: tests/CMakeFiles/CommonCxxTests.dir/__/src/inverted_index.cpp.o
tests/CommonCxxTests: tests/CMakeFiles/CommonCxxTests.dir/__/src/thread_pool.cpp.o
tests/CommonCxxTests: tests/CMakeFiles/CommonCxxTests.dir/build.make
tests/CommonCxxTests: tests/CMakeFiles/CommonCxxTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/teh-phoenix/Documents/7/KPI7_PC_CW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable CommonCxxTests"
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CommonCxxTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/CommonCxxTests.dir/build: tests/CommonCxxTests
.PHONY : tests/CMakeFiles/CommonCxxTests.dir/build

tests/CMakeFiles/CommonCxxTests.dir/clean:
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/CommonCxxTests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/CommonCxxTests.dir/clean

tests/CMakeFiles/CommonCxxTests.dir/depend:
	cd /home/teh-phoenix/Documents/7/KPI7_PC_CW/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/teh-phoenix/Documents/7/KPI7_PC_CW /home/teh-phoenix/Documents/7/KPI7_PC_CW/tests /home/teh-phoenix/Documents/7/KPI7_PC_CW/build /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests /home/teh-phoenix/Documents/7/KPI7_PC_CW/build/tests/CMakeFiles/CommonCxxTests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/CMakeFiles/CommonCxxTests.dir/depend

