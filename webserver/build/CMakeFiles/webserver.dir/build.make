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
CMAKE_SOURCE_DIR = /home/ouyangyiqi/Learning/Linux/webserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ouyangyiqi/Learning/Linux/webserver/build

# Include any dependencies generated for this target.
include CMakeFiles/webserver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/webserver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/webserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserver.dir/flags.make

CMakeFiles/webserver.dir/http_conn.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/http_conn.cpp.o: /home/ouyangyiqi/Learning/Linux/webserver/http_conn.cpp
CMakeFiles/webserver.dir/http_conn.cpp.o: CMakeFiles/webserver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ouyangyiqi/Learning/Linux/webserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webserver.dir/http_conn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webserver.dir/http_conn.cpp.o -MF CMakeFiles/webserver.dir/http_conn.cpp.o.d -o CMakeFiles/webserver.dir/http_conn.cpp.o -c /home/ouyangyiqi/Learning/Linux/webserver/http_conn.cpp

CMakeFiles/webserver.dir/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/webserver.dir/http_conn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ouyangyiqi/Learning/Linux/webserver/http_conn.cpp > CMakeFiles/webserver.dir/http_conn.cpp.i

CMakeFiles/webserver.dir/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/http_conn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ouyangyiqi/Learning/Linux/webserver/http_conn.cpp -o CMakeFiles/webserver.dir/http_conn.cpp.s

CMakeFiles/webserver.dir/main.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/main.cpp.o: /home/ouyangyiqi/Learning/Linux/webserver/main.cpp
CMakeFiles/webserver.dir/main.cpp.o: CMakeFiles/webserver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ouyangyiqi/Learning/Linux/webserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webserver.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webserver.dir/main.cpp.o -MF CMakeFiles/webserver.dir/main.cpp.o.d -o CMakeFiles/webserver.dir/main.cpp.o -c /home/ouyangyiqi/Learning/Linux/webserver/main.cpp

CMakeFiles/webserver.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/webserver.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ouyangyiqi/Learning/Linux/webserver/main.cpp > CMakeFiles/webserver.dir/main.cpp.i

CMakeFiles/webserver.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ouyangyiqi/Learning/Linux/webserver/main.cpp -o CMakeFiles/webserver.dir/main.cpp.s

# Object files for target webserver
webserver_OBJECTS = \
"CMakeFiles/webserver.dir/http_conn.cpp.o" \
"CMakeFiles/webserver.dir/main.cpp.o"

# External object files for target webserver
webserver_EXTERNAL_OBJECTS =

webserver: CMakeFiles/webserver.dir/http_conn.cpp.o
webserver: CMakeFiles/webserver.dir/main.cpp.o
webserver: CMakeFiles/webserver.dir/build.make
webserver: CMakeFiles/webserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ouyangyiqi/Learning/Linux/webserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable webserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserver.dir/build: webserver
.PHONY : CMakeFiles/webserver.dir/build

CMakeFiles/webserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserver.dir/clean

CMakeFiles/webserver.dir/depend:
	cd /home/ouyangyiqi/Learning/Linux/webserver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ouyangyiqi/Learning/Linux/webserver /home/ouyangyiqi/Learning/Linux/webserver /home/ouyangyiqi/Learning/Linux/webserver/build /home/ouyangyiqi/Learning/Linux/webserver/build /home/ouyangyiqi/Learning/Linux/webserver/build/CMakeFiles/webserver.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/webserver.dir/depend
