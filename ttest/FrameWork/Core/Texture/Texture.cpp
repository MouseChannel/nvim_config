/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-29 18:50:26
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 13:47:36
 * @FilePath: \MoChengEngine\FrameWork\Core\Texture\Texture.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "Texture.h"
#include "FrameWork/Wrapper/Buffer.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "vulkan/vulkan_core.h"
// #include "FrameWork/Wrapper/Command/CommandPool.h"

namespace MoChengEngine::FrameWork {
Texture::Texture(Wrapper::Device::Ptr device, COMMAND command,
                 const std::string &path) {
  int texWidth, texHeight, texSize, texChannles;
  stbi_uc *pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannles,
                              STBI_rgb_alpha);
  std::cout << path.c_str() << std::endl;
  if (!pixels) {
    throw std::runtime_error("Error: failed to read image data");
  }

  texSize = texWidth * texHeight * 4;
  VkExtent3D extent{
                    .width = static_cast<uint32_t>(texWidth),.height = static_cast<uint32_t>(texHeight),
                    .depth = 1};
  mImage = Wrapper::Image::CreateR(
      device, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
      VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_SAMPLE_COUNT_1_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      VK_IMAGE_ASPECT_COLOR_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

  VkImageSubresourceRange region{};
  region.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

  region.baseArrayLayer = 0;
  region.layerCount = 1;

  region.baseMipLevel = 0;
  region.levelCount = 1;
  auto command_buffer = command.request_command_buffer();
//   convert image_layout to TRANSFER_DST to wait data arrive
 
  command_buffer->Begin();
 
  mImage->SetImageLayout(
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT, region, command_buffer);
  command_buffer->Wait(command.queue);
//   fill image data
  auto image_buffer = Wrapper::Buffer::Create_Image_buffer(device, texSize);
 
  command_buffer->Begin();
  mImage->FillImageData(texSize, (void *)pixels, image_buffer, command_buffer);
 

  command_buffer->Wait(command.queue);
//   convert image_layout to read_only
 
  command_buffer->Begin();
  mImage->SetImageLayout(
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, region, command_buffer);
 
  command_buffer->Wait(command.queue);

  stbi_image_free(pixels);

  mSampler = Wrapper::Sampler::Create(device);

  mImageInfo.imageLayout = mImage->Get_layout();
  mImageInfo.imageView = mImage->Get_view();
  mImageInfo.sampler = mSampler->Get_handle();
}

Texture::~Texture() {}

} // namespace MoChengEngine::FrameWork