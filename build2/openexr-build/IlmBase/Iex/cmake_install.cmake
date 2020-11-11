# Install script for directory: C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-build/IlmBase/Iex/libIex-2_2.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-build/IlmBase/Iex/libIex-2_2.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIex-2_2.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIex-2_2.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/TDM-GCC-32/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIex-2_2.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexBaseExc.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexMathExc.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexThrowErrnoExc.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexErrnoExc.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexMacros.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/Iex.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexNamespace.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexExport.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/IlmBase/Iex/IexForward.h"
    )
endif()

