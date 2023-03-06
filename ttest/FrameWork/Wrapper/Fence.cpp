/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-15 15:24:55
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-12 20:48:49
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Fence.cpp
 * @Description: nullpt
 * 
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved. 
 */
#include "Fence.h"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {
Fence::Fence(Device::Ptr  device) : Component<VkFence, Fence>{device} {
  VkFenceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  vkCreateFence(m_device->Get_handle(), &createInfo, nullptr, &m_handle);
}
Fence::~Fence() { vkDestroyFence(m_device->Get_handle(), m_handle, nullptr); }

void Fence::Reset() { vkResetFences(m_device->Get_handle(), 1, &m_handle); }
void Fence::Wait(uint64_t timeout) {
  vkWaitForFences(m_device->Get_handle(), 1, &m_handle, VK_TRUE, timeout);
}

} // namespace MoChengEngine::FrameWork::Wrapper