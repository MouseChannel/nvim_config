/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-21 10:59:50
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-17 18:34:14
 * @FilePath: \MoChengEngine\FrameWork\Core\Rendering\Render_Target\Depth_RenderTarget.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Swapchain.h"
#include "Render_target_base.h"

namespace MoChengEngine::FrameWork::Core::Rendering {

class Depth_RenderTarget : public RenderTarget {

public:
  Depth_RenderTarget(Wrapper::Image::Ptr image_ptr,
                     VkAttachmentDescription attachments_description)
      : RenderTarget{image_ptr, attachments_description} {}
  //   ~Depth_RenderTarget() {}

void
  Set_Image_Layout(Wrapper::CommandBuffer::Ptr command_buffer) override {

    VkImageSubresourceRange depth_region{};
    depth_region.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depth_region.baseMipLevel = 0;
    depth_region.levelCount = 1;
    depth_region.baseArrayLayer = 0;
    depth_region.layerCount = 1;

    image_ptr->SetImageLayout(
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, depth_region,
        command_buffer);
  }
  inline static const ConvertFunc DEFAULT_DEPTH_CREATE_FUNC =
      [](Wrapper::Image::Ptr depth_image, Wrapper::CommandBuffer::Ptr command_Buffer) {
        // convert image_layout

        VkImageSubresourceRange depth_region{};
        depth_region.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        depth_region.baseMipLevel = 0;
        depth_region.levelCount = 1;
        depth_region.baseArrayLayer = 0;
        depth_region.layerCount = 1;
        depth_image->SetImageLayout(
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, depth_region,
            command_Buffer);

        VkAttachmentDescription attachment_Des{};
        attachment_Des.format = depth_image->Get_format();
        attachment_Des.samples =
            depth_image->Get_device()->getMaxUsableSampleCount();
        attachment_Des.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment_Des.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_Des.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment_Des.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_Des.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment_Des.finalLayout =
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        return std::make_unique<Depth_RenderTarget>(depth_image,
                                                    attachment_Des);
      };

  inline static const Create_Image_Func DEFAULT_IMAGE_CREATE_FUNC =
      [](Wrapper::SwapChain::Ptr swapchain) {
        std::vector<VkFormat> formats = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT,
        };

        auto resultFormat = FindSupportedFormat(
            swapchain->Get_device(), formats, VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        return Wrapper::Image::CreateR(
            swapchain->Get_device(), swapchain->Get_extent2D().width,
            swapchain->Get_extent2D().height, resultFormat, VK_IMAGE_TYPE_2D,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            swapchain->Get_device()->getMaxUsableSampleCount(),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY);
      };
  // Create_ATTACHMENT_Reference CREATE_ATTACHMENT_REFERENCE_FUNC = [](int
  // index)
  // {
  //     VkAttachmentReference depthAttachmentRef;
  //     depthAttachmentRef.attachment = index;
  //     depthAttachmentRef.layout =
  //         VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  //     return depthAttachmentRef;
  // };
  [[nodiscard]] VkImageLayout Get_layout() override {
    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  }
  [[nodiscard]] Wrapper::SubPass::attachment_type
  Get_attachment_type() override {
    return Wrapper::SubPass::DepthStencilAttachment;
  }
  [[nodiscard]] VkAttachmentReference
  Get_Attachement_Reference(int index) override {
    VkAttachmentReference finalAttachmentRef{};
    finalAttachmentRef.attachment = index;
    finalAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    return finalAttachmentRef;
  }
};
} // namespace MoChengEngine::FrameWork::Core::Rendering