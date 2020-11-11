
if(NOT "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-download/filesystem-prefix/src/filesystem-stamp/filesystem-gitinfo.txt" IS_NEWER_THAN "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-download/filesystem-prefix/src/filesystem-stamp/filesystem-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-download/filesystem-prefix/src/filesystem-stamp/filesystem-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  clone --no-checkout "https://github.com/chrisfontas724/FileSystem.git" "filesystem-src"
    WORKING_DIRECTORY "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/chrisfontas724/FileSystem.git'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe"  checkout master --
  WORKING_DIRECTORY "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'master'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-download/filesystem-prefix/src/filesystem-stamp/filesystem-gitinfo.txt"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-download/filesystem-prefix/src/filesystem-stamp/filesystem-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-download/filesystem-prefix/src/filesystem-stamp/filesystem-gitclone-lastrun.txt'")
endif()

