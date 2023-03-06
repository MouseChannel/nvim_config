/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-28 10:41:18
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-30 21:50:17
 * @FilePath: \MoChengEngine\FrameWork\Core\UniformManager\description.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Core/Texture/Texture.h"
#include "FrameWork/Wrapper/Buffer.h"

namespace MoChengEngine::FrameWork::Wrapper {

struct UniformParameter {
  using Ptr = std::shared_ptr<UniformParameter>;
  static Ptr create() { return std::make_shared<UniformParameter>(); }

  size_t mSize{0};
  // shader 当中的bindingID
  uint32_t mBinding{0};

  //
  uint32_t mCount{0};
  // 是buffer还是image
  VkDescriptorType mDescriptorType;
  // 在vertexShader中接受还是在fragmentShader里面接受
  VkShaderStageFlagBits mStage;

  std::vector<Buffer::Ptr> m_Buffers{};
  std::unique_ptr<Texture> mTexture{nullptr};
};

} // namespace MoChengEngine::FrameWork::Wrapper
