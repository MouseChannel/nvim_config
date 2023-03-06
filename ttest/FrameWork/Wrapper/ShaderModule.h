/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-19 14:59:53
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-30 13:03:20
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\ShaderModule.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Component.hpp"
#include "FrameWork/Wrapper/Device.h"

namespace MoChengEngine::FrameWork::Wrapper {
class ShaderModule : public Component<VkShaderModule, ShaderModule> {
private:
  std::string m_EntryPoint;
  VkShaderStageFlagBits m_ShaderStage;

public:
  ShaderModule(Device::Ptr device, const std::string &fileName,
               VkShaderStageFlagBits shaderStage,
               const std::string &entryPoint);
  ~ShaderModule();
  
  VkPipelineShaderStageCreateInfo Make_Createinfo_in_pipeline() ;
};
} // namespace MoChengEngine::FrameWork::Wrapper
