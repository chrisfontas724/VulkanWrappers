# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Chris\Desktop\Projects\VulkanWrappers

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2

# Utility rule file for NightlyMemCheck.

# Include the progress variables for this target.
include glslang-build/CMakeFiles/NightlyMemCheck.dir/progress.make

glslang-build/CMakeFiles/NightlyMemCheck:
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build && "C:\Program Files\CMake\bin\ctest.exe" -D NightlyMemCheck

NightlyMemCheck: glslang-build/CMakeFiles/NightlyMemCheck
NightlyMemCheck: glslang-build/CMakeFiles/NightlyMemCheck.dir/build.make

.PHONY : NightlyMemCheck

# Rule to build all files generated by this target.
glslang-build/CMakeFiles/NightlyMemCheck.dir/build: NightlyMemCheck

.PHONY : glslang-build/CMakeFiles/NightlyMemCheck.dir/build

glslang-build/CMakeFiles/NightlyMemCheck.dir/clean:
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build && $(CMAKE_COMMAND) -P CMakeFiles\NightlyMemCheck.dir\cmake_clean.cmake
.PHONY : glslang-build/CMakeFiles/NightlyMemCheck.dir/clean

glslang-build/CMakeFiles/NightlyMemCheck.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Chris\Desktop\Projects\VulkanWrappers C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\CMakeFiles\NightlyMemCheck.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : glslang-build/CMakeFiles/NightlyMemCheck.dir/depend

