/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-29 19:22:31
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 11:03:49
 * @FilePath: \MoChengEngine\FrameWork\Core\UniformManager\UniformManager.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Command/CommandQueue.h"
#include "FrameWork/Wrapper/DescriptorSetLayout.h"
#include "FrameWork/Wrapper/descriptorPool.h"
#include "FrameWork/Wrapper/descriptorSet.h"
#include "description.h"
#include <functional>

namespace MoChengEngine::FrameWork {
struct VPMatrices {
  glm::mat4 mViewMatrix;
  glm::mat4 mProjectionMatrix;

  VPMatrices() {
    mViewMatrix = glm::mat4(1.0f);
    mProjectionMatrix = glm::mat4(1.0f);
  }
};

struct ObjectUniform {
  glm::mat4 mModelMatrix;

  ObjectUniform() { mModelMatrix = glm::mat4(1.0f); }
};

class UniformManager {

private:
  Wrapper::Device::Ptr m_Device{nullptr};
  std::vector<Wrapper::UniformParameter::Ptr> m_UniformParams;

  Wrapper::DescriptorSetLayout::Ptr m_DescriptorSetLayout{nullptr};
  Wrapper::DescriptorPool::Ptr m_DescriptorPool{nullptr};
  Wrapper::DescriptorSet::Ptr m_DescriptorSet;

public:
  UniformManager() = default;
  ~UniformManager() = default;
  void Prepare(const Wrapper::Device::Ptr device, const COMMAND command,
               int frame_count);
  void Update(const VPMatrices &vpMatrices, const ObjectUniform &objectUniform,
              const int frameCount);
  [[nodiscard]] auto &GetDescriptorLayout() const {
    return m_DescriptorSetLayout;
  }

  [[nodiscard]] auto &GetDescriptorSet() const {
    return m_DescriptorSet->Get_handle();
  }
};

} // namespace MoChengEngine::FrameWork