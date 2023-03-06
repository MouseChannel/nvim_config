/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-21 17:38:49
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-23 08:17:51
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Pipeline\Pipeline_state.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "Pipeline_state.h"
#include "FrameWork/Wrapper/Pipeline/Pipeline_state.h"
#include <utility>
namespace MoChengEngine::FrameWork::Wrapper {
PipelineState::PipelineState() {}
PipelineState::~PipelineState() {}

void PipelineState::Make_VertexInput_Info(
    std::vector<VkVertexInputBindingDescription> &vertexbindingDes,
    std::vector<VkVertexInputAttributeDescription> &attributeDes) {
  m_vertexbindingDes = vertexbindingDes;
  m_attributeDes = attributeDes;
  m_VertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  m_VertexInputInfo.vertexBindingDescriptionCount = m_vertexbindingDes.size();
  m_VertexInputInfo.pVertexBindingDescriptions = m_vertexbindingDes.data();
  m_VertexInputInfo.vertexAttributeDescriptionCount = m_attributeDes.size();
  m_VertexInputInfo.pVertexAttributeDescriptions = m_attributeDes.data();
}

void PipelineState::Make_AssemblyInput_Info() {
  m_InputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

  m_InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  m_InputAssembly.primitiveRestartEnable = VK_FALSE;
}

void PipelineState::Make_ViewPort_Info() {
  VkViewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)800;
  viewport.height = (float)800;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor = {};
  scissor.offset = {0, 0};
  scissor.extent = {800, 800};
  Set_Viewports_and_Scissors({viewport}, {scissor});
  m_Viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

  m_Viewport.viewportCount = static_cast<uint32_t>(m_Viewports.size());
  m_Viewport.pViewports = m_Viewports.data();
  m_Viewport.scissorCount = static_cast<uint32_t>(m_Scissors.size());
  m_Viewport.pScissors = m_Scissors.data();
}

void PipelineState::Make_Raster_Info() {
  m_Rasterizer.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  m_Rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // ����ģʽ��Ҫ����gpu����
  m_Rasterizer.lineWidth = 1.0f; // ����1��Ҫ����gpu����
  m_Rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  m_Rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  m_Rasterizer.depthBiasEnable = VK_FALSE;
  m_Rasterizer.depthBiasConstantFactor = 0.0f;
  m_Rasterizer.depthBiasClamp = 0.0f;
  m_Rasterizer.depthBiasSlopeFactor = 0.0f;
}

void PipelineState::Make_MultiSample_Info(VkSampleCountFlagBits samples) {
  m_Multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

  m_Multisampling.sampleShadingEnable = VK_FALSE;
  m_Multisampling.rasterizationSamples = samples;
  m_Multisampling.minSampleShading = 1.0f;
  m_Multisampling.pSampleMask = nullptr;
  m_Multisampling.alphaToCoverageEnable = VK_FALSE;
  m_Multisampling.alphaToOneEnable = VK_FALSE;
}

void PipelineState::Make_BlendAttachment_Info() {
  VkPipelineColorBlendAttachmentState blendAttachment{};

  blendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  blendAttachment.blendEnable = VK_FALSE;
  blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

  blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
  m_BlendAttachments.push_back(blendAttachment);
}

void PipelineState::Make_BlendState_Info() {
  m_BlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  m_BlendState.logicOpEnable = VK_FALSE;
  m_BlendState.logicOp = VK_LOGIC_OP_COPY;

  // ���blendAttachment��factor��operation
  m_BlendState.blendConstants[0] = 0.0f;
  m_BlendState.blendConstants[1] = 0.0f;
  m_BlendState.blendConstants[2] = 0.0f;
  m_BlendState.blendConstants[3] = 0.0f;
  m_BlendState.attachmentCount =
      static_cast<uint32_t>(m_BlendAttachments.size());
  m_BlendState.pAttachments = m_BlendAttachments.data();
}

void PipelineState::Make_LayoutCreate_Info(VkDescriptorSetLayout &layout) {
  m_LayoutState.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  m_LayoutState.setLayoutCount = 1;
  m_LayoutState.pSetLayouts = &layout;
  m_LayoutState.pushConstantRangeCount = 0;
  m_LayoutState.pPushConstantRanges = nullptr;
}
void PipelineState::Set_Viewports_and_Scissors(
      std::vector<VkViewport>  viewports,
      std::vector<VkRect2D>  scissors) {
  m_Viewports = viewports;
  m_Scissors = scissors;
}

void PipelineState::Make_DepthStecil_Info() {
  m_DepthStencilState.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

  m_DepthStencilState.depthTestEnable = VK_TRUE;
  m_DepthStencilState.depthWriteEnable = VK_TRUE;
  m_DepthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
}
void PipelineState::Set_renderpass(RenderPass::Ptr renderpass) {
  m_renderpass = renderpass;
}
void PipelineState::fill_default() {
  Make_AssemblyInput_Info();
  Make_ViewPort_Info();
  Make_Raster_Info();
  Make_BlendAttachment_Info();
  Make_BlendState_Info();
  Make_DepthStecil_Info();
}
} // namespace MoChengEngine::FrameWork::Wrapper