# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nishanth/Documents/Penn/CIS700-Pung

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nishanth/Documents/Penn/CIS700-Pung

# Include any dependencies generated for this target.
include CMakeFiles/master_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/master_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/master_server.dir/flags.make

CMakeFiles/master_server.dir/src/master_server.cc.o: CMakeFiles/master_server.dir/flags.make
CMakeFiles/master_server.dir/src/master_server.cc.o: src/master_server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nishanth/Documents/Penn/CIS700-Pung/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/master_server.dir/src/master_server.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/master_server.dir/src/master_server.cc.o -c /Users/nishanth/Documents/Penn/CIS700-Pung/src/master_server.cc

CMakeFiles/master_server.dir/src/master_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master_server.dir/src/master_server.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nishanth/Documents/Penn/CIS700-Pung/src/master_server.cc > CMakeFiles/master_server.dir/src/master_server.cc.i

CMakeFiles/master_server.dir/src/master_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master_server.dir/src/master_server.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nishanth/Documents/Penn/CIS700-Pung/src/master_server.cc -o CMakeFiles/master_server.dir/src/master_server.cc.s

CMakeFiles/master_server.dir/src/master_server.cc.o.requires:

.PHONY : CMakeFiles/master_server.dir/src/master_server.cc.o.requires

CMakeFiles/master_server.dir/src/master_server.cc.o.provides: CMakeFiles/master_server.dir/src/master_server.cc.o.requires
	$(MAKE) -f CMakeFiles/master_server.dir/build.make CMakeFiles/master_server.dir/src/master_server.cc.o.provides.build
.PHONY : CMakeFiles/master_server.dir/src/master_server.cc.o.provides

CMakeFiles/master_server.dir/src/master_server.cc.o.provides.build: CMakeFiles/master_server.dir/src/master_server.cc.o


CMakeFiles/master_server.dir/src/server_info.cc.o: CMakeFiles/master_server.dir/flags.make
CMakeFiles/master_server.dir/src/server_info.cc.o: src/server_info.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nishanth/Documents/Penn/CIS700-Pung/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/master_server.dir/src/server_info.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/master_server.dir/src/server_info.cc.o -c /Users/nishanth/Documents/Penn/CIS700-Pung/src/server_info.cc

CMakeFiles/master_server.dir/src/server_info.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master_server.dir/src/server_info.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nishanth/Documents/Penn/CIS700-Pung/src/server_info.cc > CMakeFiles/master_server.dir/src/server_info.cc.i

CMakeFiles/master_server.dir/src/server_info.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master_server.dir/src/server_info.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nishanth/Documents/Penn/CIS700-Pung/src/server_info.cc -o CMakeFiles/master_server.dir/src/server_info.cc.s

CMakeFiles/master_server.dir/src/server_info.cc.o.requires:

.PHONY : CMakeFiles/master_server.dir/src/server_info.cc.o.requires

CMakeFiles/master_server.dir/src/server_info.cc.o.provides: CMakeFiles/master_server.dir/src/server_info.cc.o.requires
	$(MAKE) -f CMakeFiles/master_server.dir/build.make CMakeFiles/master_server.dir/src/server_info.cc.o.provides.build
.PHONY : CMakeFiles/master_server.dir/src/server_info.cc.o.provides

CMakeFiles/master_server.dir/src/server_info.cc.o.provides.build: CMakeFiles/master_server.dir/src/server_info.cc.o


# Object files for target master_server
master_server_OBJECTS = \
"CMakeFiles/master_server.dir/src/master_server.cc.o" \
"CMakeFiles/master_server.dir/src/server_info.cc.o"

# External object files for target master_server
master_server_EXTERNAL_OBJECTS =

master_server: CMakeFiles/master_server.dir/src/master_server.cc.o
master_server: CMakeFiles/master_server.dir/src/server_info.cc.o
master_server: CMakeFiles/master_server.dir/build.make
master_server: /usr/local/lib/libboost_system-mt.dylib
master_server: /usr/local/lib/libboost_filesystem-mt.dylib
master_server: CMakeFiles/master_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/nishanth/Documents/Penn/CIS700-Pung/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable master_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/master_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/master_server.dir/build: master_server

.PHONY : CMakeFiles/master_server.dir/build

CMakeFiles/master_server.dir/requires: CMakeFiles/master_server.dir/src/master_server.cc.o.requires
CMakeFiles/master_server.dir/requires: CMakeFiles/master_server.dir/src/server_info.cc.o.requires

.PHONY : CMakeFiles/master_server.dir/requires

CMakeFiles/master_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/master_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/master_server.dir/clean

CMakeFiles/master_server.dir/depend:
	cd /Users/nishanth/Documents/Penn/CIS700-Pung && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nishanth/Documents/Penn/CIS700-Pung /Users/nishanth/Documents/Penn/CIS700-Pung /Users/nishanth/Documents/Penn/CIS700-Pung /Users/nishanth/Documents/Penn/CIS700-Pung /Users/nishanth/Documents/Penn/CIS700-Pung/CMakeFiles/master_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/master_server.dir/depend

