/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 13:40:19
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-19 14:14:41
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Instance\Instance.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/PhysicalDevice.h"

namespace MoChengEngine::FrameWork::Wrapper {
class Debugger;

class Instance : public WrapperBase<VkInstance, Instance> {
private:
//   VkInstance m_handle{VK_NULL_HANDLE};
  std::unique_ptr<Debugger> m_debugger{nullptr};
  std::vector<PhysicalDevice::Ptr> gpus;

public:
  Instance();
  ~Instance();
  Instance &operator=(const Instance &) = delete;

  Instance &operator=(Instance &&) = delete;
  Instance(const Instance &) = delete;

  Instance(Instance &&) = delete;
//   [[nodiscard]] VkInstance Get_handle() { return m_handle; }
  [[nodiscard]] auto &Get_gpus() { return gpus; }

private:
  bool CheckValidationLayerSupport();
  void PrintAvailableExtensions();
  std::vector<const char *> GetRequiredExtensions();
  void QueryGPUs();
};

} // namespace MoChengEngine::FrameWork::Wrapper