/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-28 10:38:15
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-29 19:39:55
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\DescriptorSetLayout.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "Device.h"
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Component.hpp"

#include "FrameWork/Core/UniformManager/description.h"

namespace MoChengEngine::FrameWork::Wrapper {
class DescriptorSetLayout
    : public Component<VkDescriptorSetLayout, DescriptorSetLayout> {
private:
public:
  DescriptorSetLayout(Device::Ptr device);
  ~DescriptorSetLayout();
  void Build(std::vector<UniformParameter::Ptr> params);
};
} // namespace MoChengEngine::FrameWork::Wrapper