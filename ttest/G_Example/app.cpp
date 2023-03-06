/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-30 09:51:44
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2023-02-04 20:06:54
 * @FilePath: \MoChengEngine\G_Example\app.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "app.h"
#include "FrameWork/Core/UniformManager/UniformManager.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Fence.h"
#include "FrameWork/Wrapper/Semaphore.h"
#include "FrameWork/Wrapper/ShaderModule.h"
#include "vulkan/vulkan_core.h"

#include <algorithm>
#include <vector>
namespace MoChengEngine {

static void cursorPosCallBack(GLFWwindow *window, double xpos, double ypos) {
  auto pUserData = reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

  pUserData->OnMouseMove(xpos, ypos);
}
void App::OnMouseMove(double xpos, double ypos) {
  // std::cout<<xpos<<" "<<ypos<<std::endl;
  mCamera.onMouseMove(xpos, ypos);
}

void App::Run() {
  Init();
  Prepare();
  MainLoop();
  //   CleanUp();
}

void App::Init() {

  m_Window = Glfw_Window_my::Create();

  glfwSetWindowUserPointer(m_Window->Get_handle(), this);

  glfwSetCursorPosCallback(m_Window->Get_handle(), cursorPosCallBack);

  mCamera.lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f));
  mCamera.update();

  mCamera.setPerpective(45.0f, (float)m_Width / (float)m_Height, 0.1f, 100.0f);

  mCamera.setSpeed(0.05f);
  //--
  m_Instance = Instance::Create();

  m_Surface = WindowSurface::Create(m_Instance, m_Window);

  m_Device = Device::Create(m_Instance, m_Instance->Get_gpus()[0], m_Surface);

  m_model = FrameWork::Model::Create(m_Device);

  m_render_context =
      std::make_unique<Rendering::RenderContext>(m_Device, m_Window, m_Surface);
}
void App::Prepare() {
  m_render_context->Prepare();
  Prepare_Model();
  Prepare_shader_module();
  Prepare_UniformManager();
  Prepare_Pipeline();
  auto swapchain_size = m_render_context->Get_frames().size();
  command_buffers.resize(swapchain_size);
  for (int i = 0; i < swapchain_size; i++) {

    // command_buffer->Set_Fence(FrameWork::Wrapper::Fence::Create(m_Device));
    command_buffers[i] = FrameWork::Wrapper::CommandBuffer::CreateR(
        m_Device, m_render_context->Get_frames()[i]->Get_command_pool(
                      m_render_context->Get_command_queue()));

    // m_render_context->Get_frames()[i]->request_command_buffer(
    //     m_render_context->Get_command_queue());
    Record(command_buffers[i], i);
    present_finish_semaphore.push_back(Semaphore::Create(m_Device));

    render_finish_semaphore.push_back(Semaphore::Create(m_Device));
    fences.push_back(Fence::Create(m_Device));
  }
}

void App::Prepare_UniformManager() {
  // prepare uniformManager
  m_uniformManager = std::make_unique<FrameWork::UniformManager>();
  //   auto command_buffer =
  //   std::vector<FrameWork::Wrapper::CommandBuffer::Ptr> command_buffers;

  m_uniformManager->Prepare(
      m_Device,
      FrameWork::COMMAND{
          .request_command_buffer =
              [this]() { return m_render_context->request_command_buffer(); },
          .queue = m_render_context->Get_command_queue()},
      m_render_context->Get_swap_chain()->Get_image_count());

  //   m_render_context->Add_Prepare_command_buffers({command_buffer});
}
void App::Prepare_Model() {

  auto command_buffer = m_render_context->request_command_buffer();
  command_buffer->Begin();
  m_model->loadModel("D:\\MoChengEngine\\assets\\jqm.obj", m_Device,
                     command_buffer);
  command_buffer->Wait(m_render_context->Get_command_queue());
}
void App::Prepare_shader_module() {
  //   std::vector<FrameWork::Wrapper::ShaderModule::Ptr> shader_modules{};

  auto shaderVertex = FrameWork::Wrapper::ShaderModule::CreateR(
      m_Device, "D:/MoChengEngine/Shader\\vs.spv", VK_SHADER_STAGE_VERTEX_BIT,
      "main");
  shader_modules.push_back(shaderVertex);

  auto shaderFragment = FrameWork::Wrapper::ShaderModule::CreateR(
      m_Device, "D:/MoChengEngine/Shader\\fs.spv", VK_SHADER_STAGE_FRAGMENT_BIT,
      "main");
  shader_modules.push_back(shaderFragment);
}
void App::Prepare_Pipeline() {

  // prepare pipeline
  auto render_pass = m_render_context->Get_Render_Pass();
  auto vertex_input_descriptions = m_model->getVertexInputBindingDescriptions();
  auto attributeDes = m_model->getAttributeDescriptions();
  m_pipeline = std::make_unique<Rendering::GraphicPipeline>(
      m_Device, vertex_input_descriptions, attributeDes, render_pass,
      m_uniformManager->GetDescriptorLayout()->Get_handle(),
      m_Device->getMaxUsableSampleCount());
  std::vector<VkPipelineShaderStageCreateInfo> shaders{};

  for (const auto &i : shader_modules) {

    shaders.emplace_back(i->Make_Createinfo_in_pipeline());
  }

  m_pipeline->Build_pipeline(shaders);
}

