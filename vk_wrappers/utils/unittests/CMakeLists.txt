set(TEST_SOURCE
   ${TEST_SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/compiler_unittest.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/../shader_compiler.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/../reflection/spirv_reflect.c
   PARENT_SCOPE
)

include_directories("${gtest_SOURCE_DIR}/include")

