/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-29 17:52:32
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-29 19:39:38
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\descriptorPool.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Component.hpp"
#include "FrameWork/Core/UniformManager/description.h"
namespace MoChengEngine::FrameWork::Wrapper {
class DescriptorPool : public Component<VkDescriptorPool, DescriptorPool> {
private:
public:
  DescriptorPool(Device::Ptr device, std::vector<UniformParameter::Ptr> &params,
                 int frame_count = 1);
  ~DescriptorPool();
};
} // namespace MoChengEngine::FrameWork::Wrapper