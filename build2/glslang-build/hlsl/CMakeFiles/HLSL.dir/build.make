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

# Include any dependencies generated for this target.
include glslang-build/hlsl/CMakeFiles/HLSL.dir/depend.make

# Include the progress variables for this target.
include glslang-build/hlsl/CMakeFiles/HLSL.dir/progress.make

# Include the compile flags for this target's objects.
include glslang-build/hlsl/CMakeFiles/HLSL.dir/flags.make

glslang-build/hlsl/CMakeFiles/HLSL.dir/stub.cpp.obj: glslang-build/hlsl/CMakeFiles/HLSL.dir/flags.make
glslang-build/hlsl/CMakeFiles/HLSL.dir/stub.cpp.obj: glslang-build/hlsl/CMakeFiles/HLSL.dir/includes_CXX.rsp
glslang-build/hlsl/CMakeFiles/HLSL.dir/stub.cpp.obj: glslang-src/hlsl/stub.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object glslang-build/hlsl/CMakeFiles/HLSL.dir/stub.cpp.obj"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\HLSL.dir\stub.cpp.obj -c C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\hlsl\stub.cpp

glslang-build/hlsl/CMakeFiles/HLSL.dir/stub.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/stub.cpp.i"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\hlsl\stub.cpp > CMakeFiles\HLSL.dir\stub.cpp.i

glslang-build/hlsl/CMakeFiles/HLSL.dir/stub.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/stub.cpp.s"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\hlsl\stub.cpp -o CMakeFiles\HLSL.dir\stub.cpp.s

# Object files for target HLSL
HLSL_OBJECTS = \
"CMakeFiles/HLSL.dir/stub.cpp.obj"

# External object files for target HLSL
HLSL_EXTERNAL_OBJECTS =

glslang-build/hlsl/libHLSL.a: glslang-build/hlsl/CMakeFiles/HLSL.dir/stub.cpp.obj
glslang-build/hlsl/libHLSL.a: glslang-build/hlsl/CMakeFiles/HLSL.dir/build.make
glslang-build/hlsl/libHLSL.a: glslang-build/hlsl/CMakeFiles/HLSL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libHLSL.a"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl && $(CMAKE_COMMAND) -P CMakeFiles\HLSL.dir\cmake_clean_target.cmake
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\HLSL.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glslang-build/hlsl/CMakeFiles/HLSL.dir/build: glslang-build/hlsl/libHLSL.a

.PHONY : glslang-build/hlsl/CMakeFiles/HLSL.dir/build

glslang-build/hlsl/CMakeFiles/HLSL.dir/clean:
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl && $(CMAKE_COMMAND) -P CMakeFiles\HLSL.dir\cmake_clean.cmake
.PHONY : glslang-build/hlsl/CMakeFiles/HLSL.dir/clean

glslang-build/hlsl/CMakeFiles/HLSL.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Chris\Desktop\Projects\VulkanWrappers C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\hlsl C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\hlsl\CMakeFiles\HLSL.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : glslang-build/hlsl/CMakeFiles/HLSL.dir/depend

