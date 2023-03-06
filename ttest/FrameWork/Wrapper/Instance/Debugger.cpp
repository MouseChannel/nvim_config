/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 18:13:45
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 13:27:07
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Instance\Debugger.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

#include "Debugger.h"
#include "vulkan/vulkan_core.h"

namespace MoChengEngine::FrameWork::Wrapper {
Debugger::Debugger(Instance &instance) : m_instance{instance} {

  VkDebugUtilsMessengerCreateInfoEXT createInfo = {};

  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity =
        // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

  createInfo.messageType =
   VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  createInfo.pfnUserCallback = debugCallBack;
  createInfo.pUserData = nullptr;

  if (CreateDebugUtilsMessengerEXT(instance.Get_handle(), &createInfo, nullptr,
                                   &m_handle) != VK_SUCCESS) {
    throw std::runtime_error("Error:failed to create debugger");
  }
  std::cout << "vaild here" << std::endl;
}

Debugger::~Debugger() {
  DestroyDebugUtilsMessengerEXT(m_instance.Get_handle(), m_handle, nullptr);
}

VKAPI_ATTR VkBool32 VKAPI_CALL Debugger::debugCallBack(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pMessageData, void *pUserData) {

  std::cout << "ValidationLayer: " << pMessageData->pMessage << std::endl
            << std::endl;

  return VK_FALSE;
}
VkResult Debugger::CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *debugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, debugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void Debugger::DestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");

  if (func != nullptr) {
    return func(instance, debugMessenger, pAllocator);
  }
}

void Debugger::SetupDebugger() {
  // if (!mEnableValidationLayer) { return; }
}
} // namespace MoChengEngine::FrameWork::Wrapper