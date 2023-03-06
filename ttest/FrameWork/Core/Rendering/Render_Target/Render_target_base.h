/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 16:56:50
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-16 10:34:08
 * @FilePath: \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Render_target_base.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Core/ObjectBase.hpp"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Image.h"
#include "FrameWork/Wrapper/RenderPass.h"
#include "FrameWork/Wrapper/Swapchain.h"
#include "functional"

#include <algorithm>

namespace MoChengEngine::FrameWork::Core::Rendering {

/**
 * @description: contains three object，to bind framebuffer and renderpass
 * 1. image_handle
 * 2. image_view using in framebuffer
 * 3. attachment using in renderpass
 *
 *  actually only need 2 and 3
 */
class RenderTarget {
protected:
  VkExtent2D extent{0, 0};
  Wrapper::Image::Ptr image_ptr{};
  VkImage image{};
  VkImageView image_view{nullptr};

  VkAttachmentDescription attachment_description{};

public:
  /**
   * @description: convert a image Ptr to a complete render_target
   */
  using ConvertFunc = std::function<std::unique_ptr<RenderTarget>(
      Wrapper::Image::Ptr, Wrapper::CommandBuffer::Ptr)>;
  /**
   * @description: default convert swapchain images to final present images
   */
  static const ConvertFunc DEFAULT_CONVERT_FUNC;
  /**
   * @description: create a image Ptr
   */
  using Create_Image_Func =
      std::function<Wrapper::Image::Ptr(Wrapper::SwapChain::Ptr)>;

  using Create_ATTACHMENT_Reference = std::function<VkAttachmentReference(int)>;
  // virtual  const Create_ATTACHMENT_Reference
  // CREATE_ATTACHMENT_REFERENCE_FUNC;

  virtual VkAttachmentReference Get_Attachement_Reference(int index) = 0;
  RenderTarget(Wrapper::Image::Ptr image_ptr,
               VkAttachmentDescription attachments_description);

  static VkFormat FindSupportedFormat(const Wrapper::Device::Ptr &device,
                                      const std::vector<VkFormat> &candidates,
                                      VkImageTiling tiling,
                                      VkFormatFeatureFlags features);

  [[nodiscard]] auto Get_extent() { return extent; }
  [[nodiscard]] auto Get_image() { return image_ptr; }
  [[nodiscard]] auto &Get_image_view() { return image_view; }
  [[nodiscard]] auto Get_attachments_description() {
    return attachment_description;
  }
  virtual void Set_Image_Layout(Wrapper::CommandBuffer::Ptr command_buffer) = 0;
  virtual VkImageLayout Get_layout() = 0;
  virtual Wrapper::SubPass::attachment_type Get_attachment_type() = 0;
};
} // namespace MoChengEngine::FrameWork::Core::Rendering