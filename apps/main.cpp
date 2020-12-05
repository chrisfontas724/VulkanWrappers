// Copyright 2019 Chris Fontas. All rights reserved.
// Use of this source code is governed by the license that can be
// found in the LICENSE file.

#include <vk_wrappers/instance.hpp>
#include <core/logging.hpp>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
  CXL_LOG(INFO) << "Hello, world!";
  return 0;
}