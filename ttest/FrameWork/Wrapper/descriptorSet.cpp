/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-19 16:36:23
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-17 13:59:20
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\descriptorSet.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "FrameWork/Wrapper/descriptorSet.h"

namespace MoChengEngine::FrameWork::Wrapper {
DescriptorSet::DescriptorSet(Device::Ptr device,
                             std::vector<UniformParameter::Ptr> params,
                             DescriptorSetLayout::Ptr layout,
                             DescriptorPool::Ptr pool, int frame_count)
    : Component<std::vector<VkDescriptorSet>, DescriptorSet>{device} {
  std::vector<VkDescriptorSetLayout> layouts(frame_count, layout->Get_handle());

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = pool->Get_handle();
  allocInfo.descriptorSetCount = frame_count;
  allocInfo.pSetLayouts = layouts.data();

  m_handle.resize(frame_count);
  if (vkAllocateDescriptorSets(m_device->Get_handle(), &allocInfo,
                               m_handle.data()) != VK_SUCCESS) {
    throw std::runtime_error("Error: failed to allocate descriptor sets");
  }
  std::cout << m_handle.size() << std::endl;

  for (int i = 0; i < frame_count; ++i) {
    // descriptorSetWrite，将params的信息，写入buffer
    std::vector<VkWriteDescriptorSet> descriptorSetWrites{};
    for (const auto &param : params) {
      VkWriteDescriptorSet descriptorSetWrite{};
      descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorSetWrite.dstSet = m_handle[i];
      descriptorSetWrite.dstArrayElement = 0;
      descriptorSetWrite.descriptorType = param->mDescriptorType;
      descriptorSetWrite.descriptorCount = param->mCount;
      descriptorSetWrite.dstBinding = param->mBinding;

      if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
        auto buffer = param->m_Buffers[i];
        VkDescriptorBufferInfo descriptorBufferInfo{buffer->Get_handle(), 0,
                                                    buffer->Get_size()};

        // descriptorSetWrite.pBufferInfo = buffer.
        descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
      }

      if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
        descriptorSetWrite.pImageInfo = &param->mTexture->GetImageInfo();
      }

      descriptorSetWrites.push_back(descriptorSetWrite);
    }

    vkUpdateDescriptorSets(m_device->Get_handle(),
                           static_cast<uint32_t>(descriptorSetWrites.size()),
                           descriptorSetWrites.data(), 0, nullptr);
  }
}
DescriptorSet::~DescriptorSet() {}
} // namespace MoChengEngine::FrameWork::Wrapper