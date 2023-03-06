
/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 10:15:15
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2023-01-02 20:52:11
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Device.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

// #define VMA_IMPLEMENTATION
#include "Device.h"
#include "FrameWork/Wrapper/Command/CommandPool.h"
// #include "vma/vk_mem_alloc.h"

#include "FrameWork/Base/vmaExporter.cpp"
#include "FrameWork/Wrapper/Command/CommandQueue.h"
#include "FrameWork/Wrapper/Device.h"
#include "FrameWork/Wrapper/Instance/Instance.h"
#include "FrameWork/Wrapper/PhysicalDevice.h"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <stdexcept>

// #include "vma/vk_mem_alloc.h"
namespace MoChengEngine::FrameWork::Wrapper {

Device::Device(Instance::Ptr instance, PhysicalDevice::Ptr gpu,
               WindowSurface::Ptr surface)
    : m_instance{instance}, m_gpu(gpu), m_surface(surface) {

  //   std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  //   // queueFamilies.insert(m_GraphicQueueFamily.value());
  //   // queueFamilies.insert(m_PresentQueueFamily.value() );

  //   float queuePriority = 1.0;

  //   for (int i = 0; i < m_gpu->Get_queueFamilies().size(); i++) {

  //     VkDeviceQueueCreateInfo queueCreateInfo = {};
  //     queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  //     queueCreateInfo.queueFamilyIndex = i;
  //     queueCreateInfo.queueCount = 1;
  //     queueCreateInfo.pQueuePriorities = &queuePriority;

  //     queueCreateInfos.push_back(queueCreateInfo);
  //   }
  auto command_queue_create_info = MakeCommandQueueCreateInfo();
  VkPhysicalDeviceFeatures deviceFeatures = {};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo deviceCreateInfo = {};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pQueueCreateInfos = command_queue_create_info.data();
  deviceCreateInfo.queueCreateInfoCount =
      static_cast<uint32_t>(command_queue_create_info.size());
  deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
  deviceCreateInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceRequiredExtensions.size());
  deviceCreateInfo.ppEnabledExtensionNames = deviceRequiredExtensions.data();

  //   // layer��
  //   if (mInstance->getEnableValidationLayer()) {
  deviceCreateInfo.enabledLayerCount =
      static_cast<uint32_t>(validationLayers.size());
  deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
  //   } else {
  //     deviceCreateInfo.enabledLayerCount = 0;
  //   }
  //   deviceCreateInfo.enabledLayerCount = 0;

  VK_CHECK_SUCCESS(vkCreateDevice(m_gpu->Get_handle(), &deviceCreateInfo,
                                  nullptr, &m_handle),
                   "Error:failed to create logical device");

  CreateAllocator();
  FillCommandQueues();
}
Device::~Device() {

#ifdef Using_VMA
  vmaDestroyAllocator(allocator);
#endif // DEBUG

  std::cout << "free device" << std::endl;
  vkDestroyDevice(m_handle, nullptr);
}
void Device::CreateAllocator() {

  VmaAllocatorCreateInfo createInfo{};

  createInfo.physicalDevice = m_gpu->Get_handle();
  createInfo.device = Get_handle();
  createInfo.instance = m_instance->Get_handle();

  VK_CHECK_SUCCESS(vmaCreateAllocator(&createInfo, &allocator),
                   "Create vma allocator failed");
}
std::vector<VkDeviceQueueCreateInfo> Device::MakeCommandQueueCreateInfo() {

  auto queueFamilies = m_gpu->Get_queueFamilies();
  auto queue_family_properties_count = queueFamilies.size();
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::vector<std::vector<float>> queue_priorities(
      queue_family_properties_count);
  // 测试用
  queuePriority.resize(9999, 1.0);
  for (int family_index = 0; family_index < queue_family_properties_count;
       family_index++) {
    auto &current_family = queueFamilies[family_index];
    auto current_queue_count = current_family.queueCount;
    // queue_priorities[family_index].resize(current_queue_count, 1.0f);
    VkDeviceQueueCreateInfo queue_create_info{};

    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = family_index;
    queue_create_info.queueCount = current_queue_count;
    queue_create_info.pQueuePriorities = queuePriority.data();
    queueCreateInfos.push_back(queue_create_info);
    // queue_priorities[family_index].data();
  }
  return queueCreateInfos;
}

