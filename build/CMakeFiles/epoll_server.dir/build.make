# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yifanzhao/Desktop/epoll_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yifanzhao/Desktop/epoll_server/build

# Include any dependencies generated for this target.
include CMakeFiles/epoll_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/epoll_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/epoll_server.dir/flags.make

CMakeFiles/epoll_server.dir/Logger.o: CMakeFiles/epoll_server.dir/flags.make
CMakeFiles/epoll_server.dir/Logger.o: ../Logger.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yifanzhao/Desktop/epoll_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/epoll_server.dir/Logger.o"
	/usr/bin/x86_64-linux-gnu-g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epoll_server.dir/Logger.o -c /home/yifanzhao/Desktop/epoll_server/Logger.cc

CMakeFiles/epoll_server.dir/Logger.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epoll_server.dir/Logger.i"
	/usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yifanzhao/Desktop/epoll_server/Logger.cc > CMakeFiles/epoll_server.dir/Logger.i

CMakeFiles/epoll_server.dir/Logger.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epoll_server.dir/Logger.s"
	/usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yifanzhao/Desktop/epoll_server/Logger.cc -o CMakeFiles/epoll_server.dir/Logger.s

CMakeFiles/epoll_server.dir/Logger.o.requires:

.PHONY : CMakeFiles/epoll_server.dir/Logger.o.requires

CMakeFiles/epoll_server.dir/Logger.o.provides: CMakeFiles/epoll_server.dir/Logger.o.requires
	$(MAKE) -f CMakeFiles/epoll_server.dir/build.make CMakeFiles/epoll_server.dir/Logger.o.provides.build
.PHONY : CMakeFiles/epoll_server.dir/Logger.o.provides

CMakeFiles/epoll_server.dir/Logger.o.provides.build: CMakeFiles/epoll_server.dir/Logger.o


CMakeFiles/epoll_server.dir/TimeStamp.o: CMakeFiles/epoll_server.dir/flags.make
CMakeFiles/epoll_server.dir/TimeStamp.o: ../TimeStamp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yifanzhao/Desktop/epoll_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/epoll_server.dir/TimeStamp.o"
	/usr/bin/x86_64-linux-gnu-g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epoll_server.dir/TimeStamp.o -c /home/yifanzhao/Desktop/epoll_server/TimeStamp.cc

CMakeFiles/epoll_server.dir/TimeStamp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epoll_server.dir/TimeStamp.i"
	/usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yifanzhao/Desktop/epoll_server/TimeStamp.cc > CMakeFiles/epoll_server.dir/TimeStamp.i

CMakeFiles/epoll_server.dir/TimeStamp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epoll_server.dir/TimeStamp.s"
	/usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yifanzhao/Desktop/epoll_server/TimeStamp.cc -o CMakeFiles/epoll_server.dir/TimeStamp.s

CMakeFiles/epoll_server.dir/TimeStamp.o.requires:

.PHONY : CMakeFiles/epoll_server.dir/TimeStamp.o.requires

CMakeFiles/epoll_server.dir/TimeStamp.o.provides: CMakeFiles/epoll_server.dir/TimeStamp.o.requires
	$(MAKE) -f CMakeFiles/epoll_server.dir/build.make CMakeFiles/epoll_server.dir/TimeStamp.o.provides.build
.PHONY : CMakeFiles/epoll_server.dir/TimeStamp.o.provides

CMakeFiles/epoll_server.dir/TimeStamp.o.provides.build: CMakeFiles/epoll_server.dir/TimeStamp.o


# Object files for target epoll_server
epoll_server_OBJECTS = \
"CMakeFiles/epoll_server.dir/Logger.o" \
"CMakeFiles/epoll_server.dir/TimeStamp.o"

# External object files for target epoll_server
epoll_server_EXTERNAL_OBJECTS =

../lib/libepoll_server.so: CMakeFiles/epoll_server.dir/Logger.o
../lib/libepoll_server.so: CMakeFiles/epoll_server.dir/TimeStamp.o
../lib/libepoll_server.so: CMakeFiles/epoll_server.dir/build.make
../lib/libepoll_server.so: CMakeFiles/epoll_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yifanzhao/Desktop/epoll_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../lib/libepoll_server.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/epoll_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/epoll_server.dir/build: ../lib/libepoll_server.so

.PHONY : CMakeFiles/epoll_server.dir/build

CMakeFiles/epoll_server.dir/requires: CMakeFiles/epoll_server.dir/Logger.o.requires
CMakeFiles/epoll_server.dir/requires: CMakeFiles/epoll_server.dir/TimeStamp.o.requires

.PHONY : CMakeFiles/epoll_server.dir/requires

CMakeFiles/epoll_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/epoll_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/epoll_server.dir/clean

CMakeFiles/epoll_server.dir/depend:
	cd /home/yifanzhao/Desktop/epoll_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yifanzhao/Desktop/epoll_server /home/yifanzhao/Desktop/epoll_server /home/yifanzhao/Desktop/epoll_server/build /home/yifanzhao/Desktop/epoll_server/build /home/yifanzhao/Desktop/epoll_server/build/CMakeFiles/epoll_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/epoll_server.dir/depend

