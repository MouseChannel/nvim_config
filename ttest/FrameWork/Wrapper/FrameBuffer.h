/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-14 21:37:36
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-30 10:43:11
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\FrameBuffer.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "Device.h"
#include "FrameWork/Base/baseHeader.h"

#include "FrameWork/Wrapper/Base/Component.hpp"
#include "RenderPass.h"
#include "vulkan/vulkan_core.h"

namespace MoChengEngine::FrameWork::Wrapper {
class FrameBuffer : public Component<VkFrameBuffer, FrameBuffer> {
private:
 

public:
  FrameBuffer(Device::Ptr  device, VkExtent2D  extent,
              std::vector<VkImageView>  image_views,

              RenderPass::Ptr  renderPass);
  ~FrameBuffer();
};
} // namespace MoChengEngine::FrameWork::Wrapper