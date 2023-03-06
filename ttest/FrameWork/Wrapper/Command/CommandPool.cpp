/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 11:53:44
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-09 15:58:44
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Command\CommandPool.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "CommandPool.h"
#include "FrameWork/Wrapper/Device.h"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {

CommandPool::CommandPool(Device::Ptr  device, VkQueueFlags  queue_flags,
                         VkCommandPoolCreateFlagBits flag)
    : Component<VkCommandPool, CommandPool>{device},
      queue_family_index{device->Get_queue_family_index_by_flag(queue_flags)} {

  VkCommandPoolCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  createInfo.queueFamilyIndex = queue_family_index;

  createInfo.flags = flag;
  VK_CHECK_SUCCESS(vkCreateCommandPool(m_device->Get_handle(), &createInfo,
                                       nullptr, &m_handle),
                   "create commandPool failed");
}

CommandPool::~CommandPool() {

  vkDestroyCommandPool(m_device->Get_handle(), m_handle, nullptr);
}

} // namespace MoChengEngine::FrameWork::Wrapper