void Device::FillCommandQueues() {
  auto queueFamilies = m_gpu->Get_queueFamilies();

  command_queues.resize(queueFamilies.size());
  for (int family_index = 0; family_index < queueFamilies.size();
       family_index++) {
    auto current_family = queueFamilies[family_index];

    for (int queue_index = 0; queue_index < current_family.queueCount;
         queue_index++) {
      bool present_support =
          m_gpu->IsPresentSupport(family_index, m_surface->Get_handle());
      command_queues[family_index].emplace_back(
          CommandQueue::Create(m_handle, family_index, queue_index,
                               current_family, present_support));
    }
  }
}
CommandQueue::Ptr Device::Get_present_queue() {
  InitQueueFamilies(m_gpu->Get_handle());
  vkGetDeviceQueue(m_handle, queue_index, 0, &present_queue);
  present_queue_p =
      CommandQueue::CreateR(m_handle, (uint32_t)queue_index, 0,
                            m_gpu->Get_queueFamilies()[queue_index], true);
  return present_queue_p;
}

void Device::InitQueueFamilies(VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface->Get_handle(),
                                         &presentSupport);

    if (presentSupport) {
      if (queueFamily.queueCount > 0 &&
          (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
        queue_index = i;
      }
    }

    if (queue_index != -1) {
      break;
    }

    ++i;
  }
}

CommandQueue::Ptr Device::Get_suitable_graphics_queue() {
  for (auto &queues : command_queues) {
    if (queues[0]->PresentSupport()) {
      return queues[0];
    }
  }
  throw std::runtime_error("Failed to get suitable_graphics_queue");
}

CommandQueue::Ptr Device::Get_queue_by_flag(VkQueueFlags required_queue_flags,
                                            uint32_t queue_index) {
  for (auto &queue : command_queues) {
    VkQueueFlags queue_flags = queue[0]->Get_family_propertie().queueFlags;
    uint32_t queue_count = queue[0]->Get_family_propertie().queueCount;
    if (((queue_flags & required_queue_flags) == required_queue_flags) &&
        queue_index < queue_count) {
      return queue[0];
    }
  }
  throw std::runtime_error("Failed to get queue");
}
VkSampleCountFlagBits Device::getMaxUsableSampleCount() {
  VkSampleCountFlagBits res{};
  getMaxUsableSampleCount(res);
  return res;
}
void Device::getMaxUsableSampleCount(VkSampleCountFlagBits &res) {
  VkPhysicalDeviceProperties props{};
  vkGetPhysicalDeviceProperties(m_gpu->Get_handle(), &props);

  VkSampleCountFlags counts =
      std::min(props.limits.framebufferColorSampleCounts,
               props.limits.framebufferDepthSampleCounts);

  if (counts & VK_SAMPLE_COUNT_64_BIT) {
    res = VK_SAMPLE_COUNT_64_BIT;
  } else if (counts & VK_SAMPLE_COUNT_32_BIT) {
    res = VK_SAMPLE_COUNT_32_BIT;
  } else if (counts & VK_SAMPLE_COUNT_16_BIT) {
    res = VK_SAMPLE_COUNT_16_BIT;
  } else if (counts & VK_SAMPLE_COUNT_8_BIT) {
    res = VK_SAMPLE_COUNT_8_BIT;
  } else if (counts & VK_SAMPLE_COUNT_4_BIT) {
    res = VK_SAMPLE_COUNT_4_BIT;
  } else if (counts & VK_SAMPLE_COUNT_2_BIT) {
    res = VK_SAMPLE_COUNT_2_BIT;
  } else if (counts & VK_SAMPLE_COUNT_1_BIT) {
    res = VK_SAMPLE_COUNT_1_BIT;
  }
}
VkFormat Device::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                     VkImageTiling tiling,
                                     VkFormatFeatureFlags features) {
  for (auto format : candidates) {
    VkFormatProperties props;

    vkGetPhysicalDeviceFormatProperties(m_gpu->Get_handle(), format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    }

    if (tiling == VK_IMAGE_TILING_OPTIMAL &&
        (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("Error: can not find proper format");
}
} // namespace MoChengEngine::FrameWork::Wrapper