/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 13:20:09
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2023-01-02 20:49:46
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\PhysicalDevice.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once

#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"

#include <vector>

namespace MoChengEngine::FrameWork::Wrapper {
class Instance;
class PhysicalDevice : public WrapperBase<VkPhysicalDevice, PhysicalDevice> {
private:
//   VkPhysicalDevice m_handle{VK_NULL_HANDLE};
  // The features that this GPU supports
  VkPhysicalDeviceFeatures m_features{};

  // The GPU properties
  VkPhysicalDeviceProperties m_properties{};

  // The GPU memory properties
  VkPhysicalDeviceMemoryProperties m_memory_properties;
  uint32_t queueFamilyCount;
  std::vector<VkQueueFamilyProperties> queueFamilies;

  bool CheckSupport(VkQueueFlagBits bit);

public:
  PhysicalDevice(VkPhysicalDevice  physicalDevice);
  ~PhysicalDevice();

  bool IsPresentSupport(int queueFamilyIndex, VkSurfaceKHR surface);
  int FindQueueFamilyIndex(VkQueueFlags bits);
  uint32_t FindMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
//   [[nodiscard]] VkPhysicalDevice Get_handle()const { return m_handle; }
  [[nodiscard]] auto  Get_queueFamilies() { return queueFamilies; }
};

} // namespace MoChengEngine::FrameWork::Wrapper