void App::Record(FrameWork::Wrapper::CommandBuffer::Ptr &command_buffer,
                 int frame_index) {
  VkRenderPassBeginInfo renderBeginInfo{};
  renderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

  renderBeginInfo.renderPass =
      m_render_context->Get_Render_Pass()->Get_handle();
  renderBeginInfo.framebuffer = m_render_context->Get_frames()[frame_index]
                                    ->Get_framebuffer()
                                    ->Get_handle();
  renderBeginInfo.renderArea.offset = {0, 0};
  renderBeginInfo.renderArea.extent =
      m_render_context->Get_swap_chain()->Get_extent2D();
  std::vector<VkClearValue> clearColors{};
  VkClearValue finalClearColor{};
  finalClearColor.color = {0.0f, 0.0f, 0.0f, 1.0f};
  clearColors.push_back(finalClearColor);

  VkClearValue mutiClearColor{};
  mutiClearColor.color = {0.0f, 0.0f, 0.0f, 1.0f};
  clearColors.push_back(mutiClearColor);

  VkClearValue depthClearColor{};
  depthClearColor.depthStencil = {1.0f, 0};
  clearColors.push_back(depthClearColor);
  renderBeginInfo.clearValueCount = clearColors.size();
  renderBeginInfo.pClearValues = clearColors.data();

  command_buffer->Begin();
  command_buffer->BeginRenderPass(renderBeginInfo);
  command_buffer->BindGraphicPipeline(m_pipeline->Get_handle());

  command_buffer->BindDescriptorSet(
      m_pipeline->Get_Layout(),
      m_uniformManager->GetDescriptorSet()[frame_index]);

  command_buffer->BindVertexBuffer(m_model->getVertexBuffers());
  command_buffer->BindIndexBuffer(m_model->getIndexBuffer()->Get_handle());
    // command_buffer->Draw(3);
 

  command_buffer->DrawIndex(m_model->getIndexCount());

  command_buffer->EndRenderPass();

  command_buffer->End();
}
void App::Rendering() {
  auto &command_buffer =
      command_buffers[m_render_context->Get_active_frame_index()];
  auto fence = fences[m_render_context->Get_active_frame_index()];

#ifdef Test
  // test
  current_frame = image_index;
  m_render_context->Get_active_frame_index() = image_index;
  auto result = vkAcquireNextImageKHR(
      m_Device->Get_handle(), m_render_context->Get_swap_chain()->Get_handle(),
      UINT64_MAX, present_finish_semaphore[current_frame]->Get_handle(),
      VK_NULL_HANDLE, &image_index);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  // 设置同步信息

  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores =
      &present_finish_semaphore[current_frame]->Get_handle();
  submitInfo.pWaitDstStageMask = waitStages;

  // 提交哪些命令

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &command_buffer->Get_handle();

  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores =
      &render_finish_semaphore[current_frame]->Get_handle();
  // 等待该槽位的上一个commandBuffer执行完毕

    fence->Wait();
    // 阻塞住
    fence->Reset();
  if (vkQueueSubmit(m_render_context->Get_command_queue()->Get_handle(), 1,
                    &submitInfo, fence->Get_handle()) != VK_SUCCESS) {
    throw std::runtime_error("Fail");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores =
      &render_finish_semaphore[current_frame]->Get_handle();

  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &m_render_context->Get_swap_chain()->Get_handle();
  presentInfo.pImageIndices = &image_index;
  //   vkQueueWaitIdle(m_render_context->Get_command_queue()->Get_handle());
  result = vkQueuePresentKHR(
      m_render_context->Get_command_queue()->Get_handle(), &presentInfo);
  std::cout << m_render_context->Get_active_frame_index() << std::endl
            << "--" << std::endl;

#else
    // std::cout << m_render_context->Get_active_frame_index() << std::endl
    //           << "--" << std::endl;

  fence->Wait();
  fence->Reset();
  m_render_context->Begin_frame();
  m_render_context->Submit({command_buffer}, fence->Get_handle());
#endif
}
void App::MainLoop() {
  while (!m_Window->ShouldClose()) {
    m_Window->PollEvent();
    m_VPMatrices.mViewMatrix = mCamera.getViewMatrix();
    m_VPMatrices.mProjectionMatrix = mCamera.getProjectMatrix();
    // std::cout << m_render_context->Get_active_frame_index() << std::endl;
    m_uniformManager->Update(m_VPMatrices, m_model->getUniform(),
                             m_render_context->Get_active_frame_index());
    Rendering();
  }
  vkDeviceWaitIdle(m_Device->Get_handle());
  std::cout << "end loop" << std::endl;
}
} // namespace MoChengEngine