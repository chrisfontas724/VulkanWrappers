# CMake generated Testfile for 
# Source directory: C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src
# Build directory: C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(glslang-testsuite "bash" "-o" "igncr" "runtests" "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/localResults" "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/StandAlone/glslangValidator" "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-build/StandAlone/spirv-remap")
set_tests_properties(glslang-testsuite PROPERTIES  WORKING_DIRECTORY "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/Test/" _BACKTRACE_TRIPLES "C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/CMakeLists.txt;368;add_test;C:/Users/Chris/Desktop/Projects/VulkanWrappers/build2/glslang-src/CMakeLists.txt;0;")
subdirs("External")
subdirs("glslang")
subdirs("OGLCompilersDLL")
subdirs("StandAlone")
subdirs("SPIRV")
subdirs("hlsl")
subdirs("gtests")
