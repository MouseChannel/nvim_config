/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-22 10:32:45
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-23 08:16:58
 * @FilePath: \MoChengEngine\FrameWork\Core\Rendering\Graphic_pipeline.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "Graphic_pipeline.h"
#include "FrameWork/Wrapper/Pipeline/Pipeline.h"
namespace MoChengEngine::FrameWork::Core::Rendering {
GraphicPipeline::GraphicPipeline(
    Wrapper::Device::Ptr device,
    std::vector<VkVertexInputBindingDescription> &vertexbindindDes,
    std::vector<VkVertexInputAttributeDescription> &attributeDes,
    Wrapper::RenderPass::Ptr renderpass,
    VkDescriptorSetLayout &descriptorSet_layout, VkSampleCountFlagBits samples)
    : Wrapper::Pipeline{device} {
  pipeline_state->fill_default();
  pipeline_state->Set_renderpass(renderpass);
  pipeline_state->Make_LayoutCreate_Info(descriptorSet_layout);
  pipeline_state->Make_MultiSample_Info(samples);
  pipeline_state->Make_VertexInput_Info(vertexbindindDes, attributeDes);
}
GraphicPipeline::~GraphicPipeline() {}

void GraphicPipeline::Build_pipeline(
    std::vector<VkPipelineShaderStageCreateInfo> &shaders_stage) {


  VK_CHECK_SUCCESS(vkCreatePipelineLayout(m_device->Get_handle(),
                                          &pipeline_state->Get_Layout(),
                                          nullptr, &m_layout),
                   "Error: failed to create pipeline layout");
  VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
  pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

  pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaders_stage.size());
  pipelineCreateInfo.pStages = shaders_stage.data();
  auto asdas = pipeline_state->Get_VertexInput();
  pipelineCreateInfo.pVertexInputState = &pipeline_state->Get_VertexInput();
  pipelineCreateInfo.pInputAssemblyState = &pipeline_state->Get_InputAssembly();
  pipelineCreateInfo.pViewportState = &pipeline_state->Get_Viewport();
  pipelineCreateInfo.pRasterizationState = &pipeline_state->Get_Rasterization();
  pipelineCreateInfo.pMultisampleState = &pipeline_state->Get_Muti_Sampler();
  pipelineCreateInfo.pDepthStencilState = &pipeline_state->Get_DepthStencil();
  // stencil
  pipelineCreateInfo.pColorBlendState = &pipeline_state->Get_Color_Blend();

  pipelineCreateInfo.layout = m_layout;
  pipelineCreateInfo.renderPass =
      pipeline_state->Get_renderpass()->Get_handle();
  pipelineCreateInfo.subpass = 0;

    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;
  if (m_handle != VK_NULL_HANDLE) {
    vkDestroyPipeline(m_device->Get_handle(), m_handle, nullptr);
  }

  VK_CHECK_SUCCESS(
      vkCreateGraphicsPipelines(m_device->Get_handle(), VK_NULL_HANDLE, 1,
                                &pipelineCreateInfo, nullptr, &m_handle),
      "Error:failed to create pipeline")
}
} // namespace MoChengEngine::FrameWork::Core::Rendering