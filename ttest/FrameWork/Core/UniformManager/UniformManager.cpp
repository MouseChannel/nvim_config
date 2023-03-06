/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-29 19:22:34
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 19:57:09
 * @FilePath: \MoChengEngine\FrameWork\Core\UniformManager\UniformManager.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "UniformManager.h"
#include "FrameWork/Core/UniformManager/UniformManager.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
namespace MoChengEngine::FrameWork {
void UniformManager::Prepare(const Wrapper::Device::Ptr device,
                             const COMMAND command, int frame_count) {
  m_Device = device;
  auto vpParam = Wrapper::UniformParameter::create();
  vpParam->mBinding = 0;
  vpParam->mCount = 1;
  vpParam->mDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  vpParam->mSize = sizeof(VPMatrices);
  vpParam->mStage = VK_SHADER_STAGE_VERTEX_BIT;

  for (int i = 0; i < frame_count; ++i) {
    auto buffer = Wrapper::Buffer::CreateR(
        device, vpParam->mSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    // buffer->Update(nullptr, vpParam->mSize);

    vpParam->m_Buffers.push_back(buffer);
  }

  m_UniformParams.push_back(vpParam);

  auto objectParam = Wrapper::UniformParameter::create();
  objectParam->mBinding = 1;
  objectParam->mCount = 1;
  objectParam->mDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  objectParam->mSize = sizeof(ObjectUniform);
  objectParam->mStage = VK_SHADER_STAGE_VERTEX_BIT;

  for (int i = 0; i < frame_count; ++i) {

    auto buffer = Wrapper::Buffer::CreateR(
        device, objectParam->mSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    // buffer->Update(nullptr, objectParam->mSize);
    objectParam->m_Buffers.push_back(buffer);
  }

  m_UniformParams.push_back(objectParam);

  auto textureParam = Wrapper::UniformParameter::create();
  textureParam->mBinding = 2;
  textureParam->mCount = 1;
  textureParam->mDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  textureParam->mStage = VK_SHADER_STAGE_FRAGMENT_BIT;
  textureParam->mTexture = std::make_unique<Texture>(
      m_Device, command, "D:/MoChengEngine/assets\\jqm.png");

  m_UniformParams.push_back(textureParam);

  m_DescriptorSetLayout = Wrapper::DescriptorSetLayout::Create(device);
  m_DescriptorSetLayout->Build(m_UniformParams);

  m_DescriptorPool =
      Wrapper::DescriptorPool::Create(m_Device, m_UniformParams, frame_count);

  m_DescriptorSet = Wrapper::DescriptorSet::Create(
      device, m_UniformParams, m_DescriptorSetLayout, m_DescriptorPool,
      frame_count);
}
void UniformManager::Update(const VPMatrices &vpMatrices,
                            const ObjectUniform &objectUniform,
                            const int  frameCount) {
                            

  m_UniformParams[0]->m_Buffers[frameCount]->Update((void *)(&vpMatrices),
                                                    sizeof(VPMatrices));

  // update object uniform
  m_UniformParams[1]->m_Buffers[frameCount]->Update((void *)(&objectUniform),
                                                    sizeof(ObjectUniform));
}

} // namespace MoChengEngine::FrameWork