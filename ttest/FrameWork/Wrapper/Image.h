/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 17:00:17
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 12:43:57
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Image.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

#pragma once
#include "Device.h"
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Resource.hpp"
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
#include "FrameWork/Wrapper/Buffer.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "vulkan/vulkan_core.h"
#include <stdint.h>

namespace MoChengEngine::FrameWork::Wrapper {
class Image : public Resource<VkImage, Image> {
  using Command_Buffer_Task = CommandBuffer::Command_Buffer_Task;

private:
  VkImageView m_view{VK_NULL_HANDLE};

  VkFormat m_format{};
  VkImageLayout m_layout{VK_IMAGE_LAYOUT_UNDEFINED};
  VkImageType m_imageType{};

  VkExtent3D m_extent{};
  uint32_t m_width;
  uint32_t m_height;
  bool auto_destroy;
  VkDeviceMemory m_ImageMemory{VK_NULL_HANDLE};
  VkImageViewCreateInfo Make_View_Info(const VkImageAspectFlags aspectFlags);

public:
  Image(const Device::Ptr device, uint32_t width, uint32_t height,
        const VkFormat format, const VkImageType imageType,
        const VkImageTiling tiling, const VkImageUsageFlags usage,
        const VkSampleCountFlagBits sample,
        const VkMemoryPropertyFlags properties,
        const VkImageAspectFlags aspectFlags,
        const VmaMemoryUsage memory_usage, bool auto_destroy = false);
  Image(const Device::Ptr device, VkImage  image_handle,
        const VkExtent3D extent, VkFormat format,
        VkImageAspectFlags aspectFlags, bool auto_destroy = false);
  Image(Device::Ptr device, VkImage image_handle, VkImageView image_view,
          VkExtent3D extent, VkFormat format,
        VkImageAspectFlags aspectFlags, bool auto_destroy = false);
  ~Image();
  Image() = delete;
  VkImageView CreateView(VkImageViewCreateInfo viewInfo, Device::Ptr device);

  void FillImageData(size_t size, void *pData);
  // 使用barrier修改image格式
  void SetImageLayout(VkImageLayout newLayout,
                      VkPipelineStageFlags srcStageMask,
                      VkPipelineStageFlags dstStageMask,
                      VkImageSubresourceRange subresrouceRange,
                      CommandBuffer::Ptr commandBuffer);
  void FillImageData(size_t size, void *pData, Buffer::Ptr &buffer,
                     CommandBuffer::Ptr commandBuffer);

  [[nodiscard]] auto Get_view() const { return m_view; }
  //   [[nodiscard]] auto Get_extent() { return m_extent; }
  [[nodiscard]] auto Get_extent2D() {
    VkExtent2D extent{m_width, m_height};
    return extent;
  }
  [[nodiscard]] auto Get_format() { return m_format; }
  [[nodiscard]] auto Get_layout() { return m_layout; }
};
} // namespace MoChengEngine::FrameWork::Wrapper