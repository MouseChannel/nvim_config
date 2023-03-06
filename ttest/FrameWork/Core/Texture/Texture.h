/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-28 10:45:48
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-23 11:42:05
 * @FilePath: \MoChengEngine\FrameWork\Core\Texture\Texture.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once

#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Command/CommandPool.h"
#include "FrameWork/Wrapper/Device.h"
#include "FrameWork/Wrapper/Image.h"
#include "FrameWork/Wrapper/Sampler.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "tools/stb_image.h"
namespace MoChengEngine::FrameWork {

class Texture {

private:
  Wrapper::Device::Ptr mDevice{nullptr};
  Wrapper::Image::Ptr mImage{nullptr};
  Wrapper::Sampler::Ptr mSampler{nullptr};
  VkDescriptorImageInfo mImageInfo{};

public:
  Texture(Wrapper::Device::Ptr device, COMMAND command,
          const std::string &path);
  ~Texture();
  [[nodiscard]] auto &GetImageInfo() { return mImageInfo; }
};

} // namespace MoChengEngine::FrameWork