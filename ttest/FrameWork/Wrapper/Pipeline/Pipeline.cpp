/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-19 14:05:26
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-11 10:47:25
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Pipeline\Pipeline.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "Pipeline.h"

#include "FrameWork/Wrapper/Image.h"
#include "FrameWork/Wrapper/Pipeline/Pipeline_state.h"
#include "FrameWork/Wrapper/ShaderModule.h"
#include "vulkan/vulkan_core.h"
#include <memory>

namespace MoChengEngine::FrameWork::Wrapper {
Pipeline::Pipeline(Device::Ptr device)
    : Component<VkPipeline, Pipeline>{device} {
        pipeline_state = std::make_unique<PipelineState>();
    }
Pipeline::~Pipeline() {
  vkDestroyPipelineLayout(m_device->Get_handle(), m_layout, nullptr);
  vkDestroyPipeline(m_device->Get_handle(), m_handle, nullptr);
}

} // namespace MoChengEngine::FrameWork::Wrapper