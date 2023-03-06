/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-28 10:49:37
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-28 10:54:11
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Sampler.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "Sampler.h"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {
Sampler::Sampler(Device::Ptr device) : Component<VkSampler, Sampler>{device} {

  VkSamplerCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  createInfo.magFilter = VK_FILTER_LINEAR;
  createInfo.minFilter = VK_FILTER_LINEAR;
  createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

  createInfo.anisotropyEnable = VK_TRUE;
  createInfo.maxAnisotropy = 16;

  createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

  createInfo.unnormalizedCoordinates = VK_FALSE;

  createInfo.compareEnable = VK_FALSE;
  createInfo.compareOp = VK_COMPARE_OP_ALWAYS;

  createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  createInfo.mipLodBias = 0.0f;
  createInfo.minLod = 0.0f;
  createInfo.maxLod = 0.0f;

  VK_CHECK_SUCCESS(
      vkCreateSampler(m_device->Get_handle(), &createInfo, nullptr, &m_handle),
      "Error: failed to create sampler");
}
Sampler::~Sampler() {
  vkDestroySampler(m_device->Get_handle(), m_handle, nullptr);
}
} // namespace MoChengEngine::FrameWork::Wrapper