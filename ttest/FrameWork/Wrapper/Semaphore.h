/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-15 15:19:29
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-21 11:35:46
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Semaphore.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Component.hpp"
#include "vulkan/vulkan_core.h"
#include"Device.h"
namespace MoChengEngine::FrameWork::Wrapper {
class Semaphore : public Component<VkSemaphore, Semaphore> {
private:
  

public:

  Semaphore(Device::Ptr  device);
  ~Semaphore();
  
};
} // namespace MoChengEngine::FrameWork::Wrapper