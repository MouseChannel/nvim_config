/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-22 10:32:43
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-17 16:22:48
 * @FilePath: \MoChengEngine\FrameWork\Core\Rendering\Graphic_pipeline.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Device.h"
#include "FrameWork/Wrapper/Pipeline/Pipeline.h"
#include "FrameWork/Wrapper/RenderPass.h"
// #include "FrameWork/Wrapper/WrapperBase.hpp"
namespace MoChengEngine::FrameWork::Core::Rendering {
class GraphicPipeline : public Wrapper::Pipeline {
private:
public:
  GraphicPipeline(Wrapper::Device::Ptr device,
                  std::vector<VkVertexInputBindingDescription> &vertexbindindDes,
                  std::vector<VkVertexInputAttributeDescription>& attributeDes,
                  Wrapper::RenderPass::Ptr renderpass,
                  VkDescriptorSetLayout  &descriptorSet_layout,
                  VkSampleCountFlagBits samples);
  ~GraphicPipeline();
  void Build_pipeline(
      std::vector<VkPipelineShaderStageCreateInfo> &shaders_stage) override;
};
} // namespace MoChengEngine::FrameWork::Core::Rendering