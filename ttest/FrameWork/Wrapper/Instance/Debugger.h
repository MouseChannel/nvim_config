/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 18:13:45
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-12 10:01:22
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Instance\Debugger.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
 
#include "../Component/ComponentBase.hpp"
#include "Instance.h"
 


namespace MoChengEngine::FrameWork::Wrapper {
class Instance;
class Debugger : public ComponentBase<VkDebugUtilsMessengerEXT> {
private:
  VkDebugUtilsMessengerEXT m_handle{VK_NULL_HANDLE};
  Instance &m_instance;

public:
  Debugger(Instance &instance);
  ~Debugger();

public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pMessageData,
                void *pUserData);
  static VkResult CreateDebugUtilsMessengerEXT(
      VkInstance instance,
      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
      const VkAllocationCallbacks *pAllocator,
      VkDebugUtilsMessengerEXT *debugMessenger);
  static void
  DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                VkDebugUtilsMessengerEXT debugMessenger,
                                const VkAllocationCallbacks *pAllocator);
  void SetupDebugger();

  [[nodiscard]] VkDebugUtilsMessengerEXT Get_handle() const{ return m_handle; }
};

} // namespace MoChengEngine::FrameWork::Wrapper