/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-19 16:36:20
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-30 09:45:24
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\descriptorSet.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "DescriptorSetLayout.h"

#include "FrameWork/Wrapper/Base/Component.hpp"
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
#include "FrameWork/Core/UniformManager/description.h"
#include "descriptorPool.h"

namespace MoChengEngine::FrameWork::Wrapper {
class DescriptorSet : public Component<std::vector<VkDescriptorSet>, DescriptorSet> {
private:
  std::vector<VkDescriptorSet> m_DescriptorSets{};
public:
  DescriptorSet(Device::Ptr device, std::vector<UniformParameter::Ptr> params,
                  DescriptorSetLayout::Ptr layout,
                  DescriptorPool::Ptr pool, int frame_count);
  ~DescriptorSet();
  
};
} // namespace MoChengEngine::FrameWork::Wrapper