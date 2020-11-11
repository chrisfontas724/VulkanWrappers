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
CMAKE_SOURCE_DIR = C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download

# Utility rule file for openexr.

# Include the progress variables for this target.
include CMakeFiles/openexr.dir/progress.make

CMakeFiles/openexr: CMakeFiles/openexr-complete


CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-install
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-mkdir
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-download
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-update
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-patch
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-configure
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-build
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-install
CMakeFiles/openexr-complete: openexr-prefix/src/openexr-stamp/openexr-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'openexr'"
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/CMakeFiles
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/CMakeFiles/openexr-complete
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-done

openexr-prefix/src/openexr-stamp/openexr-install: openexr-prefix/src/openexr-stamp/openexr-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No install step for 'openexr'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-install

openexr-prefix/src/openexr-stamp/openexr-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'openexr'"
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-build
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/tmp
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-mkdir

openexr-prefix/src/openexr-stamp/openexr-download: openexr-prefix/src/openexr-stamp/openexr-gitinfo.txt
openexr-prefix/src/openexr-stamp/openexr-download: openexr-prefix/src/openexr-stamp/openexr-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'openexr'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 && "C:\Program Files\CMake\bin\cmake.exe" -P C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/tmp/openexr-gitclone.cmake
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-download

openexr-prefix/src/openexr-stamp/openexr-update: openexr-prefix/src/openexr-stamp/openexr-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Performing update step for 'openexr'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-src && "C:\Program Files\CMake\bin\cmake.exe" -P C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/tmp/openexr-gitupdate.cmake

openexr-prefix/src/openexr-stamp/openexr-patch: openexr-prefix/src/openexr-stamp/openexr-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'openexr'"
	"C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-patch

openexr-prefix/src/openexr-stamp/openexr-configure: openexr-prefix/tmp/openexr-cfgcmd.txt
openexr-prefix/src/openexr-stamp/openexr-configure: openexr-prefix/src/openexr-stamp/openexr-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No configure step for 'openexr'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-configure

openexr-prefix/src/openexr-stamp/openexr-build: openexr-prefix/src/openexr-stamp/openexr-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No build step for 'openexr'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-build

openexr-prefix/src/openexr-stamp/openexr-test: openexr-prefix/src/openexr-stamp/openexr-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No test step for 'openexr'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-download/openexr-prefix/src/openexr-stamp/openexr-test

openexr: CMakeFiles/openexr
openexr: CMakeFiles/openexr-complete
openexr: openexr-prefix/src/openexr-stamp/openexr-build
openexr: openexr-prefix/src/openexr-stamp/openexr-configure
openexr: openexr-prefix/src/openexr-stamp/openexr-download
openexr: openexr-prefix/src/openexr-stamp/openexr-install
openexr: openexr-prefix/src/openexr-stamp/openexr-mkdir
openexr: openexr-prefix/src/openexr-stamp/openexr-patch
openexr: openexr-prefix/src/openexr-stamp/openexr-test
openexr: openexr-prefix/src/openexr-stamp/openexr-update
openexr: CMakeFiles/openexr.dir/build.make

.PHONY : openexr

# Rule to build all files generated by this target.
CMakeFiles/openexr.dir/build: openexr

.PHONY : CMakeFiles/openexr.dir/build

CMakeFiles/openexr.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\openexr.dir\cmake_clean.cmake
.PHONY : CMakeFiles/openexr.dir/clean

CMakeFiles/openexr.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\openexr-download\CMakeFiles\openexr.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/openexr.dir/depend

