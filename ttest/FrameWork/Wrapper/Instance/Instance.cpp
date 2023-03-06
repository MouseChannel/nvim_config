/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 13:40:27
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-21 11:34:22
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Instance\Instance.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

#include "Instance.h"
#include "Debugger.h"
// #include "FrameWork/Wrapper/PhysicalDevice.h"
#include <memory>
#include <vulkan/vulkan.hpp>
namespace MoChengEngine::FrameWork::Wrapper {

Instance::Instance() {
  if (!CheckValidationLayerSupport()) {
    throw std::runtime_error("Error: validation layer is not supported");
  }
  PrintAvailableExtensions();

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "mochengVK";

  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No ENGINE";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info{};

  auto extensions = GetRequiredExtensions();
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledLayerCount =
      static_cast<uint32_t>(validationLayers.size());
  create_info.ppEnabledLayerNames = validationLayers.data();
// vk::createInstance()
  VK_CHECK_SUCCESS(vkCreateInstance(&create_info, nullptr, &m_handle),
                   "create Instance failed");
  m_debugger = std::make_unique<Debugger>(*this);
  QueryGPUs();
}

Instance::~Instance() {
  if (m_debugger) {
    m_debugger.reset();
  }

  vkDestroyInstance(m_handle, nullptr);
}

bool Instance::CheckValidationLayerSupport() {
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const auto &layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProp : availableLayers) {
      if (std::strcmp(layerName, layerProp.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

void Instance::PrintAvailableExtensions() {
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());

  std::cout << "Available extensions:" << std::endl;

  for (const auto &extension : extensions) {
    std::cout << extension.extensionName << std::endl;
  }
}
std::vector<const char *> Instance::GetRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;

  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  return extensions;
}

void Instance::QueryGPUs() {
  uint32_t physical_device_count{0};
  VK_CHECK_SUCCESS(
      vkEnumeratePhysicalDevices(m_handle, &physical_device_count, nullptr),
      "EnumeratePhysicalDevice fail");
  if (physical_device_count < 1) {
    throw std::runtime_error(
        "Couldn't find a physical device that supports Vulkan.");
  }
  std::vector<VkPhysicalDevice> physical_devices;
  physical_devices.resize(physical_device_count);
  VK_CHECK_SUCCESS(vkEnumeratePhysicalDevices(m_handle, &physical_device_count,
                                              physical_devices.data()),
                   "EnumeratePhysicalDevice fail");

  // Create gpus wrapper objects from the VkPhysicalDevice's
  for (auto &physical_device : physical_devices) {
    gpus.emplace_back(PhysicalDevice::Create(physical_device));
  }
}
} // namespace MoChengEngine::FrameWork::Wrapper