/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 10:26:21
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-30 21:33:10
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Command\CommandQueue.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"

#include "FrameWork/Wrapper/Component/ComponentBase.hpp"

#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
 
#include <stdint.h>
#include <vector>
namespace MoChengEngine::FrameWork::Wrapper {

class CommandQueue : public WrapperBase<VkQueue, CommandQueue> {
private:
  //   VkQueue m_handle;
  uint32_t family_index;
  uint32_t queue_index;
  bool presentSupport;
  VkQueueFamilyProperties familyProperties;

public:
  CommandQueue(VkDevice device, uint32_t family_index, uint32_t queue_index,
               VkQueueFamilyProperties familyProperties, bool presentSupport);
  ~CommandQueue();
  VkResult Submit(const std::vector<VkSubmitInfo> &submit_infos, VkFence fence);

  VkResult Present(VkPresentInfoKHR  &present_info);
  void Submit_and_wait(const std::vector<VkSubmitInfo> &submit_infos,
                       VkFence fence = VK_NULL_HANDLE);
  [[nodiscard]] auto &Get_family_propertie() { return familyProperties; }
  [[nodiscard]] bool PresentSupport() { return presentSupport; }
  [[nodiscard]] auto Get_family_index() { return family_index; }
};

} // namespace MoChengEngine::FrameWork::Wrapper