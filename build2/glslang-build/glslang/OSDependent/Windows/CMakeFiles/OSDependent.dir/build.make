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
include glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/depend.make

# Include the progress variables for this target.
include glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/progress.make

# Include the compile flags for this target's objects.
include glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/flags.make

glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/ossource.cpp.obj: glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/flags.make
glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/ossource.cpp.obj: glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/includes_CXX.rsp
glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/ossource.cpp.obj: glslang-src/glslang/OSDependent/Windows/ossource.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/ossource.cpp.obj"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\OSDependent.dir\ossource.cpp.obj -c C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\glslang\OSDependent\Windows\ossource.cpp

glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/ossource.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OSDependent.dir/ossource.cpp.i"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\glslang\OSDependent\Windows\ossource.cpp > CMakeFiles\OSDependent.dir\ossource.cpp.i

glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/ossource.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OSDependent.dir/ossource.cpp.s"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\glslang\OSDependent\Windows\ossource.cpp -o CMakeFiles\OSDependent.dir\ossource.cpp.s

# Object files for target OSDependent
OSDependent_OBJECTS = \
"CMakeFiles/OSDependent.dir/ossource.cpp.obj"

# External object files for target OSDependent
OSDependent_EXTERNAL_OBJECTS =

glslang-build/glslang/OSDependent/Windows/libOSDependentd.a: glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/ossource.cpp.obj
glslang-build/glslang/OSDependent/Windows/libOSDependentd.a: glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/build.make
glslang-build/glslang/OSDependent/Windows/libOSDependentd.a: glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libOSDependentd.a"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows && $(CMAKE_COMMAND) -P CMakeFiles\OSDependent.dir\cmake_clean_target.cmake
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\OSDependent.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/build: glslang-build/glslang/OSDependent/Windows/libOSDependentd.a

.PHONY : glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/build

glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/clean:
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows && $(CMAKE_COMMAND) -P CMakeFiles\OSDependent.dir\cmake_clean.cmake
.PHONY : glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/clean

glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Chris\Desktop\Projects\VulkanWrappers C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\glslang\OSDependent\Windows C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\glslang\OSDependent\Windows\CMakeFiles\OSDependent.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : glslang-build/glslang/OSDependent/Windows/CMakeFiles/OSDependent.dir/depend
