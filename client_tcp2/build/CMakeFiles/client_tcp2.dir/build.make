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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.27.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.27.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/build

# Include any dependencies generated for this target.
include CMakeFiles/client_tcp2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client_tcp2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client_tcp2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client_tcp2.dir/flags.make

CMakeFiles/client_tcp2.dir/main.c.o: CMakeFiles/client_tcp2.dir/flags.make
CMakeFiles/client_tcp2.dir/main.c.o: /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/main.c
CMakeFiles/client_tcp2.dir/main.c.o: CMakeFiles/client_tcp2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/client_tcp2.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/client_tcp2.dir/main.c.o -MF CMakeFiles/client_tcp2.dir/main.c.o.d -o CMakeFiles/client_tcp2.dir/main.c.o -c /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/main.c

CMakeFiles/client_tcp2.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/client_tcp2.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/main.c > CMakeFiles/client_tcp2.dir/main.c.i

CMakeFiles/client_tcp2.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/client_tcp2.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/main.c -o CMakeFiles/client_tcp2.dir/main.c.s

# Object files for target client_tcp2
client_tcp2_OBJECTS = \
"CMakeFiles/client_tcp2.dir/main.c.o"

# External object files for target client_tcp2
client_tcp2_EXTERNAL_OBJECTS =

client_tcp2: CMakeFiles/client_tcp2.dir/main.c.o
client_tcp2: CMakeFiles/client_tcp2.dir/build.make
client_tcp2: CMakeFiles/client_tcp2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable client_tcp2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client_tcp2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client_tcp2.dir/build: client_tcp2
.PHONY : CMakeFiles/client_tcp2.dir/build

CMakeFiles/client_tcp2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client_tcp2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client_tcp2.dir/clean

CMakeFiles/client_tcp2.dir/depend:
	cd /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2 /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2 /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/build /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/build /Users/jansenjunior/Workspace/EdTutoring/ed-code/exercise-1/client_tcp2/build/CMakeFiles/client_tcp2.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/client_tcp2.dir/depend

