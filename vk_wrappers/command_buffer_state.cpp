// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/command_buffer_state.hpp"

namespace gfx {

void CommandBufferState::set_default_state(DefaultState state) {
    switch (state) {
        case DefaultState::kOpaque:
        case DefaultState::kWireFrame:
        case DefaultState::kTranslucent:
        case DefaultState::kCustomRaytrace:
        case DefaultState::kNVRaytrace:
            break;
    };
}

}  // namespace gfx
