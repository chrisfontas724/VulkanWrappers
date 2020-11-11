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
CMAKE_SOURCE_DIR = C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download

# Utility rule file for filesystem.

# Include the progress variables for this target.
include CMakeFiles/filesystem.dir/progress.make

CMakeFiles/filesystem: CMakeFiles/filesystem-complete


CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-install
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-mkdir
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-download
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-update
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-patch
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-configure
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-build
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-install
CMakeFiles/filesystem-complete: filesystem-prefix/src/filesystem-stamp/filesystem-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'filesystem'"
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/CMakeFiles
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/CMakeFiles/filesystem-complete
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-done

filesystem-prefix/src/filesystem-stamp/filesystem-install: filesystem-prefix/src/filesystem-stamp/filesystem-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No install step for 'filesystem'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-install

filesystem-prefix/src/filesystem-stamp/filesystem-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'filesystem'"
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-src
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-build
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/tmp
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src
	"C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-mkdir

filesystem-prefix/src/filesystem-stamp/filesystem-download: filesystem-prefix/src/filesystem-stamp/filesystem-gitinfo.txt
filesystem-prefix/src/filesystem-stamp/filesystem-download: filesystem-prefix/src/filesystem-stamp/filesystem-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'filesystem'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 && "C:\Program Files\CMake\bin\cmake.exe" -P C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/tmp/filesystem-gitclone.cmake
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2 && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-download

filesystem-prefix/src/filesystem-stamp/filesystem-update: filesystem-prefix/src/filesystem-stamp/filesystem-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Performing update step for 'filesystem'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-src && "C:\Program Files\CMake\bin\cmake.exe" -P C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/tmp/filesystem-gitupdate.cmake

filesystem-prefix/src/filesystem-stamp/filesystem-patch: filesystem-prefix/src/filesystem-stamp/filesystem-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'filesystem'"
	"C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	"C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-patch

filesystem-prefix/src/filesystem-stamp/filesystem-configure: filesystem-prefix/tmp/filesystem-cfgcmd.txt
filesystem-prefix/src/filesystem-stamp/filesystem-configure: filesystem-prefix/src/filesystem-stamp/filesystem-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No configure step for 'filesystem'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-configure

filesystem-prefix/src/filesystem-stamp/filesystem-build: filesystem-prefix/src/filesystem-stamp/filesystem-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No build step for 'filesystem'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-build

filesystem-prefix/src/filesystem-stamp/filesystem-test: filesystem-prefix/src/filesystem-stamp/filesystem-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No test step for 'filesystem'"
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\filesystem-build && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-download/filesystem-prefix/src/filesystem-stamp/filesystem-test

filesystem: CMakeFiles/filesystem
filesystem: CMakeFiles/filesystem-complete
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-build
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-configure
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-download
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-install
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-mkdir
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-patch
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-test
filesystem: filesystem-prefix/src/filesystem-stamp/filesystem-update
filesystem: CMakeFiles/filesystem.dir/build.make

.PHONY : filesystem

# Rule to build all files generated by this target.
CMakeFiles/filesystem.dir/build: filesystem

.PHONY : CMakeFiles/filesystem.dir/build

CMakeFiles/filesystem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\filesystem.dir\cmake_clean.cmake
.PHONY : CMakeFiles/filesystem.dir/clean

CMakeFiles/filesystem.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download C:\Users\Chris\Desktop\Projects\VulkanWrappers\build2\coreclasses-download\CMakeFiles\filesystem.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/filesystem.dir/depend

