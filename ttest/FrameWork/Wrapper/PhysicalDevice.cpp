/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 13:20:09
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-21 11:35:13
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\PhysicalDevice.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

#include "PhysicalDevice.h"

#include "FrameWork/Wrapper/PhysicalDevice.h"
#include "vulkan/vulkan_core.h"
#include <stdexcept>

namespace MoChengEngine::FrameWork::Wrapper {
PhysicalDevice::PhysicalDevice(VkPhysicalDevice  physicalDevice) {
  m_handle = physicalDevice;
  vkGetPhysicalDeviceFeatures(m_handle, &m_features);
  vkGetPhysicalDeviceProperties(m_handle, &m_properties);
  vkGetPhysicalDeviceMemoryProperties(m_handle, &m_memory_properties);

  queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &queueFamilyCount,
                                           nullptr);

  queueFamilies.resize(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &queueFamilyCount,
                                           queueFamilies.data());
}
PhysicalDevice::~PhysicalDevice() {}

int PhysicalDevice::FindQueueFamilyIndex(VkQueueFlags bits) {
  for (int i = 0; i < queueFamilyCount; i++) {
    auto queueFamily = queueFamilies[i];
    if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & bits)) {
      return i;
    }
  }

  throw std::runtime_error("Cant find suitable QueueFamily");
}
uint32_t PhysicalDevice::FindMemoryType(uint32_t typeFilter,
                                        VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProps;
  vkGetPhysicalDeviceMemoryProperties(m_handle, &memProps);

  for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) &&
        ((memProps.memoryTypes[i].propertyFlags & properties) == properties)) {
      return i;
    }
  }

  throw std::runtime_error("Error: cannot find the property memory type!");
}

bool PhysicalDevice::IsPresentSupport(int queueFamilyIndex,
                                      VkSurfaceKHR surface) {
  VkBool32 presentSupport = VK_FALSE;
  vkGetPhysicalDeviceSurfaceSupportKHR(m_handle, queueFamilyIndex, surface,
                                       &presentSupport);
  return presentSupport;
}
bool PhysicalDevice::CheckSupport(VkQueueFlagBits bit) {
  for (auto &queueFamily : queueFamilies) {

    if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & bit)) {
      return true;
    }
   
  }
  return false;
}
} // namespace MoChengEngine::FrameWork::Wrapper