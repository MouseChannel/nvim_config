/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-29 17:52:36
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-29 19:39:11
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\descriptorPool.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "FrameWork/Core/UniformManager/description.h"
#include "FrameWork/Wrapper/descriptorPool.h"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {
DescriptorPool::DescriptorPool(Device::Ptr device,
                               std::vector<UniformParameter::Ptr> &params,
                               int frame_count)
    : Component<VkDescriptorPool, DescriptorPool>{device} {

  int uniformBufferCount = 0;

  int textureCount = 0;
  for (const auto &param : params) {
    if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
      uniformBufferCount++;
    }
    if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
      textureCount++;
    // TODO
  }
  // 描述每一种uniform都有多少个
  // 即一个DescriptorSet有几个buffer，几个Texture
  std::vector<VkDescriptorPoolSize> poolSizes{};
  VkDescriptorPoolSize uniformBufferSize{};
  uniformBufferSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uniformBufferSize.descriptorCount = uniformBufferCount * frame_count;
  poolSizes.push_back(uniformBufferSize);

  VkDescriptorPoolSize textureSize{};
  textureSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  textureSize.descriptorCount =
      textureCount * frame_count; // 这边的size是指，有多少个descriptor
  poolSizes.push_back(textureSize);

  // 创建pool
  VkDescriptorPoolCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  createInfo.pPoolSizes = poolSizes.data();
  // 每一帧都有自己单独的descriptorSet,不然无法支持并行
  createInfo.maxSets = static_cast<uint32_t>(frame_count);
  if (vkCreateDescriptorPool(m_device->Get_handle(), &createInfo, nullptr,
                             &m_handle) != VK_SUCCESS) {
    throw std::runtime_error("Error: failed to create Descriptor pool");
  }
}
DescriptorPool::~DescriptorPool() {
  vkDestroyDescriptorPool(m_device->Get_handle(), m_handle, nullptr);
}
} // namespace MoChengEngine::FrameWork::Wrapper