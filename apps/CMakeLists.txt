# Copyright 2019 Chris Fontas. All rights reserved.
# Use of this source code is governed by the license that can be
# found in the LICENSE file.

add_executable(app main.cpp)
target_compile_features(app PRIVATE cxx_std_11)

target_link_libraries(app PRIVATE vk_wrappers glslang)
