/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-15 15:19:59
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-09 15:30:59
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Semaphore.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "Semaphore.h"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {
Semaphore::Semaphore(Device::Ptr  device) : Component<VkSemaphore, Semaphore>{device} {
  VkSemaphoreCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  VK_CHECK_SUCCESS(vkCreateSemaphore(m_device->Get_handle(), &createInfo,
                                     nullptr, &m_handle),
                   "Create semaphore failed");
}
Semaphore::~Semaphore() {
  vkDestroySemaphore(m_device->Get_handle(), m_handle, nullptr);
}
} // namespace MoChengEngine::FrameWork::Wrapper