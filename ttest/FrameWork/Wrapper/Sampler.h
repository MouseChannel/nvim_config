#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Component.hpp"
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {
class Sampler : public Component<VkSampler, Sampler> {
private:
   

public:
  Sampler(Device::Ptr device);
  ~Sampler();
  };
} // namespace MoChengEngine::FrameWork::Wrapper