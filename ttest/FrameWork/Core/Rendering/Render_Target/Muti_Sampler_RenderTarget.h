/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-21 10:53:09
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-13 15:44:47
 * @FilePath: \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Muti_Sampler_RenderTarget.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Muti_Sampler_RenderTarget.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Muti_Sampler_RenderTarget.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Muti_Sampler_RenderTarget.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Muti_Sampler_RenderTarget.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Muti_Sampler_RenderTarget.h
 * \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Muti_Sampler_RenderTarget.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Core/Rendering/Render_Target/Render_target_base.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/RenderPass.h"
#include "Render_target_base.h"
#include "vulkan/vulkan_core.h"

namespace MoChengEngine::FrameWork::Core::Rendering {

class Muti_Sampler_RenderTarget : public RenderTarget {

public:
  Muti_Sampler_RenderTarget(Wrapper::Image::Ptr image_ptr,
                            VkAttachmentDescription attachments_description)
      : RenderTarget{image_ptr, attachments_description} {}
  //   ~Muti_Sampler_RenderTarget(){}

  void Set_Image_Layout(Wrapper::CommandBuffer::Ptr command_buffer) override {

    VkImageSubresourceRange regionMutiSample{};
    regionMutiSample.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    regionMutiSample.baseMipLevel = 0;
    regionMutiSample.levelCount = 1;
    regionMutiSample.baseArrayLayer = 0;
    regionMutiSample.layerCount = 1;

    image_ptr->SetImageLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                              regionMutiSample, command_buffer);
  }
  inline static const ConvertFunc DEFAULT_MUTI_CREATE_FUNC =
      [](Wrapper::Image::Ptr sampler_image,Wrapper::CommandBuffer::Ptr command_buffer) {

   VkImageSubresourceRange regionMutiSample{};
    regionMutiSample.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    regionMutiSample.baseMipLevel = 0;
    regionMutiSample.levelCount = 1;
    regionMutiSample.baseArrayLayer = 0;
    regionMutiSample.layerCount = 1;

    sampler_image->SetImageLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                              regionMutiSample, command_buffer);


        VkAttachmentDescription attachment_Des{};
        attachment_Des.format = sampler_image->Get_format();
        attachment_Des.samples =
            sampler_image->Get_device()->getMaxUsableSampleCount();
        attachment_Des.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment_Des.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment_Des.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment_Des.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_Des.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment_Des.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        return std::make_unique<Muti_Sampler_RenderTarget>(sampler_image,
                                                           attachment_Des);
      };
  inline static const Create_Image_Func DEFAULT_IMAGE_CREATE_FUNC =
      [](Wrapper::SwapChain::Ptr swapchain) {
        return Wrapper::Image::CreateR(
            swapchain->Get_device(), swapchain->Get_extent2D().width,
            swapchain->Get_extent2D().height, swapchain->Get_format(),
            VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            swapchain->Get_device()->getMaxUsableSampleCount(),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY);
      };
  void Init_Image_Layout() {}
  //   Create_ATTACHMENT_Reference CREATE_ATTACHMENT_REFERENCE_FUNC = [](int
  //   index) {
  //     VkAttachmentReference mutiAttachmentRef;
  //     mutiAttachmentRef.attachment = index;
  //     mutiAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  //     return mutiAttachmentRef;
  //   };
  [[nodiscard]] VkImageLayout Get_layout() override {
    return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  }
  [[nodiscard]] Wrapper::SubPass::attachment_type
  Get_attachment_type() override {
    return Wrapper::SubPass::ColorAttachment;
  }
  [[nodiscard]] VkAttachmentReference
  Get_Attachement_Reference(int index) override {
    VkAttachmentReference muti_AttachmentRef;
    muti_AttachmentRef.attachment = index;
    muti_AttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    return muti_AttachmentRef;
  }
};
} // namespace MoChengEngine::FrameWork::Core::Rendering