# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/local/cmake/3.18.4/bin/cmake

# The command to remove a file.
RM = /usr/local/cmake/3.18.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/build

# Utility rule file for catch.

# Include the progress variables for this target.
include test/CMakeFiles/catch.dir/progress.make

test/CMakeFiles/catch:
	cd /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/build/test && ./test-trento

catch: test/CMakeFiles/catch
catch: test/CMakeFiles/catch.dir/build.make

.PHONY : catch

# Rule to build all files generated by this target.
test/CMakeFiles/catch.dir/build: catch

.PHONY : test/CMakeFiles/catch.dir/build

test/CMakeFiles/catch.dir/clean:
	cd /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/build/test && $(CMAKE_COMMAND) -P CMakeFiles/catch.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/catch.dir/clean

test/CMakeFiles/catch.dir/depend:
	cd /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/test /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/build /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/build/test /projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/build/test/CMakeFiles/catch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/catch.dir/depend
