// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {

// Wrapper class for vk::Sampler.
class Sampler {
   public:
    static SamplerPtr create(LogicalDevicePtr device, bool lerp = true, bool unnormalized = false);
    ~Sampler();
    
    const vk::Sampler& vk() const { return sampler_; }

    bool lerp() const { return lerp_; }

    bool unnormalized() const { return unnormalized_; }

   private:
    explicit Sampler(LogicalDevicePtr device, bool lerp, bool unnormalized);

    LogicalDeviceWeakPtr device_;
    vk::Sampler sampler_;
    bool lerp_;
    bool unnormalized_;
};

}  // namespace gfx