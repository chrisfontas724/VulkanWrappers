# Install script for directory: C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-build/OpenEXR/IlmImf/libIlmImf-2_2.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-build/OpenEXR/IlmImf/libIlmImf-2_2.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_2.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_2.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/TDM-GCC-32/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_2.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfForward.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfExport.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfBoxAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfCRgbaFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfChannelList.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfChannelListAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfCompressionAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDoubleAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfFloatAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfFrameBuffer.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfHeader.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfIO.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfInputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfIntAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfLineOrderAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfMatrixAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfOpaqueAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfOutputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfRgbaFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfStringAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfVecAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfHuf.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfWav.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfLut.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfArray.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfCompression.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfLineOrder.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfName.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfPixelType.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfVersion.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfXdr.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfConvert.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfPreviewImage.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfPreviewImageAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfChromaticities.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfChromaticitiesAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfKeyCode.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfKeyCodeAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTimeCode.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTimeCodeAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfRational.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfRationalAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfFramesPerSecond.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfStandardAttributes.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfEnvmap.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfEnvmapAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfInt64.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfRgba.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTileDescription.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTileDescriptionAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTiledInputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTiledOutputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTiledRgbaFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfRgbaYca.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTestFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfThreading.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfB44Compressor.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfStringVectorAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfMultiView.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfAcesFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfMultiPartOutputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfGenericOutputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfMultiPartInputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfGenericInputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfPartType.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfPartHelper.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfOutputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTiledOutputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfInputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfTiledInputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepScanLineInputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepScanLineInputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepTiledInputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepTiledInputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepTiledOutputFile.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepTiledOutputPart.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepFrameBuffer.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepCompositing.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfCompositeDeepScanLine.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfNamespace.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfMisc.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepImageState.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfDeepImageStateAttribute.h"
    "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/openexr-src/OpenEXR/IlmImf/ImfFloatVectorAttribute.h"
    )
endif()

