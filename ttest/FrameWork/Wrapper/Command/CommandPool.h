/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 11:53:44
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-21 11:37:12
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Command\CommandPool.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
 #pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Device.h"
#include "FrameWork/Wrapper/Base/Component.hpp"

namespace MoChengEngine::FrameWork::Wrapper {
class CommandPool : public Component<VkCommandPool, CommandPool> {
private:
 
  int queue_family_index;

public:
  CommandPool(Device::Ptr  device,VkQueueFlags  queue_flag,
              VkCommandPoolCreateFlagBits flag =
                  VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
  ~CommandPool();
  [[nodiscard]] auto Get_queue_family_index(){return queue_family_index;}
 
};

} // namespace MoChengEngine::FrameWork::Wrapper