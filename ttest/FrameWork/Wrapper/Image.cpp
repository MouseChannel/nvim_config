
#include "Image.h"
#include "FrameWork/Wrapper/Buffer.h"
#include "vma/vk_mem_alloc.h"
#include "vulkan/vulkan_core.h"

#include <assert.h>
#include <stdint.h>
//  #include "FrameWork/Base/vmaExporter.cpp"
namespace MoChengEngine::FrameWork::Wrapper {
Image::Image(const Device::Ptr device, uint32_t width, uint32_t height,
             const VkFormat format, const VkImageType imageType,
             const VkImageTiling tiling, const VkImageUsageFlags image_usage,
             const VkSampleCountFlagBits sample,
             const VkMemoryPropertyFlags properties,
             const VkImageAspectFlags aspectFlags,
             const VmaMemoryUsage memory_usage, bool auto_destroy)
    : Resource<VkImage, Image>{device}, m_width{width}, m_height{height},
      m_format{format}, m_imageType{imageType}, auto_destroy{auto_destroy} {
  VkImageCreateInfo imageCreateInfo{};
  imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageCreateInfo.extent = VkExtent3D{width, height, 1};

  imageCreateInfo.format = format; // rgb rgba
  imageCreateInfo.imageType = imageType;
  imageCreateInfo.tiling = tiling;
  imageCreateInfo.usage = image_usage; // color or depth?
  imageCreateInfo.samples = sample;
  imageCreateInfo.mipLevels = 1;
  imageCreateInfo.arrayLayers = 1;
  imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

#ifdef Using_VMA
  // create image in vma

  VmaAllocationCreateInfo memory_info{};
  memory_info.usage = memory_usage;

  if (image_usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
    memory_info.preferredFlags = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
  }
  VmaAllocationInfo allocation_info{};
  VK_CHECK_SUCCESS(vmaCreateImage(m_device->Get_allocator(), &imageCreateInfo,
                                  &memory_info, &m_handle, &allocation,
                                  &allocation_info),
                   "Create image failed");
  m_ImageMemory = allocation_info.deviceMemory;
#else

  VK_CHECK_SUCCESS(vkCreateImage(m_device->Get_handle(), &imageCreateInfo,
                                 nullptr, &m_handle),
                   "create image failed");

  VkMemoryRequirements memReq{};
  vkGetImageMemoryRequirements(m_device->Get_handle(), m_handle, &memReq);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memReq.size;

  allocInfo.memoryTypeIndex =
      m_device->Get_gpu()->FindMemoryType(memReq.memoryTypeBits, properties);

  if (vkAllocateMemory(m_device->Get_handle(), &allocInfo, nullptr,
                       &m_ImageMemory) != VK_SUCCESS) {
    throw std::runtime_error("Error: failed to allocate memory");
  }

  vkBindImageMemory(m_device->Get_handle(), m_handle, m_ImageMemory, 0);
#endif
  VkImageViewCreateInfo imageViewCreateInfo{};
  imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  imageViewCreateInfo.format = format;
  imageViewCreateInfo.image = m_handle;
  imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;
  imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
  imageViewCreateInfo.subresourceRange.levelCount = 1;
  imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
  imageViewCreateInfo.subresourceRange.layerCount = 1;

  if (vkCreateImageView(m_device->Get_handle(), &imageViewCreateInfo, nullptr,
                        &m_view) != VK_SUCCESS) {
    throw std::runtime_error("Error: failed to create image view");
  }
  //   m_view = CreateView(Make_View_Info(aspectFlags), m_device);
}
Image::Image(const Device::Ptr device, VkImage  image_handle,
             const VkExtent3D extent, VkFormat format,
             VkImageAspectFlags aspectFlags, bool auto_destroy)
    : Resource<VkImage, Image>{device}, m_extent{extent},
      m_format(format), m_width{extent.width}, m_height{extent.height},
      m_imageType{VK_IMAGE_TYPE_2D}, auto_destroy(auto_destroy) {
  m_handle = image_handle;

  VkImageViewCreateInfo imageViewCreateInfo{};
  imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  imageViewCreateInfo.format = format;
  imageViewCreateInfo.image = m_handle;
  imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;
  imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
  imageViewCreateInfo.subresourceRange.levelCount = 1;
  imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
  imageViewCreateInfo.subresourceRange.layerCount = 1;

  if (vkCreateImageView(m_device->Get_handle(), &imageViewCreateInfo, nullptr,
                        &m_view) != VK_SUCCESS) {
    throw std::runtime_error("Error: failed to create image view");
  }
  //   m_view = CreateView(Make_View_Info(VK_IMAGE_ASPECT_COLOR_BIT), m_device);
}
Image::Image(Device::Ptr device, VkImage image_handle, VkImageView image_view,
             VkExtent3D extent, VkFormat format, VkImageAspectFlags aspectFlags,
             bool auto_destroy)
    : Resource<VkImage, Image>{device}, m_extent{extent},
      m_format(format), m_width{extent.width}, m_height{extent.height},
      m_imageType{VK_IMAGE_TYPE_2D}, auto_destroy(auto_destroy) {
  m_handle = image_handle;
  m_view = image_view;
}
Image::~Image() {

  if (m_view != nullptr) {
    vkDestroyImageView(m_device->Get_handle(), m_view, nullptr);
  }
  if (auto_destroy)
    return;
#ifdef Using_VMA
  vmaDestroyImage(m_device->Get_allocator(), m_handle, allocation);

#else

  if (m_ImageMemory != VK_NULL_HANDLE) {
    vkFreeMemory(m_device->Get_handle(), m_ImageMemory, nullptr);
  }

  if (m_handle != VK_NULL_HANDLE) {
    vkDestroyImage(m_device->Get_handle(), m_handle, nullptr);
  }
#endif
}
/*
void Image::Bind_Image_Memory(const VkMemoryPropertyFlags properties) {
  VkMemoryRequirements memReq{};
  vkGetImageMemoryRequirements(m_device->Get_handle(), m_handle, &memReq);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memReq.size;

  allocInfo.memoryTypeIndex =
      m_device->Get_gpu()->FindMemoryType(memReq.memoryTypeBits, properties);
  VK_CHECK_SUCCESS(vkAllocateMemory(m_device->Get_handle(), &allocInfo,
  nullptr,
                                    &m_ImageMemory),
                   "Error: failed to allocate memory");

  vkBindImageMemory(m_device->Get_handle(), m_handle, m_ImageMemory, 0);
}
*/
VkImageViewCreateInfo
Image::Make_View_Info(const VkImageAspectFlags aspectFlags) {
  VkImageViewCreateInfo imageViewCreateInfo{};
  imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  imageViewCreateInfo.format = m_format;
  imageViewCreateInfo.image = m_handle;
  imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;
  imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
  imageViewCreateInfo.subresourceRange.levelCount = 1;
  imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
  imageViewCreateInfo.subresourceRange.layerCount = 1;
  return imageViewCreateInfo;
}

// 填充该image内容
void Image::FillImageData(size_t size, void *pData, Buffer::Ptr &buffer,
                          CommandBuffer::Ptr commandBuffer) {
  assert(pData);
  assert(size);

  buffer->Update(pData, size);
  commandBuffer->CopyBufferToImage(buffer->Get_handle(), m_handle, m_layout,
                                   m_width, m_height);
  //   return [=]() {
  //     commandBuffer->CopyBufferToImage(buffer->Get_handle(), m_handle,
  //     m_layout,
  //                                      m_width, m_height);
}

// 使用barrier修改image格式
void Image::SetImageLayout(VkImageLayout newLayout,
                           VkPipelineStageFlags srcStageMask,
                           VkPipelineStageFlags dstStageMask,
                           VkImageSubresourceRange subresrouceRange,
                           CommandBuffer::Ptr commandBuffer) {
  auto oldLayout = m_layout;
  VkImageMemoryBarrier imageMemoryBarrier{};
  imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  imageMemoryBarrier.oldLayout = oldLayout;
  imageMemoryBarrier.newLayout = newLayout;
  imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  imageMemoryBarrier.image = m_handle;
  imageMemoryBarrier.subresourceRange = subresrouceRange;
  switch (oldLayout) {
    // 说明图片刚被创建，上方是初始化的虚拟操作
  case VK_IMAGE_LAYOUT_UNDEFINED:
    imageMemoryBarrier.srcAccessMask = 0;
    break;
    // 如果要写入该图片，需要等待write操作完成
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    break;
  default:
    break;
  }

  switch (newLayout) {
    // 如果目标是：将图片作为被写入的对象，则被阻塞的操作必定是write操作
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    break;
    // 如果目标是：将格式转化为一个可读的纹理，那么被阻塞的必定是read操作
    // 如果该image作为texture，那么来源只能是：1.通过map从cpu端拷贝过来，2.通过stagingBuffer拷贝而来
  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    break;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: {
    imageMemoryBarrier.dstAccessMask =
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  } break;
  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: {
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  } break;
  default:
    break;
  }
  m_layout = newLayout;
  commandBuffer->TransferImageLayout(imageMemoryBarrier, srcStageMask,
                                     dstStageMask);
}

} // namespace MoChengEngine::FrameWork::Wrapper