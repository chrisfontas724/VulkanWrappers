# Install script for directory: C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/TDM-GCC-32/bin/objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/libSPVRemapperd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/libSPIRVd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets.cmake"
         "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/CMakeFiles/Export/lib/cmake/SPVRemapperTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/CMakeFiles/Export/lib/cmake/SPVRemapperTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/CMakeFiles/Export/lib/cmake/SPVRemapperTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets.cmake"
         "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/CMakeFiles/Export/lib/cmake/SPIRVTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/CMakeFiles/Export/lib/cmake/SPIRVTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/SPIRV/CMakeFiles/Export/lib/cmake/SPIRVTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/glslang/SPIRV" TYPE FILE FILES
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/bitutils.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/spirv.hpp"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/GLSL.std.450.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/GLSL.ext.EXT.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/GLSL.ext.KHR.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/GlslangToSpv.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/hex_float.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/Logger.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/SpvBuilder.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/spvIR.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/doc.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/SpvTools.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/disassemble.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/GLSL.ext.AMD.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/GLSL.ext.NV.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/NonSemanticDebugPrintf.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/SPVRemapper.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/SPIRV/doc.h"
    )
endif()

