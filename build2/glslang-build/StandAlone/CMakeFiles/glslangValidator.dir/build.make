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
include glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/depend.make

# Include the progress variables for this target.
include glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/progress.make

# Include the compile flags for this target's objects.
include glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/flags.make

glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/StandAlone.cpp.obj: glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/flags.make
glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/StandAlone.cpp.obj: glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/includes_CXX.rsp
glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/StandAlone.cpp.obj: glslang-src/StandAlone/StandAlone.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/StandAlone.cpp.obj"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\StandAlone && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\glslangValidator.dir\StandAlone.cpp.obj -c C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\StandAlone\StandAlone.cpp

glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/StandAlone.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glslangValidator.dir/StandAlone.cpp.i"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\StandAlone && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\StandAlone\StandAlone.cpp > CMakeFiles\glslangValidator.dir\StandAlone.cpp.i

glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/StandAlone.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glslangValidator.dir/StandAlone.cpp.s"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\StandAlone && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\StandAlone\StandAlone.cpp -o CMakeFiles\glslangValidator.dir\StandAlone.cpp.s

# Object files for target glslangValidator
glslangValidator_OBJECTS = \
"CMakeFiles/glslangValidator.dir/StandAlone.cpp.obj"

# External object files for target glslangValidator
glslangValidator_EXTERNAL_OBJECTS =

glslang-build/StandAlone/glslangValidator.exe: glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/StandAlone.cpp.obj
glslang-build/StandAlone/glslangValidator.exe: glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/build.make
glslang-build/StandAlone/glslangValidator.exe: glslang-build/glslang/libglslang.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/SPIRV/libSPIRV.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/StandAlone/libglslang-default-resource-limits.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/SPIRV/libSPVRemapper.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/glslang/libMachineIndependent.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/OGLCompilersDLL/libOGLCompiler.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/glslang/OSDependent/Windows/libOSDependent.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/glslang/libGenericCodeGen.a
glslang-build/StandAlone/glslangValidator.exe: glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/linklibs.rsp
glslang-build/StandAlone/glslangValidator.exe: glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/objects1.rsp
glslang-build/StandAlone/glslangValidator.exe: glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable glslangValidator.exe"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\StandAlone && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\glslangValidator.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/build: glslang-build/StandAlone/glslangValidator.exe

.PHONY : glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/build

glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/clean:
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\StandAlone && $(CMAKE_COMMAND) -P CMakeFiles\glslangValidator.dir\cmake_clean.cmake
.PHONY : glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/clean

glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Chris\Desktop\Projects\VulkanWrappers C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-src\StandAlone C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\StandAlone C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\glslang-build\StandAlone\CMakeFiles\glslangValidator.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : glslang-build/StandAlone/CMakeFiles/glslangValidator.dir/depend

