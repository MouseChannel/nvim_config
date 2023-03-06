/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-21 17:37:57
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-21 11:13:19
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Pipeline\Pipeline_state.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
#include "FrameWork/Wrapper/RenderPass.h"
namespace MoChengEngine::FrameWork::Wrapper {

class PipelineState {
  struct VertexInput {
    std::vector<VkVertexInputBindingDescription> vertexbindindDes;
    std::vector<VkVertexInputAttributeDescription> attributeDes;
  };

private:
  std::vector<VkViewport> m_Viewports{};
  std::vector<VkRect2D> m_Scissors{};
  std::vector<VkVertexInputBindingDescription> m_vertexbindingDes;
  std::vector<VkVertexInputAttributeDescription> m_attributeDes;

public:
  VkPipelineVertexInputStateCreateInfo m_VertexInputInfo{};
  void Make_VertexInput_Info(
      std::vector<VkVertexInputBindingDescription> &vertexbindindDes,
      std::vector<VkVertexInputAttributeDescription> &attributeDes);

  VkPipelineInputAssemblyStateCreateInfo m_InputAssembly{};
  void Make_AssemblyInput_Info();

  VkPipelineViewportStateCreateInfo m_Viewport{};
  void Make_ViewPort_Info();
  VkPipelineRasterizationStateCreateInfo m_Rasterizer{};
  void Make_Raster_Info();
  VkPipelineMultisampleStateCreateInfo m_Multisampling{};
  void Make_MultiSample_Info(VkSampleCountFlagBits samples);
  std::vector<VkPipelineColorBlendAttachmentState> m_BlendAttachments{};
  void Make_BlendAttachment_Info();
  VkPipelineColorBlendStateCreateInfo m_BlendState{};
  void Make_BlendState_Info();
  VkPipelineDepthStencilStateCreateInfo m_DepthStencilState{};
  void Make_DepthStecil_Info();
  VkPipelineLayoutCreateInfo m_LayoutState{};
  void Make_LayoutCreate_Info(VkDescriptorSetLayout &layout);
  void Set_Viewports_and_Scissors(std::vector<VkViewport> viewports,
                                  std::vector<VkRect2D> scissors);
  RenderPass::Ptr m_renderpass{nullptr};
  void Set_renderpass(RenderPass::Ptr renderpass);

public:
  PipelineState();
  ~PipelineState();
  void fill_default();
  [[nodiscard]] auto &Get_VertexInput() { return m_VertexInputInfo; }
  [[nodiscard]] auto &Get_InputAssembly() { return m_InputAssembly; }
  [[nodiscard]] auto &Get_Viewport() { return m_Viewport; }
  [[nodiscard]] auto &Get_Rasterization() { return m_Rasterizer; }
  [[nodiscard]] auto &Get_Muti_Sampler() { return m_Multisampling; }

  [[nodiscard]] auto &Get_DepthStencil() { return m_DepthStencilState; }
  [[nodiscard]] auto &Get_Color_Blend() { return m_BlendState; }
  [[nodiscard]] auto &Get_Layout() { return m_LayoutState; }
  [[nodiscard]] auto &Get_renderpass() { return m_renderpass; }
};
} // namespace MoChengEngine::FrameWork::Wrapper