/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-14 21:57:15
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-13 11:06:28
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\RenderPass.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "RenderPass.h"
#include "FrameWork/Wrapper/RenderPass.h"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {
RenderPass::RenderPass(Device::Ptr device)
    : Component<VkRenderPass, RenderPass>{device} {}
RenderPass::~RenderPass() {

  vkDestroyRenderPass(m_device->Get_handle(), m_handle, nullptr);
}

void RenderPass::BuildRenderPass() {

  if (mSubPasses.empty() || mAttachmentDescriptions.empty() ||
      mDependencies.empty()) {
    throw std::runtime_error("Error: not enough elements to build renderPass");
  }

  // unwrap
  std::vector<VkSubpassDescription> subPasses{};
  for (int i = 0; i < mSubPasses.size(); ++i) {
    subPasses.push_back(mSubPasses[i]->GetSubPassDescription());
  }

  VkRenderPassCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

  createInfo.attachmentCount =
      static_cast<uint32_t>(mAttachmentDescriptions.size());
  createInfo.pAttachments = mAttachmentDescriptions.data();

  createInfo.dependencyCount = static_cast<uint32_t>(mDependencies.size());
  createInfo.pDependencies = mDependencies.data();

  createInfo.subpassCount = static_cast<uint32_t>(subPasses.size());
  createInfo.pSubpasses = subPasses.data();

  VK_CHECK_SUCCESS(vkCreateRenderPass(m_device->Get_handle(), &createInfo,
                                      nullptr, &m_handle),
                   "Error: failed to create renderPass");
}
} // namespace MoChengEngine::FrameWork::Wrapper