/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-14 21:56:24
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-13 16:13:33
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\RenderPass.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "Device.h"
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Component.hpp"
#include "FrameWork/Wrapper/RenderPass.h"
#include <memory>

namespace MoChengEngine::FrameWork::Wrapper {
class SubPass;
class RenderPass : public Component<VkRenderPass, RenderPass> {
private:
  std::vector<std::shared_ptr<SubPass>> mSubPasses;
  std::vector<VkSubpassDependency> mDependencies;
  // need to sync index in frameBuffer attachments;
  std::vector<VkAttachmentDescription> mAttachmentDescriptions;

public:
  RenderPass(Device::Ptr device);
  ~RenderPass();
  void AddSubPass(const SubPass &subpass,
                  const VkSubpassDependency &dependency) {
    mSubPasses.push_back(std::make_unique<SubPass>(subpass));
    mDependencies.push_back(dependency);
  }

  void Set_attachment_description(
      std::vector<VkAttachmentDescription> &attachmentDes) {
    mAttachmentDescriptions = attachmentDes;
  }
  [[nodiscard]] auto &Get_subpasses() { return mSubPasses; }
  void BuildRenderPass();
};

class SubPass {
private:
  VkSubpassDescription m_SubPassDescription{};
  // AttachmentReference代表的是在RenderPassAttachments数组里面的下标
  // 该pass的输出
  VkAttachmentReference m_ColorAttachmentReference{};
  std::vector<VkAttachmentReference> m_ColorAttachmentReferences{};

  // 上pass的输出，即该pass的输入
  VkAttachmentReference m_InputAttachmentReference{};
  std::vector<VkAttachmentReference> m_InputAttachmentReferences{};
  VkAttachmentReference m_DepthStencilAttachmentReference{};
  VkAttachmentReference m_ResolvedAttachmentReference{};
  VkSubpassDependency m_dependence{};

public:
  enum attachment_type {
    ColorAttachment = 0,
    ResolveAttachment = 1,
    DepthStencilAttachment = 2,
    InputAttachment = 3

  };
  SubPass() = default;
  ~SubPass() = default;

  void Add_attachment_reference(const VkAttachmentReference ref,
                                attachment_type type) {
    switch (type) {
    case Wrapper::SubPass::ColorAttachment:
      m_ColorAttachmentReference = ref;
      //   m_ColorAttachmentReferences.push_back(ref);
      break;
    case Wrapper::SubPass::DepthStencilAttachment:
      m_DepthStencilAttachmentReference = ref;
      break;
    case Wrapper::SubPass::InputAttachment:
      m_InputAttachmentReference = ref;
      //   m_InputAttachmentReferences.push_back(ref);
      break;
    case Wrapper::SubPass::ResolveAttachment:
      m_ResolvedAttachmentReference = ref;
      break;
    }
  }
  void Set_dependence(VkSubpassDependency dependence) {
    m_dependence = dependence;
  }
  void Build_SubPass_Description() {
    // if (m_ColorAttachmentReferences.empty()) {
    //   throw std::runtime_error("Error: color attachment group is empty!");
    // }
    m_SubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    m_SubPassDescription.colorAttachmentCount = 1;

    m_SubPassDescription.pColorAttachments = &m_ColorAttachmentReference;

    // m_SubPassDescription.pColorAttachments = &m_ResolvedAttachmentReference;

    m_SubPassDescription.inputAttachmentCount = 0;
    // static_cast<uint32_t>(m_InputAttachmentReferences.size());
    m_SubPassDescription.pInputAttachments = nullptr;
    // m_InputAttachmentReferences.data();
    m_SubPassDescription.pResolveAttachments =
    &m_ResolvedAttachmentReference;

    m_SubPassDescription.pDepthStencilAttachment =
       &m_DepthStencilAttachmentReference;
  }

  [[nodiscard]] auto GetSubPassDescription() const {
    return m_SubPassDescription;
  }
};
} // namespace MoChengEngine::FrameWork::Wrapper