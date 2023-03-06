/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 14:36:55
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 12:55:58
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Command\CommandBuffer.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "CommandBuffer.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Command/CommandQueue.h"
#include "vulkan/vulkan_core.h"

namespace MoChengEngine::FrameWork::Wrapper {

CommandBuffer::CommandBuffer(Device::Ptr device, CommandPool::Ptr commandPool,
                             bool asSecondary)
    : Component<VkCommandBuffer, CommandBuffer>{device}, m_commandPool{
                                                             commandPool} {

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandBufferCount = 1;
  allocInfo.commandPool = m_commandPool->Get_handle();
  allocInfo.level = asSecondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY
                                : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
 
  VK_CHECK_SUCCESS(
      vkAllocateCommandBuffers(m_device->Get_handle(), &allocInfo, &m_handle),
      "falied to create commandBuffer");
}
CommandBuffer::~CommandBuffer() {
  std::cout << "Free command buffer" << m_handle << std::endl;
  vkFreeCommandBuffers(m_device->Get_handle(), m_commandPool->Get_handle(), 1,
                       &m_handle);
}

void CommandBuffer::Add_Task(Command_Buffer_Task task) { task(); }
void CommandBuffer::Wait(CommandQueue::Ptr command_queue) {
  End();
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_handle;
  vkQueueSubmit(command_queue->Get_handle(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(command_queue->Get_handle());
}

void CommandBuffer::Wait_All(CommandQueue::Ptr command_queue,
                             std::vector<CommandBuffer::Ptr> command_buffers) {
  for (const auto &i : command_buffers) {
    i->Wait(command_queue);
  }
}
void CommandBuffer::Begin(VkCommandBufferUsageFlags flag,
                          const VkCommandBufferInheritanceInfo &inheritance) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = flag;
  beginInfo.pInheritanceInfo = &inheritance;
  VK_CHECK_SUCCESS(vkBeginCommandBuffer(m_handle, &beginInfo),
                   "failed to begin commandBuffer");
}
void CommandBuffer::End() {
  VK_CHECK_SUCCESS(vkEndCommandBuffer(m_handle),
                   "failed to end Command Buffer");
}
void CommandBuffer::BeginRenderPass(
    const VkRenderPassBeginInfo &renderPassBeginInfo,
    const VkSubpassContents &subPassContents) {
  vkCmdBeginRenderPass(m_handle, &renderPassBeginInfo, subPassContents);
}
void CommandBuffer::EndRenderPass() { vkCmdEndRenderPass(m_handle); }
void CommandBuffer::BindGraphicPipeline(const VkPipeline &pipeline) {
  vkCmdBindPipeline(m_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}
void CommandBuffer::BindVertexBuffer(const std::vector<VkBuffer> &buffers) {
  std::vector<VkDeviceSize> offsets(buffers.size(), 0);
  vkCmdBindVertexBuffers(m_handle, 0, static_cast<uint32_t>(buffers.size()),
                         buffers.data(), offsets.data());
}
void CommandBuffer::BindDescriptorSet(const VkPipelineLayout layout,
                                      const VkDescriptorSet &descriptorSet) {
  vkCmdBindDescriptorSets(m_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0,
                          1, &descriptorSet, 0, nullptr);
}
void CommandBuffer::BindIndexBuffer(const VkBuffer &buffer) {
  vkCmdBindIndexBuffer(m_handle, buffer, 0, VK_INDEX_TYPE_UINT32);
}
void CommandBuffer::DrawIndex(size_t indexCount) {
  vkCmdDrawIndexed(m_handle, indexCount, 1, 0, 0, 0);
}
void CommandBuffer::Draw(size_t indexCount) {

  vkCmdDraw(m_handle, indexCount, 1, 0, 0);
}

void CommandBuffer::CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage,
                                      VkImageLayout dstImageLayout,
                                      uint32_t width, uint32_t height) {
  VkBufferImageCopy region{};

  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(m_handle, srcBuffer, dstImage, dstImageLayout, 1,
                         &region);
}

void CommandBuffer::TransferImageLayout(VkImageMemoryBarrier imageMemoryBarrier,
                                        VkPipelineStageFlags srcStageMask,
                                        VkPipelineStageFlags dstStageMask) {
  vkCmdPipelineBarrier(m_handle, srcStageMask, dstStageMask, 0, 0,
                       nullptr,    // MemoryBarrier
                       0, nullptr, // BufferMemoryBarrier
                       1, &imageMemoryBarrier);
}

void CommandBuffer::CopyBufferToBuffer(VkBuffer src_buffer, VkBuffer dst_buffer,
                                       VkDeviceSize size) {
  VkBufferCopy copy_info{};
  copy_info.size = size;
  vkCmdCopyBuffer(m_handle, src_buffer, dst_buffer, 1, &copy_info);
}
} // namespace MoChengEngine::FrameWork::Wrapper