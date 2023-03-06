/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 16:46:25
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2023-02-04 20:23:09
 * @FilePath: \MoChengEngine\FrameWork\Core\Rendering\RenderFrame.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "RenderFrame.h"
#include "FrameWork/Core/ObjectPool.hpp"
#include "FrameWork/Core/Rendering/RenderFrame.h"
#include "FrameWork/Core/Rendering/Render_Target/Render_target_base.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Command/CommandPool.h"
#include "FrameWork/Wrapper/FrameBuffer.h"
#include "FrameWork/Wrapper/RenderPass.h"
#include "FrameWork/Wrapper/Semaphore.h"
#include "vulkan/vulkan_core.h"
#include <algorithm>
namespace MoChengEngine::FrameWork::Core::Rendering {
RenderFrame::RenderFrame(Wrapper::Device::Ptr &device)
    : m_device{device}, m_render_finish_semaphore{Wrapper::Semaphore::Create(
                            m_device)},
      m_present_finish_semaphore{Wrapper::Semaphore::Create(m_device)} {}

RenderFrame::~RenderFrame() {}
void RenderFrame::Prepare(
    Wrapper::RenderPass::Ptr render_pass,
    std::vector<std::unique_ptr<RenderTarget>> &&renderTarget) {
  m_swapchain_render_targets = std::move(renderTarget);

  Prepare_frame_buffer(render_pass);
}
void RenderFrame::Prepare_frame_buffer(Wrapper::RenderPass::Ptr render_pass) {
  std::vector<VkImageView> swapchian_image_views;
  for (auto &i : m_swapchain_render_targets) {
    swapchian_image_views.emplace_back(i->Get_image_view());
  }

  m_frame_buffer = Wrapper::FrameBuffer::CreateR(
      m_device, m_swapchain_render_targets[0]->Get_extent(),
      swapchian_image_views, render_pass);
}

Wrapper::CommandPool::Ptr
RenderFrame::Get_command_pool(Wrapper::CommandQueue::Ptr command_queue) {
  auto target_command_pool =
      std::find_if(m_command_pool.begin(), m_command_pool.end(),
                   [command_queue](auto command_pool) {
                     return command_pool->Get_queue_family_index() ==
                            command_queue->Get_family_index();
                   });
  if (target_command_pool == m_command_pool.end()) {
    // did not contains suitable command_pool, create one
    m_command_pool.emplace_back(Wrapper::CommandPool::Create(
        m_device, command_queue->Get_family_propertie().queueFlags));
    return m_command_pool.back();
  }
  return *target_command_pool;
}
Wrapper::CommandBuffer::Ptr
RenderFrame::request_command_buffer(Wrapper::CommandQueue::Ptr command_queue) {
  auto  command_pool = Get_command_pool(command_queue);
  return command_buffers.request_object([this, &command_pool]() {
    return Wrapper::CommandBuffer::Create(m_device, command_pool);
  });
  
}

} // namespace MoChengEngine::FrameWork::Core::Rendering