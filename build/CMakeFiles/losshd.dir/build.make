# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/max/cpp-projects/losshd-1.0.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/max/cpp-projects/losshd-1.0.1/build

# Include any dependencies generated for this target.
include CMakeFiles/losshd.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/losshd.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/losshd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/losshd.dir/flags.make

CMakeFiles/losshd.dir/losshd.cpp.o: CMakeFiles/losshd.dir/flags.make
CMakeFiles/losshd.dir/losshd.cpp.o: ../losshd.cpp
CMakeFiles/losshd.dir/losshd.cpp.o: CMakeFiles/losshd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/cpp-projects/losshd-1.0.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/losshd.dir/losshd.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/losshd.dir/losshd.cpp.o -MF CMakeFiles/losshd.dir/losshd.cpp.o.d -o CMakeFiles/losshd.dir/losshd.cpp.o -c /home/max/cpp-projects/losshd-1.0.1/losshd.cpp

CMakeFiles/losshd.dir/losshd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/losshd.dir/losshd.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/cpp-projects/losshd-1.0.1/losshd.cpp > CMakeFiles/losshd.dir/losshd.cpp.i

CMakeFiles/losshd.dir/losshd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/losshd.dir/losshd.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/cpp-projects/losshd-1.0.1/losshd.cpp -o CMakeFiles/losshd.dir/losshd.cpp.s

CMakeFiles/losshd.dir/options.cpp.o: CMakeFiles/losshd.dir/flags.make
CMakeFiles/losshd.dir/options.cpp.o: ../options.cpp
CMakeFiles/losshd.dir/options.cpp.o: CMakeFiles/losshd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/cpp-projects/losshd-1.0.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/losshd.dir/options.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/losshd.dir/options.cpp.o -MF CMakeFiles/losshd.dir/options.cpp.o.d -o CMakeFiles/losshd.dir/options.cpp.o -c /home/max/cpp-projects/losshd-1.0.1/options.cpp

CMakeFiles/losshd.dir/options.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/losshd.dir/options.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/cpp-projects/losshd-1.0.1/options.cpp > CMakeFiles/losshd.dir/options.cpp.i

CMakeFiles/losshd.dir/options.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/losshd.dir/options.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/cpp-projects/losshd-1.0.1/options.cpp -o CMakeFiles/losshd.dir/options.cpp.s

# Object files for target losshd
losshd_OBJECTS = \
"CMakeFiles/losshd.dir/losshd.cpp.o" \
"CMakeFiles/losshd.dir/options.cpp.o"

# External object files for target losshd
losshd_EXTERNAL_OBJECTS =

losshd: CMakeFiles/losshd.dir/losshd.cpp.o
losshd: CMakeFiles/losshd.dir/options.cpp.o
losshd: CMakeFiles/losshd.dir/build.make
losshd: CMakeFiles/losshd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/max/cpp-projects/losshd-1.0.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable losshd"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/losshd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/losshd.dir/build: losshd
.PHONY : CMakeFiles/losshd.dir/build

CMakeFiles/losshd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/losshd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/losshd.dir/clean

CMakeFiles/losshd.dir/depend:
	cd /home/max/cpp-projects/losshd-1.0.1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/max/cpp-projects/losshd-1.0.1 /home/max/cpp-projects/losshd-1.0.1 /home/max/cpp-projects/losshd-1.0.1/build /home/max/cpp-projects/losshd-1.0.1/build /home/max/cpp-projects/losshd-1.0.1/build/CMakeFiles/losshd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/losshd.dir/depend

