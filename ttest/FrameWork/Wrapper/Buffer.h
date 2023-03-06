/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-14 11:30:26
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-17 14:49:26
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Buffer.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/Resource.hpp"

#include "Device.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
// #define Using_VMA
namespace MoChengEngine::FrameWork::Wrapper {
class Buffer : public Resource<VkBuffer, Buffer> {
private:
  bool persistent;

  VmaAllocationInfo allocation_info{};
  bool mapped = false;
  VkDeviceMemory mBufferMemory{VK_NULL_HANDLE};
  VkDescriptorBufferInfo m_BufferInfo{};
  void *mapped_data{nullptr};
  VkDeviceSize size;

public:
  Buffer(Device::Ptr device, VkDeviceSize size, VkBufferUsageFlags buffer_usage,
         VmaMemoryUsage mem_usage, VkMemoryPropertyFlags properties,
         VmaAllocationCreateFlags flags = 0,
         const std::vector<uint32_t> &queue_family_indices =
             std::vector<uint32_t>());
  ~Buffer();
  [[nodiscard]] auto Get_Allocation_info() { return allocation_info; }
  [[nodiscard]] auto Get_Buffer_info() {
#ifdef Using_VMA
    return allocation_info;
#else
    return m_BufferInfo;
#endif
  }
  [[nodiscard]] auto Get_size() { return size; }
  static Ptr Create_GPU_Only_Buffer(Device::Ptr device, void *data,
                                    VkDeviceSize size,
                                    VkBufferUsageFlags buffer_usage,
                                    CommandBuffer::Ptr command_Buffer);
  void Map(size_t size, size_t offset = 0);
  void UnMap();
  void Update(void *data, size_t size, size_t offset = 0);
  void Flush();
  uint32_t FindMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);

  static Ptr Create_Image_buffer(Device::Ptr device, size_t size);
};
} // namespace MoChengEngine::FrameWork::Wrapper