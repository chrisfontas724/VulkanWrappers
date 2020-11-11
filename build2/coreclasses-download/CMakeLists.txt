cmake_minimum_required(VERSION 3.7)

project(filesystem-download NONE)
project(openexr-download NONE)
project(coreclasses-download NONE)
project(glslang-download NONE)

include(ExternalProject)

ExternalProject_Add(filesystem
  GIT_REPOSITORY    https://github.com/chrisfontas724/FileSystem.git
  GIT_TAG           master
  SOURCE_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-src"
  BINARY_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/filesystem-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

ExternalProject_Add(coreclasses
  GIT_REPOSITORY    https://github.com/chrisfontas724/CoreClasses.git
  GIT_TAG           master
  SOURCE_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-src"
  BINARY_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/coreclasses-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

ExternalProject_Add(glslang 
  GIT_REPOSITORY    https://github.com/KhronosGroup/glslang.git
  GIT_TAG           master
  SOURCE_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src"
  BINARY_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

ExternalProject_Add(openexr 
  GIT_REPOSITORY    https://github.com/mitsuba-renderer/openexr.git
  GIT_TAG           master
  SOURCE_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src"
  BINARY_DIR        "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
